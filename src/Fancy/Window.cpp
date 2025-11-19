//
// Created by TX on 2025/9/21.
//

#include "Window.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QPainter>
#include <QVariantAnimation>
#include <QWindow>
#include <QWindowStateChangeEvent>

#include "Defs.h"
#include "MaximizeButton.h"
#include "MinimizeButton.h"
#include "Palette.h"
#include "SystemAccessor.h"
#include "ThemeModeController.h"
#include "TitleBar.h"
#include "WindowController.h"

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
        setBorderWidth();

        setWindowFlags(windowFlags() | Qt::WindowType::FramelessWindowHint);

#ifdef Q_OS_WIN
        MARGINS margins = {1, 1, 0, 1};
        SystemAccessor::DwmExtendFrameIntoClientArea(reinterpret_cast<HWND>(winId()), &margins);
        SetWindowLongPtr(reinterpret_cast<HWND>(winId()),
                         GWL_STYLE,
                         GetWindowLongPtr(
                             reinterpret_cast<HWND>(winId()),
                             GWL_STYLE
                         ) | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU
        );
#endif

        connect(windowHandle(), &QWindow::screenChanged, this, &Window::setDwmMargins);
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
        if (_useWindowEffects)
        {
            _useWindowEffects = false;
            _titleBar->setTransparency(false);
            if (_useCompositionWindowEffect)
            {
                _useCompositionWindowEffect = false;
                WindowController::setCompositionWindowEffect(reinterpret_cast<HWND>(winId()), CompositionWindowEffect::Disabled);
            }
            if (_dwmWindowEffect != DwmWindowEffect::Last)
            {
                _dwmWindowEffect = DwmWindowEffect::None;
                WindowController::setDwmWindowEffect(reinterpret_cast<HWND>(winId()), DwmWindowEffect::None);
            }
        }
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

    bool Window::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
    {
        switch (const MSG *msg = static_cast<MSG *>(message); msg->message)
        {
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
                    GetWindowRect(msg->hwnd, &widget_rect); // 获取窗口的绝对区域

                    // 综合考虑使用习惯以及其他一些写法（8个if直接判断、九宫格位视图法）
                    // 下面的代码可能有些冗余，但能有效减少if判断的次平均次数以及减少临时变量的创建
                    if (p.x <= widget_rect.right && p.x >= widget_rect.right - _xBorderWidth) // 右
                    {
                        if (p.y <= widget_rect.bottom && p.y >= widget_rect.bottom - _yBorderWidth) // 下
                            *result = HTBOTTOMRIGHT;
                        else if (p.y >= widget_rect.top && p.y <= widget_rect.top + _yBorderWidth) // 上
                            *result = HTTOPRIGHT;
                        else
                            *result = HTRIGHT;
                        return true;
                    }
                    if (p.y <= widget_rect.bottom && p.y >= widget_rect.bottom - _yBorderWidth) // 下
                    {
                        if (p.x >= widget_rect.left && p.x <= widget_rect.left + _xBorderWidth) // 左
                            *result = HTBOTTOMLEFT;
                        else
                            *result = HTBOTTOM;
                        return true;
                    }
                    if (p.y >= widget_rect.top && p.y <= widget_rect.top + _yBorderWidth) // 上
                    {
                        if (p.x >= widget_rect.left && p.x <= widget_rect.left + _xBorderWidth) // 左
                            *result = HTTOPLEFT;
                        else
                            *result = HTTOP;
                        return true;
                    }
                    if (p.x >= widget_rect.left && p.x <= widget_rect.left + _xBorderWidth) // 左
                    {
                        *result = HTLEFT;
                        return true;
                    }
                }

                if (_titleBar->_maximizeButton && !_titleBar->_minimizeButton->isHidden())
                {
                    if (!MapWindowPoints(HWND_DESKTOP, msg->hwnd, &p, 1)) // 将p转换成相对坐标
                    {
                        if (!ScreenToClient(reinterpret_cast<HWND>(winId()), &p))
                        {
                            QPoint t(mapFromGlobal(QPoint(p.x, p.y)));
                            p.x = t.x();
                            p.y = t.y();
                        }
                    }
                    QPoint widget_point(p.x, p.y); // Qt相对坐标
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
                if (_titleBar->_maximizeButton && !_titleBar->_minimizeButton->isHidden())
                {
                    if (_titleBar->_maximizeButton->_state == TitleBtnState::Pressed || _titleBar->_maximizeButton->_state == TitleBtnState::PressedOut)
                        emit _titleBar->_maximizeButton->mouseMove(false);
                    else
                        emit _titleBar->_maximizeButton->leave();
                }
                break;
            case WM_NCLBUTTONDBLCLK : [[fallthrough]]; // 鼠标左键位于窗口非工作区双击时发出
            case WM_NCLBUTTONUP : // 鼠标左键非客户区松开,相当于点击最大化按钮
                if (_titleBar->_maximizeButton && !_titleBar->_minimizeButton->isHidden())
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
                if (_titleBar->_maximizeButton && !_titleBar->_minimizeButton->isHidden())
                {
                    if (_titleBar->_maximizeButton->_state == TitleBtnState::Pressed || _titleBar->_maximizeButton->_state == TitleBtnState::PressedOut)
                        emit _titleBar->_maximizeButton->leave();
                }
                break;
            case WM_NCLBUTTONDOWN : // 鼠标左键位于窗口非工作区按下时发出
                if (_titleBar->_maximizeButton && !_titleBar->_minimizeButton->isHidden())
                {
                    if (msg->wParam == HTMAXBUTTON)
                    {
                        *result = 0;
                        emit _titleBar->_maximizeButton->mousePress();
                        return true;
                    }
                }
                break;
            case WM_DISPLAYCHANGE : // 分辨率或dpi改变
                setBorderWidth();
                update();
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
                    _verticalLayout->setContentsMargins(_xBorderWidth, _yBorderWidth, _xBorderWidth, _yBorderWidth);
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
        WindowController::setWindowThemeMode(reinterpret_cast<HWND>(winId()), theme);

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

    void Window::setDwmMargins() const
    {
        switch (_dwmWindowEffect)
        {
            case DwmWindowEffect::Acrylic : // 亚克力
            case DwmWindowEffect::Mica : // 云母
            case DwmWindowEffect::MicaAlt : // 云母ALT
                WindowController::extendFrameIntoClientArea(
                    reinterpret_cast<HWND>(winId()),
                    {static_cast<int>(width() * devicePixelRatioF()) + 1, 0, 0, 0}
                );
            default :
                break;
        }
    }

    void Window::setBorderWidth()
    {
#ifdef Q_OS_WIN
#ifdef _MSC_VER
        _xBorderWidth = GetSystemMetrics(SM_CXSIZEFRAME);
        _yBorderWidth = GetSystemMetrics(SM_CYSIZEFRAME);
        if (!_xBorderWidth)
            _xBorderWidth = 5;
        if (!_yBorderWidth)
            _yBorderWidth = 5;
#elif defined(__GNUC__)
        _xBorderWidth = 5;
        _yBorderWidth = 5;
#endif
#endif
    }

    void Window::setDwmWindowEffect(DwmWindowEffect effect)
    {
        // DWM 窗口特效仅支持 Windows 11
        if (!SystemAccessor::isWindows11())
        {
            qWarning() << "The DWM window effect is only supported in Windows 11;";
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
        _useWindowEffects = true;
        _dwmWindowEffect = effect;
        _titleBar->setTransparency();
        auto hWnd = reinterpret_cast<HWND>(winId());
#if (QT_VERSION <= QT_VERSION_CHECK(6, 0, 0))
        setWindowFlags((this->windowFlags()) & (~Qt::WindowType::FramelessWindowHint));
#endif
        setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);
        WindowController::extendFrameIntoClientArea(hWnd);
        WindowController::setDwmWindowEffect(hWnd, effect);
        WindowController::setWindowThemeMode(hWnd, ThemeModeController::controller().appTheme());
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
        HWND hWnd = reinterpret_cast<HWND>(winId());
        _useWindowEffects = true;
        _useCompositionWindowEffect = true;
        _titleBar->setTransparency();
        MARGINS margins{0, 0, 1, 0};
        WindowController::setCompositionWindowEffect(hWnd, effect,ABGR(color.alpha(), color.red(), color.blue(), color.green()));
        WindowController::extendFrameIntoClientArea(hWnd, margins);
    }
} // fancy
