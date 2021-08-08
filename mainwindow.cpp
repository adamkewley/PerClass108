#include "mainwindow.h"

#include "hsvviewer.h"
#include "imageviewer.h"

#include <iostream>
#include <QtWidgets>  // TODO: use specific headers

pc::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    imgViewer{new ImageViewer{"C:\\Users\\adamk\\OneDrive\\Desktop\\PerClass108\\smiley-face.jpg", this}} {

    // init this window's state
    this->setObjectName("main_app_window");
    this->setGeometry(0, 0, 800, 0);
    this->move(this->screen()->geometry().center() - this->frameGeometry().center());
    this->setWindowTitle(QApplication::translate("appctx", "PerClass108"));
    this->setCentralWidget(new QWidget{});
    this->centralWidget()->setObjectName("main_app_window_central_widget");
    this->setAcceptDrops(true);

    // menu bar
    if (true) {  // TODO: not required for MVP
        auto* menuBar = this->menuBar();
        menuBar->setObjectName("menubar");
        QMenu* fileMenu = menuBar->addMenu("File");

        auto* openAction = new QAction{"open_action", this};
        openAction->setText("Open");
        connect(openAction, &QAction::triggered, this, &MainWindow::promptUserForImage);

        auto* quitAction = new QAction{"quit_action", this};
        quitAction->setText("Quit");
        connect(quitAction, &QAction::triggered, this, &MainWindow::userRequestedExit);

        fileMenu->addAction(openAction);
        fileMenu->addAction(quitAction);
    }

    // main area: image (left) and HSV widget (right)
    if (true) {

        // lay them out horizontally
        QHBoxLayout* hbox = new QHBoxLayout{this->centralWidget()};
        hbox->setObjectName("main_area_hbox");

        // left: image file viewer
        imgViewer = new ImageViewer{"C:\\Users\\adamk\\OneDrive\\Desktop\\PerClass108\\smiley-face.jpg", this};
        hbox->addWidget(imgViewer);

        // right: HSV viewer
        auto* hsvViewer = new HSVViewer{};
        hbox->addWidget(hsvViewer);
    }

    // status bar
    if (false) {  // TODO: not required for MVP
        QStatusBar* statusBar = this->statusBar();
        statusBar->addWidget(new QLabel{"hello"});
    }
}

pc::MainWindow::~MainWindow() noexcept = default;

void pc::MainWindow::dragEnterEvent(QDragEnterEvent* e) {
    e->acceptProposedAction();
}

void pc::MainWindow::dragMoveEvent(QDragMoveEvent* e) {
    e->acceptProposedAction();
}

void pc::MainWindow::dropEvent(QDropEvent* e) {
    // see: https://wiki.qt.io/Drag_and_Drop_of_files

    QMimeData const& mimeData = *e->mimeData();

    if (!mimeData.hasUrls()) {
        return;  // we're looking for file:// URLs, specifically
    }

    QList<QUrl> urlList = mimeData.urls();

    if (urlList.empty()) {
        return; // just in case
    }

    // HACK: only accept the last file because this UI doesn't support
    // tabbed viewing
    imgViewer->setImageFile(urlList.back().toLocalFile());

    e->acceptProposedAction();
}

void pc::MainWindow::promptUserForImage(bool) {
    QString selected = QFileDialog::getOpenFileName(
                this,
                tr("Open Image File"));

    if (selected.isNull()) {
        return;  // user cancelled out
    }

    imgViewer->setImageFile(selected);
}

void pc::MainWindow::userRequestedExit() {
    QApplication::quit();
}
