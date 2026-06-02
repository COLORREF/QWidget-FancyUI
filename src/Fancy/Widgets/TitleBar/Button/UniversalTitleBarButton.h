//
// Created by TX on 2025/10/6.
//

#ifndef QWIDGET_FANCYUI_UNIVERSALTITLEBARBUTTON_H
#define QWIDGET_FANCYUI_UNIVERSALTITLEBARBUTTON_H
#include <QPushButton>


namespace fancy
{
    struct IconId;
}

namespace fancy
{
    enum class TitleBtnState;
    class SvgWidget;

    class UniversalTitleBarButton : public QPushButton
    {
        Q_OBJECT

        friend class TitleBar;
        friend class Window;

    public:
        explicit UniversalTitleBarButton(QWidget *parent);

        void setRadius(int radius) { _radius = radius; }
        [[nodiscard]] int radius() const { return _radius; }
        [[nodiscard]] TitleBtnState titleBtnState() const { return _state; }
        void setTitleBtnState(TitleBtnState state) { _state = state; }
        void setClearBeforeNewPaint(bool before) { _clearBeforeNewPaint = before; }
        [[nodiscard]] bool clearBeforeNewPaint() const { return _clearBeforeNewPaint; }
        [[nodiscard]] SvgWidget *svgWidget() const { return _svgWidget; }
        void setAutoEnlargeIcon(bool enlarge) { _AutoEnlargeIcon = enlarge; }
        [[nodiscard]] bool autoEnlargeIcon() const { return _AutoEnlargeIcon; }

        void setIcon(const IconId &id);

    private:
        TitleBtnState _state;
        int _radius;
        bool _clearBeforeNewPaint;
        bool _AutoEnlargeIcon;
        SvgWidget *_svgWidget;

    protected:
        [[nodiscard]] virtual QBrush maskBrush() const;

        void enterEvent(QEnterEvent *event) override;

        void leaveEvent(QEvent *event) override;

        void mouseReleaseEvent(QMouseEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

        void paintEvent(QPaintEvent *event) override;
    };
} // fancy

#endif //QWIDGET_FANCYUI_UNIVERSALTITLEBARBUTTON_H
