//
// Created by TX on 2025/9/21.
//

#ifndef QWIDGET_FANCYUI_WINDOW_H
#define QWIDGET_FANCYUI_WINDOW_H

#include <QWidget>
#include <array>

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

        /**
         * @brief 启用系统 Acrylic 材料，需要 Windows 11 Build 22621 或更新版本
         * @note 特效设置失败时保留此前状态并输出诊断；框架回退失败时另行输出诊断
         */
        void setAcrylicEffect();

        void setMicaEffect();

        void setMicaAltEffect();

        void setAreoEffect();

        void setAreoEffect(const QColor &color);

        void setFullyTransparentEffect();

        void setSystemColorFillEffect();

        void setTransparentColorFillEffect(const QColor &color);

        void setSolidColorFillEffect(const QColor &color);

        /**
         * @brief 关闭当前特效并恢复普通背景绘制；系统调用失败时保留已启用状态。
         */
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
#else
        bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
#endif


        void changeEvent(QEvent *event) override;

        void resizeEvent(QResizeEvent *event) override;

    private:
        int _xBorderWidth{};
        int _yBorderWidth{};
        bool _useWindowEffects;
        bool _useCompositionWindowEffect;
        bool _dwmPreviousNoSystemBackground{};
        DwmWindowEffect _dwmWindowEffect;
        QColor _bkgColor;
        TitleBar *_titleBar; // 标题栏
        QWidget *_clientArea; // 客户区
        QVBoxLayout *_verticalLayout;
        QVariantAnimation *_animation;
        std::array<int, 4> _frameMargins{}; //最近成功应用的框架边距（左、右、上、下），用于失败回退

        /**
         * @brief 应用框架扩展；成功后才记录边距，失败输出诊断
         */
        bool applyFrameMargins(const std::array<int, 4> &margins);

        void setBorderWidth();

        void updateMaximizedMargins();

        void setDwmWindowEffect(DwmWindowEffect effect);

        void setCompositionWindowEffect(CompositionWindowEffect effect, const QColor &color = {});

    private slots:
        void onThemeChanged();

        void updateBkgColor(const QVariant &value);

        void setDwmMargins();
    };
} // fancy

#endif //QWIDGET_FANCYUI_WINDOW_H
