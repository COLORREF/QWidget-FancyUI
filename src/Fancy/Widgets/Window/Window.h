//
// Created by TX on 2025/9/21.
//

#ifndef QWIDGET_FANCYUI_WINDOW_H
#define QWIDGET_FANCYUI_WINDOW_H

#include <QWidget>

class QVariantAnimation;
class QHBoxLayout;
class QVBoxLayout;

namespace fancy
{
    enum class DwmWindowEffect;
    enum class Theme;
    enum class CompositionWindowEffect;
    class TitleBar;

    class Window : public QWidget
    {
        Q_OBJECT

        friend class MSWindow;
        friend class StackWindow;

    public:
        explicit Window(QWidget *parent = nullptr);

        void setAcrylicEffect();

        void setMicaEffect();

        void setMicaAltEffect();

        void setAreoEffect();

        void setAreoEffect(const QColor &color);

        void setFullyTransparentEffect();

        void setSystemColorFillEffect();

        void setTransparentColorFillEffect(const QColor &color);

        void setSolidColorFillEffect(const QColor &color);

        void disabledWindowEffect();

        void setDoubleClickMaximize(bool allow);

        void removeCloseButton();

        void removeMaximizeButton();

        void removeMinimizeButton();

        void removeIconLabel();

        void removeTextLabel();

        void moveToCenter();

        QWidget *setupUiWidget();

        QWidget *titleBar();

    protected:
        void paintEvent(QPaintEvent *event) override;

        bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;

        void changeEvent(QEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

    private:
        int _xBorderWidth{};
        int _yBorderWidth{};
        bool _useWindowEffects;
        bool _useCompositionWindowEffect;
        DwmWindowEffect _dwmWindowEffect;
        QColor _bkgColor;
        TitleBar *_titleBar; // 标题栏
        QWidget *_clientArea; // 客户区
        QVBoxLayout *_verticalLayout;
        QVariantAnimation *_animation;

        void setBorderWidth();

        void setDwmWindowEffect(DwmWindowEffect effect);

        void setCompositionWindowEffect(CompositionWindowEffect effect, const QColor &color = {});

    private slots:
        void onThemeChanged();

        void updateBkgColor(const QVariant &value);

        void setDwmMargins() const;
    };
} // fancy

#endif //QWIDGET_FANCYUI_WINDOW_H
