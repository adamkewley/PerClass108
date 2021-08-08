#include "imageviewer.h"

#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>

namespace {
    // tries to load an image file located at `path` into the provided `QLabel`
    void loadImageFileIntoLabel(QString const& path, QLabel& label) {
        QPixmap pxmap{path};

        if (pxmap.isNull()) {
            return;  // HACK: i'm too lazy to handle loading errors right now
        }

        QSize dims = label.size();

        // rescale pixelmap to fit widget bounds
        pxmap = pxmap.scaled(dims.width(), dims.height(), Qt::AspectRatioMode::KeepAspectRatio);

        label.setPixmap(pxmap);
    }
}

pc::ImageViewer::ImageViewer(QString const& initialImage, QWidget* parent) :
    QWidget{parent},
    label{new QLabel{"loading...", this}} {

    QVBoxLayout* vbox = new QVBoxLayout{this};

    vbox->addWidget(label);
    label->setMinimumSize(512, 512);

    loadImageFileIntoLabel(initialImage, *label);
}

void pc::ImageViewer::setImageFile(QString const& newPath) {
    loadImageFileIntoLabel(newPath, *label);
}
