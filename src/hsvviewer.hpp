#pragma once

#include <QWidget>
#include <QPixmap>

class QLabel;
class QSlider;
class QColor;

namespace pc
{
    // raw representation of HSV (hue, saturation, value)
    struct HSV final {
        float h, s, v;

        HSV(float h_, float s_, float v_) : h{h_}, s{s_}, v{v_} {}
        explicit HSV(QColor const&);
    };

    // circle containing HSV color space
    class HSViewerColorCircle final : public QWidget {
        Q_OBJECT

    public:
        explicit HSViewerColorCircle(QWidget* parent = nullptr);

    public slots:
        void setValue(float);
        void setColor(QColor const&);

    signals:
        // emitted whenever the mouse moves over a color in the HSV circle
        void mouseMoveOverColor(QColor const&);

    private:
        void mouseMoveEvent(QMouseEvent*) override;
        void paintEvent(QPaintEvent*) override;

        HSV hsv;
        QPixmap cachedCircle;
        float cachedCircleValue;
    };

    // slider for manipulating V
    class HSViewerValueSlider final : public QWidget {
        Q_OBJECT

    public:
        explicit HSViewerValueSlider(QWidget* parent = nullptr);

    public slots:
        // set value (brightness), clamped between [0.0f, 1.0f]
        void setValue(float);

    signals:
        // emitted when user changes value
        void valueChangedEvent(float);

    private slots:
        // used internally when user clicks the slider
        void sliderValueChanged(int);

    private:
        QSlider* slider;
    };


    // viewer that shows the HSV value in text
    class HSViewerDetails final : public QWidget {
        Q_OBJECT

    public:
        explicit HSViewerDetails(QWidget* parent = nullptr);

    public slots:
        // sets the details to a new HSV value
        void setColor(QColor const&);

    private:
        QLabel* hLabel;
        QLabel* sLabel;
        QLabel* vLabel;

        QLabel* rLabel;
        QLabel* gLabel;
        QLabel* bLabel;
    };


    // viewer that shows HSV color circle and details
    class HSVViewer final : public QWidget {
        Q_OBJECT

    public:
        explicit HSVViewer(QWidget* parent = nullptr);

    public slots:
        void setColor(QColor const&);

    private:
        HSViewerColorCircle* m_ColorCircle;
        HSViewerValueSlider* m_ValueSlider;
        HSViewerDetails* m_DetailsPrintout;
    };
}
