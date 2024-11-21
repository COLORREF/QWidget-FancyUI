#include "FancyWindow.h"

FramelessWindowBase::FramelessWindowBase(QWidget *parent)
    : QWidget{parent}
{
    this->initialize(Type::Base);
}

void FramelessWindowBase::setThemeColor(const QColor &light, const QColor &dark)
{
    if (!this->_themeColorAnimation)
        return;
    this->_themeColorAnimation->setStartValue(light);
    this->_themeColorAnimation->setEndValue(dark);
}

QColor FramelessWindowBase::lightThemeColor()
{
    if (!this->_themeColorAnimation)
        return QColor();
    return this->_themeColorAnimation->lightColor();
}

QColor FramelessWindowBase::darkThemeColor()
{
    if (!this->_themeColorAnimation)
        return QColor();
    return this->_themeColorAnimation->darkColor();
}

void FramelessWindowBase::setThemeColorToggleTime(int msecs)
{
    if (!this->_themeColorAnimation)
        return;
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

void FramelessWindowBase::showSystemTitleBarMenu()
{
    // 获取鼠标全局位置
    POINT p;
    ::GetCursorPos(&p);
    HWND hWnd = (HWND)this->window()->winId();
    HMENU hSysMenu = GetSystemMenu(hWnd, FALSE);// 获取系统菜单
    if (hSysMenu)
    {
        TrackPopupMenu(hSysMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN | TPM_VERNEGANIMATION | TPM_HORNEGANIMATION, p.x, p.y, 0, hWnd, NULL);// 启动菜单的循环
    }
}

void FramelessWindowBase::windowTop(bool top)
{
    if(top)
        WindowManager::setWindowTop((HWND)this->winId());
    else
        WindowManager::cancelWindowTop((HWND)this->winId());
}

#if (QT_VERSION >= QT_VERSION_CHECK(6, 5, 0))
// https://www.qt.io/blog/dark-mode-on-windows-11-with-qt-6.5
// Qt6.5以上会自动检测系统深浅主题，并自动设置窗口和控件的颜色
void FramelessWindowBase::show()
{
    QWidget::show();
    Theme::setTheme(Theme::Type::LIGHT);
}
#endif

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
    this->_mainArea = new QWidget(this); // 用于包裹垂直布局
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
    this->connect(this->_themeColorAnimation, &ThemeColorManagement::valueChanged, this, static_cast<void (FramelessWindowBase::*)()>(&FramelessWindowBase::update));
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
    this->connect(this->windowHandle(),&QWindow::screenChanged,this,[this](QScreen *screen){this->_dpi = screen->devicePixelRatio();});
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
        this->_dpi = this->screen()->devicePixelRatio();
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

F_DEFINITION_PAINTEVENT(FramelessWindowBase)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setBrush(this->_themeColorAnimation->runTimeColor());
    painter.drawRect(this->rect());
}

SimpleFramelessWindow::SimpleFramelessWindow(QWidget *parent)
    : FramelessWindowBase(parent, Type::Simple),
    maximize_button((MaximizeButton *)((SimpleTitleBar *)this->_titleBar)->maximizeButton())
{
}

void SimpleFramelessWindow::deleteTitleBar()
{
    FramelessWindowBase::deleteTitleBar();
    if (this->maximize_button)
        this->maximize_button = nullptr;
}

SimpleFramelessWindow::SimpleFramelessWindow(QWidget *parent, Type type)
    : FramelessWindowBase(parent, type)
{
    switch (type)
    {
    case Type::Base:
        break;
    case Type::Simple:
        this->maximize_button = (MaximizeButton *)((SimpleTitleBar *)this->_titleBar)->maximizeButton();
        break;
    case Type::Standard:
        this->maximize_button = (MaximizeButton *)((StandardTitleBar *)this->_titleBar)->maximizeButton();
        break;
    }
}

F_DEFINITION_NATIVEEVENT(SimpleFramelessWindow)
{
    if (this->maximize_button)
    {
        MSG *msg = (MSG *)message;
        switch (msg->message)
        {
        case WM_NCHITTEST:
        {
            POINT p{GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam)}; // 获取鼠标的绝对坐标
            if (!::MapWindowPoints(HWND_DESKTOP, msg->hwnd, &p, 1))        // 将p转换成相对坐标
            {
                if (!::ScreenToClient((HWND)winId(), &p))
                {
                    QPoint t(this->mapFromGlobal(QPoint(p.x, p.y)));
                    p.x = t.x();
                    p.y = t.y();
                }
            }
            QPoint widget_point(p.x, p.y); // Qt相对坐标
            widget_point /= this->_dpi;    // 除以缩放dpi

            QRect max_btn_rect(maximize_button->mapTo(this, QPoint(0, 0)), maximize_button->size()); // 最大化按钮区域
            if (max_btn_rect.contains(widget_point))                                                 // 鼠标在最大化按钮内
            {
                *result = HTMAXBUTTON;
                if (this->maximize_button->isPressed()) // 鼠标按下
                    emit this->maximize_button->mouseMove(true);
                else
                    emit this->maximize_button->enter();
                return true;
            }
        }
        break;
        case WM_NCMOUSELEAVE: // 光标从非工作区离开
            if (this->maximize_button->isPressed())
                emit this->maximize_button->mouseMove(false);
            else
                emit this->maximize_button->leave();
            break;
        case WM_NCLBUTTONDBLCLK: // 鼠标左键位于窗口非工作区双击时发出
            [[fallthrough]];
        case WM_NCLBUTTONUP: // 鼠标左键非客户区松开,相当于点击最大化按钮
            if (msg->wParam == HTMAXBUTTON)
            {
                emit this->maximize_button->leave();
                if (this->isMaximized())
                    this->showNormal();
                else
                    this->showMaximized();
                *result = 0;
                return true;
            }
            break;
        case WM_LBUTTONUP: // 客户区鼠标左键松开
            if (this->maximize_button->isPressed())
                emit this->maximize_button->leave();
            break;
        case WM_NCLBUTTONDOWN: // 鼠标左键位于窗口非工作区按下时发出
            if (msg->wParam == HTMAXBUTTON)
            {
                *result = 0;
                emit this->maximize_button->mousePress();
                return true;
            }
            break;
        default:
            break;
        }
    }
    return FramelessWindowBase::nativeEvent(eventType, message, result);
}

F_DEFINITION_CHANEGEVENT(SimpleFramelessWindow)
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
            emit this->maximize_button->stateChange(MaximizeButton::State::MAXIMIZE);
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
            emit this->maximize_button->stateChange(MaximizeButton::State::NORMAL);
            this->_verticalLayout->setContentsMargins(0, 0, 0, 0);
            this->update();
        }
    }
    QWidget::changeEvent(event);
}

FramelessWindow::FramelessWindow(QWidget *parent)
    : SimpleFramelessWindow(parent, Type::Standard)
{
}

void FramelessWindow::setWindowIcon(const QPixmap &icon)
{
    ((StandardTitleBar *)(this->_titleBar))->setIcon(icon);
}

void FramelessWindow::setWindowTitle(const QString &title)
{
    ((StandardTitleBar *)(this->_titleBar))->setTitle(title);
}

void FramelessWindow::setTitleFont(const QFont &font)
{
    ((StandardTitleBar *)(this->_titleBar))->setTitleFont(font);
}

void FramelessWindow::setTitleColor(const QColor &globalColor)
{
    ((StandardTitleBar *)(this->_titleBar))->setTitleColor(globalColor);
}

void FramelessWindow::setTitleColor(const QColor &lightColor, const QColor &darkColor)
{
    ((StandardTitleBar *)(this->_titleBar))->setTitleColor(lightColor, darkColor);
}

TransparentEffectWindowBase::TransparentEffectWindowBase(QWidget *parent, WindowEffectType effectType) : FramelessWindow{parent},
    effect_type{effectType}
{
    bool isWindows11 = WindowManager::isWindows11();
    bool isWindowsVersionBelowWindows10 = WindowManager::isWindowsVersionBelowWindows10();

    if (isWindowsVersionBelowWindows10)
    {
        qWarning() << "Operating systems younger than Windows10 May not be supported or may have unknown errors";
        qWarning() << "The window has been initialized as the default window";
        this->effect_type = WindowEffectType::DEFAULT;
    }

    switch (effect_type)
    {
    case WindowEffectType::ACRYLIC: // 亚克力，仅支持 Windows11
        if (isWindows11)
            this->setAcrylicEffect((HWND)this->winId());
        else
        {
            qWarning() << "The acrylic effect is only supported in Windows11";
            qWarning() << "The window has been initialized as the default window";
            this->effect_type = WindowEffectType::DEFAULT;
        }
        return;
    case WindowEffectType::AREO:
        return;                              // 毛玻璃的初始化将在毛玻璃窗口类中（子类）中完成，支持 Windows10 和 Windows11
    case WindowEffectType::FULLYTRANSPARENT: // 全透明，支持 Windows10 和 Windows11
        this->setClientAreaFullyTransparent((HWND)this->winId());
        return;
    case WindowEffectType::MICA: // 云母，仅支持 Windows11
        if (isWindows11)
            this->setMicaEffect((HWND)this->winId());
        else
        {
            qWarning() << "The mica effect is only supported in Windows11";
            qWarning() << "The window has been initialized as the default window";
            this->effect_type = WindowEffectType::DEFAULT;
        }
        return;
    case WindowEffectType::MICAALT: // 云母alt，仅支持 Windows11
        if (isWindows11)
            this->setMicaAltEffect((HWND)this->winId());
        else
        {
            qWarning() << "The mica alt effect is only supported in Windows11";
            qWarning() << "The window has been initialized as the default window";
            this->effect_type = WindowEffectType::DEFAULT;
        }
        return;
    case WindowEffectType::SYSTEMCOLOR: // 系统色，支持 Windows10 和 Windows11
        this->setFollowingSystemColor((HWND)this->winId());
        return;
    case WindowEffectType::DEFAULT: // 默认窗口
        return;
    }
}

F_DEFINITION_PAINTEVENT(TransparentEffectWindowBase)
{
    if (this->effect_type != WindowEffectType::DEFAULT)
    {
        QPainter painter(this);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
        return QWidget::paintEvent(event);
    }
    FramelessWindow::paintEvent(event);
}

AreoWindow::AreoWindow(QWidget *parent)
    : TransparentEffectWindowBase{parent, WindowEffectType::AREO}
{
    this->init();
}

AreoWindow::AreoWindow(COLORREF abgr, QWidget *parent)
    : TransparentEffectWindowBase{parent, WindowEffectType::AREO}
{
    this->init(abgr);
}

void AreoWindow::changeColor(COLORREF abgr)
{
    if (WindowManager::isWindows10() || WindowManager::isWindows11())
    {
        if (this->window_color < 0)
        {
            if (abgr)
            {
                CompositionWindowEffect::removeCompositionWindowEffect((HWND)(this->winId()));
                this->setAreoEffect((HWND)(this->winId()), abgr);
                this->window_color = abgr;
            }
            else
                return;
        }
        else
        {
            if (abgr == this->window_color)
                return;
            CompositionWindowEffect::removeCompositionWindowEffect((HWND)(this->winId()));
            if (abgr)
            {
                this->setAreoEffect((HWND)(this->winId()), abgr);
                this->window_color = abgr;
            }
            else
            {
                this->setAreoEffect((HWND)(this->winId()));
                this->window_color = -1;
            }
        }
    }
}

void AreoWindow::init(long long abgr)
{
    if (WindowManager::isWindows10() || WindowManager::isWindows11())
    {
        if (abgr < 0)
            this->setAreoEffect((HWND)(this->winId()));
        else
        {
            this->setAreoEffect((HWND)(this->winId()), abgr);
            this->window_color = abgr;
        }
    }
}

LightAndDarkWidget::LightAndDarkWidget(QWidget *parent, QColor light, QColor dark)
    : QWidget{parent},
    _themeColorManagement{new ThemeColorManagement(this, light, dark)}
{
    this->_isClearBeforeNewPaint = false;
    this->connect(this->_themeColorManagement, &ThemeColorManagement::valueChanged, this, static_cast<void (QWidget::*)()>(&QWidget::update));
}

F_DEFINITION_PAINTEVENT(LightAndDarkWidget)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(this->_themeColorManagement->runTimeColor());

    if (this->_isClearBeforeNewPaint)
    {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
    }
    painter.drawRect(this->rect());
}

NTQQWindow::NTQQWindow(QWidget *parent) : SimpleFramelessWindow(parent)
{
    QWidget *sidebar_area = new QWidget(this);
    QHBoxLayout *sidbar_area_layout = new QHBoxLayout(sidebar_area);
    sidbar_area_layout->setSpacing(0);
    sidbar_area_layout->setContentsMargins(0, 0, 0, 0);
    this->_sidebar = new LightAndDarkWidget(this, QColor(242, 242, 242, 105), QColor(17, 17, 17, 105));
    sidbar_area_layout->addWidget(this->_sidebar);
    this->_horizontalLayout->insertWidget(0, sidebar_area);
    this->_sidebar->setMinimumWidth(58);
    this->_themeColorAnimation->setThemeColor(QColor(242, 242, 242), QColor(17, 17, 17));
    this->setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground);

    if (WindowManager::isWindows11())
    {
        DwmWindowEffect::setAcrylicEffect(((HWND)this->winId()));
        MARGINS margins = {INT_MAX, 0, 0, 0};
        ::DwmExtendFrameIntoClientArea(((HWND)this->winId()), &margins);
    }
    else if (WindowManager::isWindows10())
    {
        MARGINS margins = {0, 0, 1, 0};
        ::DwmExtendFrameIntoClientArea(((HWND)this->winId()), &margins);
        CompositionWindowEffect::setClientAreaFullyTransparent(((HWND)this->winId()));
    }
    else
    {
        qWarning()
        << "The QQNT window with fuzzy sidebar effect is only effective in Windows11 operating system,"
        << "and the sidebar of Windows10 operating system will not have fuzzy effect."
        << "The effect of the operating system below Windows10 is unknown, and unknown errors may occur.";
    }
}

void NTQQWindow::setUnmaskSidebarColor(bool isUnmask)
{
    if (isUnmask)
    {
        this->_sidebar->themeColorManagement()->setThemeColor(Qt::GlobalColor::transparent, Qt::GlobalColor::transparent);
        this->_sidebar->themeColorManagement()->setEnable(false);
    }
    else
    {
        this->_sidebar->themeColorManagement()->setThemeColor(QColor(242, 242, 242, 105), QColor(17, 17, 17, 105));
        this->_sidebar->themeColorManagement()->setEnable(true);
    }
}

F_DEFINITION_PAINTEVENT(NTQQWindow)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.setBrush(this->_themeColorAnimation->runTimeColor());
    painter.drawRect(QRect(this->_mainArea->mapToParent(QPoint(0, 0)), this->_mainArea->size()));
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.eraseRect(QRect(this->_sidebar->mapFromParent(QPoint(0, 0)), this->_sidebar->size()));
}

MSWindow::MSWindow(QWidget *parent, WindowEffectType effectType) : TransparentEffectWindowBase(parent, effectType)
{
    QWidget *parent_master_scope = TransparentEffectWindowBase::clientArea();
    this->_horizontalLayout = new QHBoxLayout(parent_master_scope);
    this->_clientArea = new MSFilletedCornerWidget(parent_master_scope);
    this->_sidebar = new QWidget(parent_master_scope);
    this->_titleBar->setFixedHeight(50);
    this->_horizontalLayout->setSpacing(0);
    this->_horizontalLayout->setContentsMargins(0, 1, 0, 0);
    this->_horizontalLayout->addWidget(this->_sidebar);
    this->_horizontalLayout->addWidget(this->_clientArea);
    this->_sidebar->setFixedWidth(75);
}

F_DEFINITION_PAINTEVENT(MSFilletedCornerWidget)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(this->_themeColorManagement->runTimeColor());
    QRect rect = this->rect();
    QPainterPath path;
    path.moveTo(0, 10);
    path.lineTo(rect.bottomLeft());
    path.lineTo(rect.bottomRight());
    path.lineTo(rect.topRight());
    path.lineTo(10, 0);
    path.arcTo(QRect(rect.left(), rect.top(), 20, 20), 90, 90);
    path.closeSubpath();
    if (Theme::isLigth())
        painter.setPen(QPen(QColor(0, 0, 0, 17), 1));
    else
        painter.setPen(QPen(QColor(0, 0, 0, 46), 1));
    painter.drawPath(path);
}

AdaptiveLayoutWindow::AdaptiveLayoutWindow(QWidget *parent,int sidebarMinimumWidth, int sidebarMaximumWidth, WindowEffectType effectType)
    : MSWindow{parent, effectType}
{
    this->_isExpand = false;
    this->_animation = new QVariantAnimation(this);
    this->_animation->setDuration(200);
    this->_animation->setStartValue(sidebarMaximumWidth);
    this->_animation->setEndValue(sidebarMinimumWidth);

    connect(this->_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant &value){
        this->_sidebar->setFixedWidth(value.value<int>());
    });

    connect(this->_animation, &QVariantAnimation::finished, this, [this](){
        this->_isExpand = !this->_isExpand;
        this->checkWidth();
        emit this->expand(this->_isExpand);
    });

    this->_sidebar->setFixedWidth(sidebarMinimumWidth);
}
void AdaptiveLayoutWindow::checkWidth()
{
    int w = this->width();
    if (w <= 850 && this->_animation->state() != QVariantAnimation::State::Running && this->_isExpand)
    {
        this->_animation->setDirection(QVariantAnimation::Direction::Forward);
        this->_animation->start();
    }

    else if (w > 850 && this->_animation->state() != QVariantAnimation::State::Running && !this->_isExpand)
    {
        this->_animation->setDirection(QVariantAnimation::Direction::Backward);
        this->_animation->start();
    }
}

F_DEFINITION_RESIZEEVENT(AdaptiveLayoutWindow)
{
    this->checkWidth();
    QWidget::resizeEvent(event);
}
