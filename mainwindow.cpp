#include "mainwindow.h"

#include <QtWidgets>  // TODO: use specific headers

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    // init this window's state
    this->setObjectName("main_app_window");
    this->setFixedSize(800, 600);
    this->setWindowTitle(QApplication::translate("appctx", "PerClass108"));
    QWidget* centralWidget = new QWidget{};
    centralWidget->setObjectName("main_app_window_central_widget");

    // configure menu bar
    if (false) {  // TODO: not required for MVP
        auto* menuBar = this->menuBar();
        menuBar->setObjectName("menubar");
        QMenu* fileMenu = menuBar->addMenu("File");

        auto* action = new QAction{"open_action", this};
        action->setText("Open");

        fileMenu->addAction(action);
    }

    // create a hbox for the child widgets, so they're laid out horizontally
    if (true) {
        QHBoxLayout* hbox = new QHBoxLayout{this->centralWidget()};
        hbox->setObjectName("main_area_hbox");
        hbox->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);

        // left-hand side: image viewer
        auto* imageViewer = new QPushButton{QApplication::translate("appctx", "Press me")};
        hbox->addWidget(imageViewer);

        // right-hand side: HSV viewer/editor
        auto* hsvEditor = new QPushButton{QApplication::translate("appctx", "Press me")};
        hbox->addWidget(hsvEditor);
    }

    // configure status bar
    if (false) {  // TODO: not required for MVP
        QStatusBar* statusBar = this->statusBar();
        statusBar->addWidget(new QLabel{"hello"});
    }
}
