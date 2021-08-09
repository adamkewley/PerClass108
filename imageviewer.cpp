#include "imageviewer.h"

#include <QImage>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

pc::ImageViewer::ImageViewer(QString const& initialImage, QWidget* parent) :
    QWidget{parent},
    image{new QImage{initialImage}} {

    this->setLayout(new QVBoxLayout{this});
    this->setMinimumWidth(512);
    this->setMouseTracking(true);
}

void pc::ImageViewer::setImageFile(QString const& newPath) {
    auto newImage = std::make_unique<QImage>(newPath);

    if (newImage->isNull()) {
        return;  // error loading image
    }

    this->image = std::move(newImage);
    this->update();
}

void pc::ImageViewer::paintEvent(QPaintEvent* e) {
    QPainter{this}.drawImage(this->rect(), *this->image);
}

#include <iostream>

void pc::ImageViewer::mouseMoveEvent(QMouseEvent* e) {
    // the mouse move event is happening in a "stretched"
    // space, because the user is viewing a stretched blit
    // of the image

    QPointF p = e->position();
    float img_w = static_cast<float>(image->width());
    float img_h = static_cast<float>(image->height());
    float this_w = static_cast<float>(this->width());
    float this_h = static_cast<float>(this->height());

    int img_x = p.x() * (img_w/this_w);
    int img_y = p.y() * (img_h/this_h);

    QColor color = this->image->pixelColor(img_x, img_y);

    emit mouseMoveOverColor(color);
}
