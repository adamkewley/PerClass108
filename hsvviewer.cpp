#include "hsvviewer.h"

#include "hsvviewer.moc"

#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>



pc::HSViewerColorCircle::HSViewerColorCircle(QWidget* parent) : QWidget{parent}, v{0.5f} {
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
        return;  // mouse is out of bounds of the circle
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

    emit mouseMoveOverHSV(HSV{h, s, v});
}

void pc::HSViewerColorCircle::paintEvent(QPaintEvent*) {
    QSize dims = this->size();
    QRect rect{0, 0, dims.width(), dims.height()};

    QPainter painter{this};

    painter.setPen(QColor{255, 0, 0});

    painter.drawArc(rect, 0, 5760);
}

pc::HSViewerDetails::HSViewerDetails(QWidget* parent) :
    QWidget{parent},
    hLabel{new QLabel{"N/A", this}},
    sLabel{new QLabel{"N/A", this}},
    vLabel{new QLabel{"N/A", this}} {

    QVBoxLayout* vbox = new QVBoxLayout{this};
    vbox->setObjectName("HSViewerDetails_vbox");
    vbox->addWidget(hLabel);
    vbox->addWidget(sLabel);
    vbox->addWidget(vLabel);
}

void pc::HSViewerDetails::setHSVValue(HSV hsv) {
    hLabel->setText(QString::number(hsv.h));
    sLabel->setText(QString::number(hsv.s));
    vLabel->setText(QString::number(hsv.v));
}


// top-level HSVViewer impl.

pc::HSVViewer::HSVViewer(QWidget *parent) : QWidget(parent) {
    QVBoxLayout* vbox = new QVBoxLayout{this};
    vbox->setObjectName("HSVViewer_vbox");

    // top: the circle
    auto* circle = new HSViewerColorCircle{};
    vbox->addWidget(circle);

    // bottom: the data layout
    auto* viewer = new HSViewerDetails{};
    vbox->addWidget(viewer);

    connect(circle, &HSViewerColorCircle::mouseMoveOverHSV, viewer, &HSViewerDetails::setHSVValue);
}
