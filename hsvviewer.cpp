#include "hsvviewer.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>
#include <QSlider>

namespace {

    // result from attempting to map a screenspace coordinate into HSV
    struct HSVInterpolationResult final {
        pc::HSV result = {0.0f, 0.0f, 0.0f};
        bool inBounds = false;
    };

    // try to map `posPxV`, a pixel position vector in a rect (left-handed coord system),
    // that is two radii accross and two radii high in HSV space (`dimsPxV`) into the
    // HSV color space at some value (V)
    //
    // result is only valid if the position was in-bounds
    HSVInterpolationResult screenPosToHSV(QVector2D posPxV, QVector2D dimsPxV, float v) {
        HSVInterpolationResult rv;

        // remap pixel position (x, y) into relative coordinates (2x/w - 1.0f, 2y/w - 1.0f)
        //
        // yields values in the range [-1.0f, 1.0f], still in the left-handed coord system
        QVector2D posR = posPxV/dimsPxV * 2.0f;  // [0.0f, 2.0f]
        posR -= QVector2D{1.0f, 1.0f};   // [-1.0f, 1.0f]

        // the coordinate mapping above is (conveniently) centered at the center of
        // the circle

        // S (saturation) is just the distance from the center of the circle
        float s = posR.length();

        if (s > 1.0f) {
            rv.inBounds = false;
            return rv;
        }

        // H (hue) is the proportion of the angle between a center-to-top vector
        // and the position
        //
        // e.g. a point perfectly above the middle maps to 0. A point perfectly
        // below maps to 0.5, go clockwise

        // had to look this up, which is why it's hacky
        //
        // https://stackoverflow.com/questions/14066933/direct-way-of-computing-clockwise-angle-between-2-vectors

        QVector2D upVertical{0.0f, -1.0f};  // in a left-handed coord system
        QVector2D posRNormalized = posR.normalized();

        float dot = QVector2D::dotProduct(upVertical, posRNormalized);
        float det = upVertical.x()*posRNormalized.y() - upVertical.y()*posRNormalized.x();
        float ang = atan2(det, dot); // goes 0 -> pi, then -pi -> 0 when past the midpoint

        // remap into [0, 2pi]
        if (ang < 0.0f) {
            ang = M_PI + (M_PI + ang);
        }

        // remap into [0, 1]
        float h = ang / (2.0f * M_PI);

        rv.result = {h, s, v};
        rv.inBounds = true;

        return rv;
    }

    // basic RGB definition for internal use
    struct RGB final {
        float r, g, b;
    };

    // convert a HSV value into a QColor
    QColor hsv2QColor(pc::HSV const& hsv) {
        QColor c;
        c.setHsvF(hsv.h, hsv.s, hsv.v);
        return c;
    }

    // convert a HSV value into an RGB value
    RGB hsv2rgb(pc::HSV const& hsv) {
        // cheekily use QColor, rather than roll my own version

        QColor c = hsv2QColor(hsv);
        RGB rgb;
        c.getRgbF(&rgb.r, &rgb.g, &rgb.b);
        return rgb;
    }

    constexpr float hsv_senteniel = -1337.0f;
}

pc::HSV::HSV(QColor const& c) {
    c.getHsvF(&h, &s, &v);
}

pc::HSViewerColorCircle::HSViewerColorCircle(QWidget* parent) :
    QWidget{parent},
    hsv{hsv_senteniel, hsv_senteniel, 1.0f} {

    this->setFixedSize(256, 256);  // size of the wheel
    this->setMouseTracking(true);  // always capture mouse events, even if the mouse isn't clicked
}

void pc::HSViewerColorCircle::mouseMoveEvent(QMouseEvent* e) {
    // position in widget space (pixel location, left-handed coord system)
    QPointF posPx = e->position();
    QVector2D posPxV{static_cast<float>(posPx.x()), static_cast<float>(posPx.y())};

    // dimensions in widget space (pixel dims)
    QSizeF dimsPx = this->size();
    QVector2D dimsPxV{static_cast<float>(dimsPx.width()), static_cast<float>(dimsPx.height())};

    auto [newHsv, inBounds] = screenPosToHSV(posPxV, dimsPxV, hsv.v);

    if (!inBounds) {
        return;
    }

    hsv = newHsv;

    emit mouseMoveOverColor(QColor::fromHsvF(hsv.h, hsv.s, hsv.v));

    this->update();
}

void pc::HSViewerColorCircle::paintEvent(QPaintEvent*) {
    // HACK: brute force it by coloring each pixel in software
    //
    // a good implementation would use QRadialGradient etc. to
    // hardware-interpolate the circle

    QSize dims = this->size();
    QVector2D dimsV{static_cast<float>(dims.width()), static_cast<float>(dims.height())};

    QPainter painter{this};

    float v = hsv.v;
    for (int y = 0; y < dims.height(); ++y) {
        for (int x = 0; x < dims.width(); ++x) {
            QVector2D posV{static_cast<float>(x), static_cast<float>(y)};
            auto [hsv, inBounds] = screenPosToHSV(posV, dimsV, v);
            if (inBounds) {
                painter.setPen(hsv2QColor(hsv));
                painter.drawRect(x, y, 1, 1);
            }

        }
    }

    // also, if h and s aren't senteniels (either because the user moused
    // over a value or it was externally set) draw a circle at the hsv pos
    if (hsv.h != hsv_senteniel && hsv.s != hsv_senteniel) {

        // figure out point location on circle. Saturation (S)
        // is radius, Hue (H) is rotation from up vertical
        QPointF up{0.0f, -hsv.s * (dimsV.x()/2.0f)};
        QTransform xform{};
        xform.translate(dimsV.x()/2.0f, dimsV.y()/2.0f);
        xform.rotateRadians(2.0f * M_PI * hsv.h);

        QPointF origin = xform.map(up);

        QRect rect(origin.x() - 1, origin.y() - 1, 3, 3);

        painter.setPen(QColor::fromRgb(0, 0, 0));
        painter.setBrush(QColor::fromRgb(0, 0, 0));
        painter.drawRect(rect);
    }
}

void pc::HSViewerColorCircle::setValue(float v) {
    this->hsv.v = v;
    this->update();
}

void pc::HSViewerColorCircle::setColor(QColor const& c) {
    this->hsv = HSV{c};
    this->update();
}

pc::HSViewerValueSlider::HSViewerValueSlider(QWidget* parent) :
    slider{new QSlider{Qt::Orientation::Vertical, parent}} {

    slider->setMaximum(1024);
    slider->setValue(1024);

    QVBoxLayout* vbox = new QVBoxLayout{this};
    vbox->addWidget(slider);

    connect(slider, &QSlider::valueChanged, this, &HSViewerValueSlider::sliderValueChanged);
}

void pc::HSViewerValueSlider::setValue(float v) {
    slider->setValue(static_cast<int>(v * 1024.0f));
}

void pc::HSViewerValueSlider::sliderValueChanged(int v) {
    emit valueChangedEvent(static_cast<float>(v) / 1024);
}

pc::HSViewerDetails::HSViewerDetails(QWidget* parent) :
    QWidget{parent},

    // HSV labels
    hLabel{new QLabel{"N/A", this}},
    sLabel{new QLabel{"N/A", this}},
    vLabel{new QLabel{"N/A", this}},

    // RGB labels
    rLabel{new QLabel{"N/A", this}},
    gLabel{new QLabel{"N/A", this}},
    bLabel{new QLabel{"N/A", this}} {

    // each label is vertically aligned
    QVBoxLayout* vbox = new QVBoxLayout{this};
    vbox->setObjectName("HSViewerDetails_vbox");

    // micro helper function for genrating a labelled label
    auto createAnnotatedLabel = [this, vbox](char const* text, QLabel* val) {
        QWidget* container = new QWidget{this};
        vbox->addWidget(container);

        QHBoxLayout* hbox = new QHBoxLayout{container};
        container->setLayout(hbox);

        QLabel* annotation = new QLabel{text, container};
        hbox->addWidget(annotation);
        hbox->addWidget(val);

        hbox->setStretch(1, 1);
        hbox->stretch(1);
    };

    // HSV printouts
    {
        createAnnotatedLabel("H = ", hLabel);
        createAnnotatedLabel("S = ", sLabel);
        createAnnotatedLabel("V = ", vLabel);
    }

    // separator line
    {
        QFrame* line = new QFrame{};
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        vbox->addWidget(line);
    }

    // RGB printouts
    {
        createAnnotatedLabel("R = ", rLabel);
        createAnnotatedLabel("G = ", gLabel);
        createAnnotatedLabel("B = ", bLabel);
    }
}

void pc::HSViewerDetails::setColor(QColor const& color) {

    // update HSV labels
    HSV hsv{color};
    hLabel->setText(QString::number(hsv.h));
    sLabel->setText(QString::number(hsv.s));
    vLabel->setText(QString::number(hsv.v));

    // update RGB labels
    RGB rgb = hsv2rgb(hsv);
    rLabel->setText(QString::number(rgb.r));
    gLabel->setText(QString::number(rgb.g));
    bLabel->setText(QString::number(rgb.b));
}


// top-level HSVViewer impl.

pc::HSVViewer::HSVViewer(QWidget *parent) :
    QWidget(parent),
    circle{new HSViewerColorCircle{this}},
    slider{new HSViewerValueSlider{this}},
    details{new HSViewerDetails{this}} {

    QVBoxLayout* vbox = new QVBoxLayout{this};
    vbox->setObjectName("HSVViewer_vbox");

    // top: circle (left) and value slider (right)

    // stub widget for top horizontal layout
    auto* container = new QWidget{this};
    vbox->addWidget(container);

    // lay the circle (HS) and slider (V) out horizontally
    auto* hbox = new QHBoxLayout{container};
    container->setLayout(hbox);

    // put circle on the left
    hbox->addWidget(circle);

    // put the slider on the right
    hbox->addWidget(slider);

    // bottom: HSV value printouts (as text values)
    vbox->addWidget(details);

    // connect the circle to the viewer, so the viewer displays values as the
    // mouse moves over it
    connect(circle, &HSViewerColorCircle::mouseMoveOverColor, details, &HSViewerDetails::setColor);

    // connect the slider to the circle
    connect(slider, &HSViewerValueSlider::valueChangedEvent, circle, &HSViewerColorCircle::setValue);
}

void pc::HSVViewer::setColor(QColor const& c) {
    circle->setColor(c);
    details->setColor(c);
    HSV hsv{c};
    slider->setValue(hsv.v);
}
