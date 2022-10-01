#include "imageviewer.hpp"

#include <QImage>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPointF>

pc::ImageViewer::ImageViewer(QString const& initialImage, QWidget* parent) :
    QWidget{parent},
    m_Image{new QImage{initialImage}}
{
    this->setLayout(new QVBoxLayout{this});
    this->setMinimumWidth(512);
    this->setMouseTracking(true);
}

void pc::ImageViewer::setImageFile(QString const& newPath)
{
    auto newImage = std::make_unique<QImage>(newPath);

    if (newImage->isNull())
    {
        return;  // error loading image
    }

    this->m_Image = std::move(newImage);
    this->update();
}

void pc::ImageViewer::paintEvent(QPaintEvent*)
{
    QPainter{this}.drawImage(this->rect(), *this->m_Image);
}

void pc::ImageViewer::mouseMoveEvent(QMouseEvent* e)
{
    // the mouse move event is happening in a "stretched"
    // space, because the user is viewing a stretched blit
    // of the image

    QPointF p = e->position();
    float imageWidth = static_cast<float>(m_Image->width());
    float imageHeight = static_cast<float>(m_Image->height());
    float widgetWidth = static_cast<float>(this->width());
    float widgetHeight = static_cast<float>(this->height());

    int imageX = p.x() * (imageWidth/widgetWidth);
    int imageY = p.y() * (imageHeight/widgetHeight);

    QColor color = this->m_Image->pixelColor(imageX, imageY);

    emit mouseMoveOverColor(color);
}
