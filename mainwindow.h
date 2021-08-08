#pragma once

#include <QMainWindow>

namespace pc {
    class ImageViewer;

    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow() noexcept;

    private slots:
        void promptUserForImage(bool checked);

    private:
        ImageViewer* imgViewer;
    };
}
