#include "mainwindow.h"

#include "hsvviewer.h"

#include <QtWidgets>  // TODO: use specific headers

pc::MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    // init this window's state
    this->setObjectName("main_app_window");
    this->setGeometry(0, 0, 800, 0);
    this->move(this->screen()->geometry().center() - this->frameGeometry().center());
    this->setWindowTitle(QApplication::translate("appctx", "PerClass108"));
    this->setCentralWidget(new QWidget{});
    this->centralWidget()->setObjectName("main_app_window_central_widget");

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
        // TODO: at least load a hard-coded Qt Resource
        QImage img{"C:\\Users\\adamk\\OneDrive\\Desktop\\PerClass108\\smiley-face.jpg"};  // load the image into CPU memory

        if (!img.isNull()) {
            // loaded fine

            QPixmap imgPixmap = QPixmap::fromImage(img);  // "upload" (e.g. to GPU)
            imgPixmap = imgPixmap.scaled(512, 512, Qt::AspectRatioMode::KeepAspectRatio);
            auto* label = new QLabel{};
            label->setMaximumWidth(512);
            label->setMaximumHeight(512);
            hbox->addWidget(label);
            label->setPixmap(imgPixmap);  // share pixmap with label (uses refcounting?)
        } else {
            auto* label = new QLabel{"Error loading image"};
            hbox->addWidget(label);
        }

        // right-hand side: HSV viewer/editor
        auto* hsvViewer = new HSVViewer{};
        hbox->addWidget(hsvViewer);
    }

    // configure status bar
    if (false) {  // TODO: not required for MVP
        QStatusBar* statusBar = this->statusBar();
        statusBar->addWidget(new QLabel{"hello"});
    }
}

pc::MainWindow::~MainWindow() noexcept = default;
