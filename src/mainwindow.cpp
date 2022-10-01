#include "mainwindow.hpp"

#include "hsvviewer.hpp"
#include "imageviewer.hpp"

#include <QtWidgets>

pc::MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    m_ImageViewer{new ImageViewer{":/images/smiley-face.jpg", this}}
{
    // init this window's state
    this->setObjectName("main_app_window");
    this->setGeometry(0, 0, 800, 0);
    this->move(this->screen()->geometry().center() - this->frameGeometry().center());
    this->setWindowTitle(QApplication::translate("appctx", "PerClass108"));
    this->setCentralWidget(new QWidget{});
    this->centralWidget()->setObjectName("main_app_window_central_widget");
    this->setAcceptDrops(true);

    // menu bar
    {
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
    {
        // lay them out horizontally
        QHBoxLayout* hbox = new QHBoxLayout{this->centralWidget()};
        hbox->setObjectName("main_area_hbox");

        // left: image file viewer
        hbox->addWidget(m_ImageViewer);

        // right: HSV viewer
        auto* hsvViewer = new HSVViewer{};
        hbox->addWidget(hsvViewer);

        connect(this->m_ImageViewer, &ImageViewer::mouseMoveOverColor, hsvViewer, &HSVViewer::setColor);
    }

    // status bar
    {
        QStatusBar* statusBar = this->statusBar();
        statusBar->addWidget(new QLabel{"hello"});
    }
}

pc::MainWindow::~MainWindow() noexcept = default;

void pc::MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
    e->acceptProposedAction();
}

void pc::MainWindow::dragMoveEvent(QDragMoveEvent* e)
{
    e->acceptProposedAction();
}

void pc::MainWindow::dropEvent(QDropEvent* e)
{
    // see: https://wiki.qt.io/Drag_and_Drop_of_files

    QMimeData const& mimeData = *e->mimeData();

    if (!mimeData.hasUrls())
    {
        return;  // we're looking for file:// URLs, specifically
    }

    QList<QUrl> urlList = mimeData.urls();

    if (urlList.empty())
    {
        return; // just in case
    }

    // HACK: only accept the last file because this UI doesn't support
    // tabbed viewing
    m_ImageViewer->setImageFile(urlList.back().toLocalFile());

    e->acceptProposedAction();
}

void pc::MainWindow::promptUserForImage(bool)
{
    QString selected = QFileDialog::getOpenFileName(
        this,
        tr("Open Image File")
    );

    if (selected.isNull())
    {
        return;  // user cancelled out
    }

    m_ImageViewer->setImageFile(selected);
}

void pc::MainWindow::userRequestedExit()
{
    QApplication::quit();
}
