#pragma once

#include <QWidget>

class QLabel;

namespace pc {
    class ImageViewer final : public QWidget {
        Q_OBJECT

    public:
        explicit ImageViewer(QString const& filepath, QWidget* parent = nullptr);

    public slots:
        void setImageFile(QString const&);

    private:
        QLabel* label;
    };
}
