#include "framelesswindowbase.h"
#include "titlebarbase.h"
#include "simpletitlebar.h"
#include "standardtitlebar.h"
#include "win32_utils/dwmwindoweffect.h"

// #ifdef _MSC_VER
// #ifndef Linked
// #pragma comment(lib, "user32.lib")
// #pragma comment(lib, "dwmapi.lib")
// #pragma comment(lib, "Advapi32.lib")
// #define Linked
// #endif
// #endif
FramelessWindowBase::FramelessWindowBase(QWidget *parent)
    : QWidget{parent}
{
    this->initialize(Type::Base);
}

void FramelessWindowBase::setThemeColor(const QColor &light, const QColor &dark)
{
    if (!this->_themeColorAnimation)return;
    this->_themeColorAnimation->setStartValue(light);
    this->_themeColorAnimation->setEndValue(dark);
}

QColor FramelessWindowBase::lightThemeColor()
{
    if (!this->_themeColorAnimation)return QColor();
    return this->_themeColorAnimation->lightColor();
}

QColor FramelessWindowBase::darkThemeColor()
{
    if (!this->_themeColorAnimation)return QColor();
    return this->_themeColorAnimation->darkColor();
}

void FramelessWindowBase::setThemeColorToggleTime(int msecs)
{
    if (!this->_themeColorAnimation)return;
    this->_themeColorAnimation->setDuration(msecs);
}

void FramelessWindowBase::deleteTitleBar()
{
    if (this->_titleBar)
    {
        this->_titleBar->close();
        this->_verticalLayout->removeWidget(this->_titleBar);
        delete this->_titleBar;
        this->_titleBar = nullptr;
    }
}

void FramelessWindowBase::onThemeChange(Theme::Type type)
{
    if (type == Theme::Type::LIGHT)
        DwmWindowEffect::setWindowLightMode((HWND)this->winId());
    else
        DwmWindowEffect::setWindowDarkMode((HWND)this->winId());
}

F_DEFINITION_CHANEGEVENT(FramelessWindowBase)
{
    if (event->type() == QEvent::Type::WindowStateChange)
    {
        QWindowStateChangeEvent *stateChangeEvent = static_cast<QWindowStateChangeEvent *>(event);
        // 最大化后最小化
        if (stateChangeEvent->oldState() & Qt::WindowState::WindowMaximized && this->windowState() & Qt::WindowState::WindowMinimized)
            return QWidget::changeEvent(event);
        // 最大化
        if (this->windowState() & Qt::WindowState::WindowMaximized)
        {
            this->_verticalLayout->setContentsMargins(
                this->X_BORDER_WIDTH,
                this->Y_BORDER_WIDTH,
                this->X_BORDER_WIDTH,
                this->Y_BORDER_WIDTH);
            this->update();
        }
        // 最大化后还原
        if (stateChangeEvent->oldState() & Qt::WindowState::WindowMaximized && !(this->windowState() & Qt::WindowState::WindowMaximized))
        {
            this->_verticalLayout->setContentsMargins(0, 0, 0, 0);
            this->update();
        }
    }
    QWidget::changeEvent(event);
}

void FramelessWindowBase::initialize(Type type)
{
    this->_type = type;

    this->_horizontalLayout = new QHBoxLayout(this);
    this->_horizontalLayout->setSpacing(0);
    this->_horizontalLayout->setContentsMargins(0, 0, 0, 0);
    this->_mainArea = new QWidget(this);//用于包裹垂直布局
    this->_horizontalLayout->addWidget(this->_mainArea);

    this->_verticalLayout = new QVBoxLayout(this->_mainArea);
    this->_clientArea = new QWidget(this);
    switch (this->_type)
    {
    case Type::Base:
        this->_titleBar = new TitleBarBase(this);
        break;
    case Type::Simple:
        this->_titleBar = new SimpleTitleBar(this);
        break;
    case Type::Standard:
        this->_titleBar = new StandardTitleBar(this);
        break;
    }

    this->_themeColorAnimation = new ThemeColorManagement(this);
    this->connect(this->_themeColorAnimation,&ThemeColorManagement::valueChanged, this, static_cast<void(FramelessWindowBase::*)()>(&FramelessWindowBase::update));
    this->_verticalLayout->setSpacing(0);
    this->_verticalLayout->setContentsMargins(0, 0, 0, 0);
    this->_verticalLayout->addWidget(this->_titleBar);
    this->_verticalLayout->addWidget(this->_clientArea);

    this->_dpi = this->screen()->devicePixelRatio();
#ifdef _MSC_VER
    this->X_BORDER_WIDTH = ::GetSystemMetrics(SM_CXSIZEFRAME);
    this->Y_BORDER_WIDTH = ::GetSystemMetrics(SM_CYSIZEFRAME);
    if (!this->X_BORDER_WIDTH)
        this->X_BORDER_WIDTH = 5;
    if (!this->Y_BORDER_WIDTH)
        this->Y_BORDER_WIDTH = 5;
#elif defined(__GNUC__)
    this->X_BORDER_WIDTH = 5;
    this->Y_BORDER_WIDTH = 5;
#endif
    MARGINS margins = {1, 1, 0, 1};
    ::DwmExtendFrameIntoClientArea((HWND)this->winId(), &margins);
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    ::SetWindowLongPtr((HWND)this->winId(), GWL_STYLE, ::GetWindowLongPtr((HWND)this->winId(), GWL_STYLE) | WS_BORDER | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU);
    this->connect(Theme::themeObject(), &Theme::themeChange, this, &FramelessWindowBase::onThemeChange);
    this->resize(640, 480);
}

FramelessWindowBase::FramelessWindowBase(QWidget *parent, Type type)
    : QWidget{parent}
{
    this->initialize(type);
}

F_DEFINITION_NATIVEEVENT(FramelessWindowBase)
{
    MSG *msg = (MSG *)message;
    switch (msg->message)
    {
    case WM_NCCALCSIZE: // 取消原有的标题栏和边框
        *result = msg->wParam ? 0 : WVR_REDRAW;
        return true;

    case WM_NCHITTEST: // 光标位置测试
    {
        if (!this->isMaximized())
        {
            POINT p{GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)}; // 获取鼠标的绝对坐标
            RECT widget_rect{0, 0, 0, 0};
            ::GetWindowRect(msg->hwnd, &widget_rect); // 获取窗口的绝对区域

            // 综合考虑使用习惯以及其他一些写法（8个if直接判断、九宫格位视图法）
            // 下面的代码虽然有些冗余，但能有效减少if判断的次平均次数以及减少临时变量的创建
            if (p.x <= widget_rect.right && p.x >= widget_rect.right - this->X_BORDER_WIDTH) // 右
            {
                if (p.y <= widget_rect.bottom && p.y >= widget_rect.bottom - this->Y_BORDER_WIDTH) // 下
                    *result = HTBOTTOMRIGHT;
                else if (p.y >= widget_rect.top && p.y <= widget_rect.top + this->Y_BORDER_WIDTH) // 上
                    *result = HTTOPRIGHT;
                else
                    *result = HTRIGHT;
                return true;
            }
            if (p.y <= widget_rect.bottom && p.y >= widget_rect.bottom - this->Y_BORDER_WIDTH) // 下
            {
                if (p.x >= widget_rect.left && p.x <= widget_rect.left + this->X_BORDER_WIDTH) // 左
                    *result = HTBOTTOMLEFT;
                else
                    *result = HTBOTTOM;
                return true;
            }
            if (p.y >= widget_rect.top && p.y <= widget_rect.top + this->Y_BORDER_WIDTH) // 上
            {
                if (p.x >= widget_rect.left && p.x <= widget_rect.left + this->X_BORDER_WIDTH) // 左
                    *result = HTTOPLEFT;
                else
                    *result = HTTOP;
                return true;
            }
            if (p.x >= widget_rect.left && p.x <= widget_rect.left + this->X_BORDER_WIDTH) // 左
            {
                *result = HTLEFT;
                return true;
            }
        }
    }
    break;
    case WM_DISPLAYCHANGE: // 分辨率或dpi改变
        this->_dpi = screen()->devicePixelRatio();
#ifdef _MSC_VER
        this->X_BORDER_WIDTH = ::GetSystemMetrics(SM_CXSIZEFRAME);
        this->Y_BORDER_WIDTH = ::GetSystemMetrics(SM_CYSIZEFRAME);
        if (!this->X_BORDER_WIDTH)
            this->X_BORDER_WIDTH = 5;
        if (!this->Y_BORDER_WIDTH)
            this->Y_BORDER_WIDTH = 5;
#endif
        this->update();
        break;
    case WM_WININICHANGE:
        *result = 0;
        // const char* str = (const char*)(msg->lParam);//ImmersiveColorSet
        return true;
    default:
        break;
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void FramelessWindowBase::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setBrush(this->_themeColorAnimation->runTimeColor());
    painter.drawRect(this->rect());
}
