#pragma once

#include <QWidget>
#include <QColor>

#include <memory>

class QLabel;
class QImage;
class QMouseEvent;
class QEvent;

namespace pc {

    class ImageViewer final : public QWidget {
        Q_OBJECT

    public:
        explicit ImageViewer(QString const& filepath, QWidget* parent = nullptr);

    public slots:
        void setImageFile(QString const&);

    signals:
        // emits when mouse moves over image
        void mouseMoveOverColor(QColor const&);

    private:
        void mouseMoveEvent(QMouseEvent*) override;
        void paintEvent(QPaintEvent*) override;

        std::unique_ptr<QImage> image;
    };
}
