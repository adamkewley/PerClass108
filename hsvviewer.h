#pragma once

#include <QWidget>

class QLabel;

namespace pc {
    // raw representation of HSV (hue, saturation, value)
    struct HSV final {
        float h, s, v;
    };

    // circle containing HSV color space
    class HSViewerColorCircle final : public QWidget {
        Q_OBJECT

    public:
        explicit HSViewerColorCircle(QWidget* parent = nullptr);

        void mouseMoveEvent(QMouseEvent*) override;
        void paintEvent(QPaintEvent*) override;

    signals:
        // emitted whenever the mouse moves over a value in the HSV circle
        void mouseMoveOverHSV(HSV);

    private:
        float v;
    };

    // viewer that shows the HSV value in text
    class HSViewerDetails final : public QWidget {
        Q_OBJECT

    public:
        explicit HSViewerDetails(QWidget* parent = nullptr);

    public slots:
        // sets the details to a new HSV value
        void setHSVValue(HSV);

    private:
        QLabel* hLabel;
        QLabel* sLabel;
        QLabel* vLabel;
    };

    // viewer that shows HSV color circle and details
    class HSVViewer final : public QWidget {
        Q_OBJECT

    public:
        explicit HSVViewer(QWidget* parent = nullptr);
    };
}
