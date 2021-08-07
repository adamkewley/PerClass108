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
    // TODO: map the mouse position into a HSV event
}

void pc::HSViewerColorCircle::paintEvent(QPaintEvent*) {
    QPainter painter{this};
    painter.setPen(QColor{255, 0, 0});
    painter.drawLine(0, 0, 256, 256);
}



pc::HSViewerDetails::HSViewerDetails(QWidget* parent) : QWidget{parent} {
    QVBoxLayout* vbox = new QVBoxLayout{this};
    vbox->setObjectName("HSViewerDetails_vbox");

    auto* hLabel = new QLabel{"N/A"};
    vbox->addWidget(hLabel);
    auto* sLabel = new QLabel{"N/A"};
    vbox->addWidget(sLabel);
    auto* vLabel = new QLabel{"N/A"};
    vbox->addWidget(vLabel);
}

void pc::HSViewerDetails::setHSVValue(HSV) {
    // TODO: set the value, update any labels, etc.
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
