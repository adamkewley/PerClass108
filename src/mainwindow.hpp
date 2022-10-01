#pragma once

#include <QMainWindow>

namespace pc { class ImageViewer; }

namespace pc
{
    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow() noexcept;

    private slots:
        void promptUserForImage(bool checked);
        void userRequestedExit();

    private:
        void dragEnterEvent(QDragEnterEvent*) override;
        void dragMoveEvent(QDragMoveEvent*) override;
        void dropEvent(QDropEvent*) override;

        ImageViewer* m_ImageViewer;
    };
}
