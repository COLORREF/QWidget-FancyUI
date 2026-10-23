//
// Created by TX on 2025/9/21.
//

#include "Window.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QVariantAnimation>
#include <QWindow>
#include <QWindowStateChangeEvent>
#include <QScreen>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QtMath>

#include "Core/Defs.h"
#include "Core/SystemAccessor.h"
#include "Core/WindowController.h"
#include "Core/Palette/Palette.h"
#include "Core/Theme/ThemeModeController.h"
#include "Widgets/TitleBar/TitleBar.h"
#include "Widgets/TitleBar/Button/MaximizeButton.h"
#include "Widgets/TitleBar/Button/MinimizeButton.h"

#include "windowsx.h"

namespace fancy
{
    Window::Window(QWidget *parent) :
        QWidget(parent),
        _useWindowEffects(false),
        _useCompositionWindowEffect(false),
        _dwmWindowEffect(DwmWindowEffect::Last),
        _bkgColor(Palette::palette().color(ColorRole::Window)),
        _titleBar(new TitleBar(this)),
        _clientArea(new QWidget(this)),
        _verticalLayout(new QVBoxLayout(this)),
        _animation(new QVariantAnimation(this))
    {
        _animation->setStartValue(Palette::palette().color(ColorRole::Window, Theme::Light));
        _animation->setEndValue(Palette::palette().color(ColorRole::Window, Theme::Dark));
        _animation->setDuration(300);

        _verticalLayout->setSpacing(0);
        _verticalLayout->setContentsMargins(0, 0, 0, 0);
        _verticalLayout->addWidget(_titleBar);
        _verticalLayout->addWidget(_clientArea);
        setWindowFlags(windowFlags() | Qt::WindowType::FramelessWindowHint);

#ifdef Q_OS_WIN
        const HWND hwnd = reinterpret_cast<HWND>(winId());
        applyFrameMargins({1, 1, 0, 1});
        SetLastError(ERROR_SUCCESS);
        const auto previousStyle = SetWindowLongPtr(hwnd,
                         GWL_STYLE,
                         GetWindowLongPtr(
                             hwnd,
                             GWL_STYLE
                         ) | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU
        );
        if (previousStyle == 0 && GetLastError() != ERROR_SUCCESS)
            qWarning() << "Failed to configure native window style";
#endif
        setBorderWidth();

        connect(windowHandle(), &QWindow::screenChanged, this, &Window::setDwmMargins);
        connect(windowHandle(), &QWindow::screenChanged, this, [this] {
            QTimer::singleShot(0, this, &Window::setBorderWidth);
        });
        connect(_animation, &QVariantAnimation::valueChanged, this, &Window::updateBkgColor);
        connect(&Palette::palette(), &Palette::appThemeChange, this, &Window::onThemeChanged);

        resize(640, 480);
    }

    void Window::setAcrylicEffect()
    {
        setDwmWindowEffect(DwmWindowEffect::Acrylic);
    }

    void Window::setMicaEffect()
    {
        setDwmWindowEffect(DwmWindowEffect::Mica);
    }

    void Window::setMicaAltEffect()
    {
        setDwmWindowEffect(DwmWindowEffect::MicaAlt);
    }

    void Window::setAreoEffect()
    {
        setCompositionWindowEffect(CompositionWindowEffect::Areo);
    }

    void Window::setAreoEffect(const QColor &color)
    {
        setCompositionWindowEffect(CompositionWindowEffect::AreoWithColor, color);
    }

    void Window::setFullyTransparentEffect()
    {
        setCompositionWindowEffect(CompositionWindowEffect::FullyTransparent);
    }

    void Window::setSystemColorFillEffect()
    {
        setCompositionWindowEffect(CompositionWindowEffect::SystemColor);
    }

    void Window::setTransparentColorFillEffect(const QColor &color)
    {
        setCompositionWindowEffect(CompositionWindowEffect::TransparentColor, color);
    }

    void Window::setSolidColorFillEffect(const QColor &color)
    {
        setCompositionWindowEffect(CompositionWindowEffect::SolidColor, color);
    }

    void Window::disabledWindowEffect()
    {
        if (!_useWindowEffects)
            return;
        const auto hwnd = reinterpret_cast<HWND>(winId());
        const bool disabled = _useCompositionWindowEffect
            ? WindowController::setCompositionWindowEffect(hwnd, CompositionWindowEffect::Disabled)
            : WindowController::setDwmWindowEffect(hwnd, DwmWindowEffect::None);
        if (!disabled)
        {
            qWarning() << "Failed to disable window effect; retaining the active effect state";
            return;
        }
        if (!_useCompositionWindowEffect)
        {
            _dwmWindowEffect = DwmWindowEffect::None;
            setAttribute(Qt::WA_NoSystemBackground, _dwmPreviousNoSystemBackground);
        }
        _useCompositionWindowEffect = false;
        _useWindowEffects = false;
        _titleBar->setTransparency(false);
        applyFrameMargins({1, 1, 0, 1});
        _animation->stop();
        _bkgColor = Palette::palette().color(ColorRole::Window);
        update();
    }

    void Window::setDoubleClickMaximize(bool allow)
    {
        _titleBar->setDoubleClickMaximize(allow);
    }

    void Window::removeCloseButton()
    {
        _titleBar->removeCloseButton();
    }

    void Window::removeMaximizeButton()
    {
        _titleBar->removeMaximizeButton();
    }

    void Window::removeMinimizeButton()
    {
        _titleBar->removeMinimizeButton();
    }

    void Window::removeIconLabel()
    {
        _titleBar->removeIconLabel();
    }

    void Window::removeTextLabel()
    {
        _titleBar->removeTextLabel();
    }

    void Window::moveToCenter()
    {
        if (isMaximized() || isFullScreen())
            return;
        QRect targetGeometry;
        if (const auto currentScreen = screen())
            targetGeometry = currentScreen->availableGeometry();
        else
        {
            if (const auto targetScreen = qApp->screenAt(geometry().center()))
                targetGeometry = targetScreen->availableGeometry();
            else
                targetGeometry = qApp->primaryScreen()->availableGeometry();
        }
        move(targetGeometry.left() + (targetGeometry.width() - width()) / 2, targetGeometry.top() + (targetGeometry.height() - height()) / 2);
    }

    QWidget *Window::setupUiWidget()
    {
        return _clientArea;
    }

    QWidget *Window::titleBar()
    {
        return _titleBar;
    }

    void Window::paintEvent(QPaintEvent *event)
    {
        if (_useWindowEffects)
        {
            QPainter painter(this);
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
            painter.eraseRect(this->rect());
            return QWidget::paintEvent(event);
        }
        QPainter painter(this);
        painter.setPen(Qt::PenStyle::NoPen);
        painter.setBrush(_bkgColor);
        painter.drawRect(rect());
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    bool Window::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
    bool Window::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
    {
        switch (const MSG *msg = static_cast<MSG *>(message); msg->message)
        {
            case WM_NCACTIVATE :
                if (_useWindowEffects && !_useCompositionWindowEffect)
                {
                    // Qt 6.11 的无边框路径会吞掉此消息，使 DWM 材料停留在失活外观。
                    // 更新原生激活状态，但不让 DefWindowProc 重绘系统标题栏。
                    *result = DefWindowProc(msg->hwnd, msg->message, msg->wParam, -1);
                    return true;
                }
                break;
            case WM_NCCALCSIZE : // 取消原有的标题栏和边框
                // *result = msg->wParam ? 0 : WVR_REDRAW;
                *result = 0;
                return true;
            case WM_NCHITTEST : // 光标位置测试
            {
                POINT p{GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)}; // 获取鼠标的绝对坐标
                if (!isMaximized())
                {
                    RECT widget_rect{0, 0, 0, 0};
                    if (!GetWindowRect(msg->hwnd, &widget_rect))
                        break;
                    if (p.x < widget_rect.left || p.x >= widget_rect.right ||
                        p.y < widget_rect.top || p.y >= widget_rect.bottom)
                        break;

                    // 综合考虑使用习惯以及其他一些写法（8个if直接判断、九宫格位视图法）
                    // 下面的代码可能有些冗余，但能有效减少if判断的次平均次数以及减少临时变量的创建
                    if (p.x < widget_rect.right && p.x >= widget_rect.right - _xBorderWidth) // 右
                    {
                        if (p.y < widget_rect.bottom && p.y >= widget_rect.bottom - _yBorderWidth) // 下
                            *result = HTBOTTOMRIGHT;
                        else if (p.y < widget_rect.top + _yBorderWidth) // 上
                            *result = HTTOPRIGHT;
                        else
                            *result = HTRIGHT;
                        return true;
                    }
                    if (p.y < widget_rect.bottom && p.y >= widget_rect.bottom - _yBorderWidth) // 下
                    {
                        if (p.x < widget_rect.left + _xBorderWidth) // 左
                            *result = HTBOTTOMLEFT;
                        else
                            *result = HTBOTTOM;
                        return true;
                    }
                    if (p.y < widget_rect.top + _yBorderWidth) // 上
                    {
                        if (p.x < widget_rect.left + _xBorderWidth) // 左
                            *result = HTTOPLEFT;
                        else
                            *result = HTTOP;
                        return true;
                    }
                    if (p.x < widget_rect.left + _xBorderWidth) // 左
                    {
                        *result = HTLEFT;
                        return true;
                    }
                }

                if (_titleBar->_maximizeButton && !_titleBar->_maximizeButton->isHidden())
                {
                    // 返回零也可能表示成功且坐标位移为零，需结合错误码判断。
                    POINT clientPoint = p;
                    SetLastError(ERROR_SUCCESS);
                    if (MapWindowPoints(HWND_DESKTOP, msg->hwnd, &clientPoint, 1) == 0 && GetLastError() != ERROR_SUCCESS)
                    {
                        clientPoint = p;
                        if (!ScreenToClient(msg->hwnd, &clientPoint))
                            break;
                    }
                    QPoint widget_point(clientPoint.x, clientPoint.y); // Win32 客户区物理像素
                    widget_point /= devicePixelRatioF(); // 除以缩放dpi

                    QRect max_btn_rect(_titleBar->_maximizeButton->mapTo(this, QPoint{0, 0}), _titleBar->_maximizeButton->size()); // 最大化按钮区域
                    if (max_btn_rect.contains(widget_point)) // 鼠标在最大化按钮内
                    {
                        *result = HTMAXBUTTON;
                        if (_titleBar->_maximizeButton->_state == TitleBtnState::Pressed || _titleBar->_maximizeButton->_state == TitleBtnState::PressedOut) // 鼠标按下
                            emit _titleBar->_maximizeButton->mouseMove(true);
                        else
                            emit _titleBar->_maximizeButton->enter();
                        return true;
                    }
                }
            }
            break;
            case WM_NCMOUSELEAVE : // 光标从非工作区离开
                if (_titleBar->_maximizeButton && !_titleBar->_maximizeButton->isHidden())
                {
                    if (_titleBar->_maximizeButton->_state == TitleBtnState::Pressed || _titleBar->_maximizeButton->_state == TitleBtnState::PressedOut)
                        emit _titleBar->_maximizeButton->mouseMove(false);
                    else
                        emit _titleBar->_maximizeButton->leave();
                }
                break;
            case WM_NCLBUTTONDBLCLK : [[fallthrough]]; // 鼠标左键位于窗口非工作区双击时发出
            case WM_NCLBUTTONUP : // 鼠标左键非客户区松开,相当于点击最大化按钮
                if (_titleBar->_maximizeButton && !_titleBar->_maximizeButton->isHidden())
                {
                    if (msg->wParam == HTMAXBUTTON)
                    {
                        emit _titleBar->_maximizeButton->leave();
                        if (isMaximized())
                            // showNormal();//Qt 6.8会产生窗口动画消失、窗口大小无法复原等bug
                            ::SendMessage(reinterpret_cast<HWND>(window()->winId()), WM_SYSCOMMAND, SC_RESTORE, 0); //使用Windows API可以避免这些bug
                        else
                            // showMaximized();
                            ::SendMessage(reinterpret_cast<HWND>(window()->winId()), WM_SYSCOMMAND, SC_MAXIMIZE, 0);
                        *result = 0;
                        return true;
                    }
                }
                break;
            case WM_LBUTTONUP : // 客户区鼠标左键松开
                if (_titleBar->_maximizeButton && !_titleBar->_maximizeButton->isHidden())
                {
                    if (_titleBar->_maximizeButton->_state == TitleBtnState::Pressed || _titleBar->_maximizeButton->_state == TitleBtnState::PressedOut)
                        emit _titleBar->_maximizeButton->leave();
                }
                break;
            case WM_NCLBUTTONDOWN : // 鼠标左键位于窗口非工作区按下时发出
                if (_titleBar->_maximizeButton && !_titleBar->_maximizeButton->isHidden())
                {
                    if (msg->wParam == HTMAXBUTTON)
                    {
                        *result = 0;
                        emit _titleBar->_maximizeButton->mousePress();
                        return true;
                    }
                }
                break;
            case WM_DPICHANGED : // Qt 处理完新的窗口 DPI 后再刷新边框及布局单位
            case WM_DISPLAYCHANGE :
                QTimer::singleShot(0, this, [this] {
                    setBorderWidth();
                    setDwmMargins();
                    update();
                });
                break;
            // case WM_WININICHANGE :
            // *result = 0;
            // const char* str = (const char*)(msg->lParam);//ImmersiveColorSet
            // return true;
            default :
                break;
        }
        return QWidget::nativeEvent(eventType, message, result);
    }

    void Window::changeEvent(QEvent *event)
    {
        if (event->type() == QEvent::Type::WindowStateChange)
        {
            if (const auto *stateChangeEvent = dynamic_cast<QWindowStateChangeEvent *>(event))
            {
                // 最大化后最小化
                if (stateChangeEvent->oldState() & Qt::WindowState::WindowMaximized && windowState() & Qt::WindowState::WindowMinimized)
                    return QWidget::changeEvent(event);
                // 最大化
                if (windowState() & Qt::WindowState::WindowMaximized)
                {
                    if (_titleBar->_maximizeButton)
                        emit _titleBar->_maximizeButton->stateChange(MaximizeBtnState::Maximize);
                    updateMaximizedMargins();
                    update();
                }
                // 最大化后还原
                if (stateChangeEvent->oldState() & Qt::WindowState::WindowMaximized && !(windowState() & Qt::WindowState::WindowMaximized))
                {
                    if (_titleBar->_maximizeButton)
                        emit _titleBar->_maximizeButton->stateChange(MaximizeBtnState::Normal);
                    _verticalLayout->setContentsMargins(0, 0, 0, 0);
                    update();
                }
                // 最小化后还原
                if (stateChangeEvent->oldState() & Qt::WindowState::WindowMinimized)
                {
                    setFocus();
                    update();
                }
            }
        }
        QWidget::changeEvent(event);
    }

    void Window::resizeEvent(QResizeEvent *event)
    {
        setDwmMargins();
        QWidget::resizeEvent(event);
    }

    void Window::onThemeChanged()
    {
        const Theme theme = ThemeModeController::controller().appTheme();
        if (!WindowController::setWindowThemeMode(reinterpret_cast<HWND>(winId()), theme))
            qWarning() << "Failed to apply native window theme";

        if (!_useWindowEffects)
        {
            if (theme == Theme::Light)
                _animation->setDirection(QAbstractAnimation::Direction::Backward);
            else
                _animation->setDirection(QAbstractAnimation::Direction::Forward);
            _animation->start();
        }
    }

    void Window::updateBkgColor(const QVariant &value)
    {
        _bkgColor = value.value<QColor>();
        update();
    }

    bool Window::applyFrameMargins(const std::array<int, 4> &margins)
    {
        const MARGINS nativeMargins{margins[0], margins[1], margins[2], margins[3]};
        if (!WindowController::extendFrameIntoClientArea(reinterpret_cast<HWND>(winId()), nativeMargins))
        {
            qWarning() << "Failed to extend window frame into client area";
            return false;
        }
        _frameMargins = margins;
        return true;
    }

    void Window::setDwmMargins()
    {
        switch (_dwmWindowEffect)
        {
            case DwmWindowEffect::Acrylic : // 亚克力
            case DwmWindowEffect::Mica : // 云母
            case DwmWindowEffect::MicaAlt : // 云母ALT
                applyFrameMargins({static_cast<int>(width() * devicePixelRatioF()) + 1, 0, 0, 0});
            default :
                break;
        }
    }

    void Window::setBorderWidth()
    {
#ifdef Q_OS_WIN
        const QSize borderSize = SystemAccessor::windowResizeBorderSize();
        _xBorderWidth = borderSize.width();
        _yBorderWidth = borderSize.height();
#endif
        updateMaximizedMargins();
    }

    void Window::updateMaximizedMargins()
    {
        if (!isMaximized())
            return;
        // Win32 命中测试使用原始像素值；Qt 布局边距转换为窗口的逻辑像素
        const qreal dpr = devicePixelRatioF();
        _verticalLayout->setContentsMargins(
            qRound(_xBorderWidth / dpr), qRound(_yBorderWidth / dpr),
            qRound(_xBorderWidth / dpr), qRound(_yBorderWidth / dpr));
    }

    void Window::setDwmWindowEffect(DwmWindowEffect effect)
    {
        // DWMWA_SYSTEMBACKDROP_TYPE 的正式支持下限为 Windows 11 22H2
        if (!SystemAccessor::isWindows11() || SystemAccessor::windowsVersion().BuildNumber < 22621)
        {
            qWarning() << "The DWM backdrop requires Windows 11 build 22621 or later;";
            qWarning() << "Including acrylic effect, mica effect, mica Alt effect;";
            qWarning() << "The window has been initialized as the default window and will not use any DWM effects;";
            return;
        }
        if (_useCompositionWindowEffect)
        {
            qWarning() << "The DWM effect cannot be enabled when the Composition window effect has been enabled;";
            qWarning() << "Must restart the application to switch window effects";
            return;
        }
        const auto oldFlags = windowFlags();
        const bool hadNoSystemBackground = testAttribute(Qt::WA_NoSystemBackground);
        const auto oldMargins = _frameMargins;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        setWindowFlags((this->windowFlags()) & (~Qt::WindowType::FramelessWindowHint));
#endif
        // 只接管背景绘制；保留原生窗口格式，由 paintEvent 清出材料显示区域
        setAttribute(Qt::WA_NoSystemBackground);
        // Qt 属性或标志改变后再获取句柄，避免使用已被重建的 HWND
        const auto hWnd = reinterpret_cast<HWND>(winId());
        // 与 resizeEvent 使用同一扩展方式，避免负边距带出系统标题栏按钮。
        const bool frameReady = applyFrameMargins({static_cast<int>(width() * devicePixelRatioF()) + 1, 0, 0, 0});
        if (!frameReady || !WindowController::setDwmWindowEffect(hWnd, effect))
        {
            qWarning() << "Failed to enable DWM backdrop; retaining the previous effect state";
            if (windowFlags() != oldFlags)
                setWindowFlags(oldFlags);
            setAttribute(Qt::WA_NoSystemBackground, hadNoSystemBackground);
            if (frameReady)
                applyFrameMargins(oldMargins);
            update();
            return;
        }
        // 切换 DWM 材料时保留首次开启前的属性，关闭时恢复
        if (!_useWindowEffects)
            _dwmPreviousNoSystemBackground = hadNoSystemBackground;
        _useWindowEffects = true;
        _dwmWindowEffect = effect;
        _titleBar->setTransparency();
        // 主题不是材料启用的必要条件，失败仅报告
        if (!WindowController::setWindowThemeMode(hWnd, ThemeModeController::controller().appTheme()))
            qWarning() << "Backdrop enabled, but native window theme could not be applied";
        // 动态启用时，先前的 WM_NCACTIVATE 可能已被 Qt 消费，不会自动重发。
        DefWindowProc(hWnd, WM_NCACTIVATE, GetForegroundWindow() == hWnd, -1);
        update();
    }

    void Window::setCompositionWindowEffect(CompositionWindowEffect effect, const QColor &color)
    {
        if (SystemAccessor::isBelowWindows10())
        {
            qWarning() << "Operating systems younger than Windows10 May not be supported or may have unknown errors";
            qWarning() << "The window has been initialized as the default window and will not use any DWM effects;";
            return;
        }
        if (_dwmWindowEffect != DwmWindowEffect::Last)
        {
            qWarning() << "The Composition effect cannot be enabled when the DWM window effect has been enabled or set;";
            qWarning() << "Must restart the application to switch window effects";
            return;
        }
        const auto oldMargins = _frameMargins;
        if (!applyFrameMargins({0, 0, 1, 0}))
            return;
        const auto hWnd = reinterpret_cast<HWND>(winId());
        if (!WindowController::setCompositionWindowEffect(hWnd, effect, ABGR(color.alpha(), color.red(), color.blue(), color.green())))
        {
            qWarning() << "Failed to enable composition effect; retaining the previous effect state";
            applyFrameMargins(oldMargins);
            return;
        }
        _useWindowEffects = true;
        _useCompositionWindowEffect = true;
        _titleBar->setTransparency();
        update();
    }
} // fancy
