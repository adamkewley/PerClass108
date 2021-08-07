#include "hsvviewer.h"

#include "hsvviewer.moc"

#include <QWidget>
#include <QVBoxLayout>
#include <QPainter>
#include <QLabel>
#include <QMouseEvent>



pc::HSViewerColorCircle::HSViewerColorCircle(QWidget* parent) : QWidget{parent} {
    this->setFixedSize(256, 256);  // size of the wheel
    this->setMouseTracking(true);  // always capture mouse events, even if the mouse isn't clicked
}

void pc::HSViewerColorCircle::mouseMoveEvent(QMouseEvent* e) {
    QPoint pos = e->pos();

    // set dummy placeholder value
    emit mouseMoveOverHSV(HSV{static_cast<float>(pos.x()), static_cast<float>(pos.y()), 0.0f});
}

void pc::HSViewerColorCircle::paintEvent(QPaintEvent*) {
    QPainter painter{this};
    painter.setPen(QColor{255, 0, 0});
    painter.drawLine(0, 0, 256, 256);
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
