#pragma once

#include <QMainWindow>

namespace pc {
    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow() noexcept;
    };
}
