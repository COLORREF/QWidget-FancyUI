#include "FancyTitlebar.h"
#include "FancyIcon.h"

MouseEventColorManagement::MouseEventColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush)
{
    this->_normalBrush = normalBrush;
    this->_hoverBrush = hoverBrush;
    this->_pressHoverBrush = pressHoverBrush;
    this->_pressLeaveBrush = pressLeaveBrush;
}

MouseEventColorManagement::MouseEventColorManagement(const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen)
{
    this->_normalPen = normalPen;
    this->_hoverPen = hoverPen;
    this->_pressHoverPen = pressHoverPen;
    this->_pressLeavePen = pressLeavePen;
}

MouseEventColorManagement::MouseEventColorManagement(const QBrush &normalBrush, const QBrush &hoverBrush, const QBrush &pressHoverBrush, const QBrush &pressLeaveBrush, const QPen &normalPen, const QPen &hoverPen, const QPen &pressHoverPen, const QPen &pressLeavePen)
{
    this->_normalBrush = normalBrush;
    this->_hoverBrush = hoverBrush;
    this->_pressHoverBrush = pressHoverBrush;
    this->_pressLeaveBrush = pressLeaveBrush;
    this->_normalPen = normalPen;
    this->_hoverPen = hoverPen;
    this->_pressHoverPen = pressHoverPen;
    this->_pressLeavePen = pressLeavePen;
}

TitleBarButton::TitleBarButton(QWidget *parent)
    : QPushButton{parent}
{
    this->_mouseState = MouseState::NORMAL;
    this->_radius = 0;
    this->_iconWidget = new FancyIconWidget(this);
    this->_isClearBeforeNewPaint = false;
    this->setFixedSize(46, 32);
    this->_normalBrush.setColor(Qt::GlobalColor::transparent);
    this->_hoverBrush.setColor(QColor::fromRgbF((float)0.51, (float)0.51, (float)0.51, (float)0.1));
    this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05)); // 浅色背景点击时颜色减淡，深色背景颜色加深
    this->_pressLeaveBrush = this->_normalBrush;

    this->_normalBrush.setStyle(Qt::BrushStyle::SolidPattern);
    this->_hoverBrush.setStyle(Qt::BrushStyle::SolidPattern);
    this->_pressHoverBrush.setStyle(Qt::BrushStyle::SolidPattern);
    this->_pressLeaveBrush.setStyle(Qt::BrushStyle::SolidPattern);

    int m = this->width() > this->height() ? this->height() : this->width();
    this->_iconWidget->resize(m / 2, m / 2);
    this->_iconWidget->move((this->width() - this->_iconWidget->width()) / 2, (this->height() - this->_iconWidget->height()) / 2);
    connect(Theme::themeObject(), &Theme::themeChange, this, &TitleBarButton::onThemeChange);
}

F_DEFINITION_ENTEREVENT(TitleBarButton)
{
    this->setMouseState(MouseState::HOVER);
    QPushButton::enterEvent(event);
}

F_DEFINITION_LEAVEEVENT(TitleBarButton){
    this->setMouseState(MouseState::NORMAL);
    QPushButton::leaveEvent(event);
}

F_DEFINITION_MOUSEPRESSEVENT(TitleBarButton)
{
    if(event->button() == Qt::LeftButton)
        this->setMouseState(MouseState::PRESSEDHOVER);
    QPushButton::mousePressEvent(event);
}

F_DEFINITION_MOUSERELEASEEVENT(TitleBarButton)
{
    if (this->rect().contains(event->pos()))
        this->setMouseState(MouseState::HOVER);
    else
        this->setMouseState(MouseState::NORMAL);
    QPushButton::mouseReleaseEvent(event);
}

F_DEFINITION_MOUSEMOVEEVENT(TitleBarButton)
{
    // 不开启鼠标追踪只有左键按下才能触发
    if (this->rect().contains(event->pos()))
        this->setMouseState(MouseState::PRESSEDHOVER);
    else
        this->setMouseState(MouseState::PRESSEDLEAVE);
    QPushButton::mouseMoveEvent(event);
}

F_DEFINITION_PAINTEVENT(TitleBarButton)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setBrush(this->drawBrush());
    painter.setPen(Qt::PenStyle::NoPen);
    if (this->_isClearBeforeNewPaint && (this->_mouseState == MouseState::NORMAL || this->_mouseState == MouseState::PRESSEDLEAVE)) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
    } painter.drawRoundedRect(this->rect(), this->_radius, this->_radius);
}

bool TitleBarButton::isPressed()
{
    return this->_mouseState == MouseState::PRESSEDHOVER || this->_mouseState == MouseState::PRESSEDLEAVE;
}

bool TitleBarButton::isHover()
{
    return this->_mouseState == MouseState::HOVER || this->_mouseState == MouseState::PRESSEDHOVER;
}

void TitleBarButton::setIcon(const QPixmap &icon)
{
    this->_iconWidget->loadPixmap(icon);
}

void TitleBarButton::setIcon(FancyIcon iconEnum, bool autoReverse)
{
    this->_iconWidget->loadFancyIcon(iconEnum, autoReverse);
}

void TitleBarButton::setIcon(const QPicture &picture)
{
    this->_iconWidget->loadPicture(picture);
}

void TitleBarButton::setSvgIcon(const QString &path)
{
    this->_iconWidget->loadSvg(path);
}

void TitleBarButton::setSvgIcon(const QByteArray &data)
{
    this->_iconWidget->loadSvg(data);
}

void TitleBarButton::onThemeChange(Theme::Type themeType)
{
    if (themeType == Theme::Type::LIGHT)
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05));
    else
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.2));
}

const QPen &TitleBarButton::drawPen()
{
    switch (this->_mouseState)
    {
    case MouseState::HOVER:
        return this->_hoverPen;
    case MouseState::NORMAL:
        return this->_normalPen;
    case MouseState::PRESSEDHOVER:
        return this->_pressHoverPen;
    default:
        return this->_pressLeavePen;
    }
}

const QBrush &TitleBarButton::drawBrush()
{
    switch (this->_mouseState)
    {
    case MouseState::HOVER:
        return this->_hoverBrush;
    case MouseState::NORMAL:
        return this->_normalBrush;
    case MouseState::PRESSEDHOVER:
        return this->_pressHoverBrush;
    default:
        return this->_pressLeaveBrush;
    }
}


MinimizeButton::MinimizeButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(FancyIcon::Minimize);
    connect(this, &MinimizeButton::clicked, this->window(), &QWidget::showMinimized);
    // connect(this, &MinimizeButton::clicked,this,[this](){SendMessage((HWND)(this->window()->winId()), WM_SYSCOMMAND, SC_MINIMIZE, 0);});
}

void MinimizeButton::onThemeChange(Theme::Type themeType)
{
    if (themeType == Theme::Type::LIGHT)
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05));
    else
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.2));
}

MaximizeButton::MaximizeButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(FancyIcon::Maximize);
    connect(this, &MaximizeButton::enter, this, &MaximizeButton::onEnter);
    connect(this, &MaximizeButton::leave, this, &MaximizeButton::onLeave);
    connect(this, &MaximizeButton::mousePress, this, &MaximizeButton::onMousePress);
    connect(this, &MaximizeButton::mouseRelease, this, &MaximizeButton::onMouseRelease);
    connect(this, &MaximizeButton::mouseMove, this, &MaximizeButton::onMouseMove);
    connect(this, &MaximizeButton::stateChange, this, &MaximizeButton::onStateChange);
}

MaximizeButton::State MaximizeButton::maxmizeState()
{
    return this->state;
}

void MaximizeButton::onThemeChange(Theme::Type themeType)
{
    if (themeType == Theme::Type::LIGHT)
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.05)); // 浅色背景点击时颜色减淡，声色背景颜色加深
    else
        this->_pressHoverBrush.setColor(QColor::fromRgbF((float)0.43, (float)0.43, (float)0.43, (float)0.2));
}

void MaximizeButton::onStateChange(MaximizeButton::State state)
{
    if (state == State::NORMAL)
        this->setIcon(FancyIcon::Maximize);
    else
        this->setIcon(FancyIcon::Restore);
}

void MaximizeButton::onEnter()
{
    this->setMouseState(TitleBarButton::MouseState::HOVER);
}

void MaximizeButton::onLeave()
{
    this->setMouseState(TitleBarButton::MouseState::NORMAL);
}

void MaximizeButton::onMousePress()
{
    this->setMouseState(TitleBarButton::MouseState::PRESSEDHOVER);
}

void MaximizeButton::onMouseRelease(bool isInSelf)
{
    if (isInSelf)
        this->setMouseState(TitleBarButton::MouseState::HOVER);
    else
        this->setMouseState(TitleBarButton::MouseState::NORMAL);
}

void MaximizeButton::onMouseMove(bool isInSelf)
{
    if (isInSelf)
        this->setMouseState(TitleBarButton::MouseState::PRESSEDHOVER);
    else
        this->setMouseState(TitleBarButton::MouseState::PRESSEDLEAVE);
}

CloseButton::CloseButton(QWidget *parent)
    : TitleBarButton{parent}
{
    this->setIcon(FancyIcon::Close);
    this->_hoverBrush.setColor(QColor(196, 43, 28));
    this->_pressHoverBrush.setColor(QColor(200, 64, 49));
    this->connect(this, &CloseButton::clicked, this->window(), &QWidget::close);
}

void CloseButton::onThemeChange(Theme::Type)
{
    this->setIcon(FancyIcon::Close);
}

F_DEFINITION_ENTEREVENT(CloseButton)
{
    if (Theme::isLigth())
        this->setIcon(QPixmap(fancyIconPath(FancyIcon::Close,Theme::Type::LIGHT)));
    return TitleBarButton::enterEvent(event);
}

F_DEFINITION_LEAVEEVENT(CloseButton)
{
    if (Theme::isLigth())
        this->setIcon(FancyIcon::Close);
    return TitleBarButton::leaveEvent(event);
}

F_DEFINITION_MOUSEMOVEEVENT(CloseButton)
{
    if (this->rect().contains(event->pos()))
        this->setIcon(QPixmap(fancyIconPath(FancyIcon::Close,Theme::Type::LIGHT)));
    else
    {
        if (Theme::isLigth())
            this->setIcon(FancyIcon::Close);
    }
    TitleBarButton::mouseMoveEvent(event);
}

    // this->setMinimumSize(QSize(0, 0)); // 重置最小尺寸
    // this->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)); // 重置最大尺寸

TitleBarBase::TitleBarBase(QWidget *parent)
    : QWidget{parent}
{
    this->_closeButton = new CloseButton(this);
    this->_horizontalLayout = new QHBoxLayout(this);
    this->_horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    this->setMinimumHeight(32);
    this->setLayout(this->_horizontalLayout);
    QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);
    this->_horizontalLayout->setSpacing(0);
    this->_horizontalLayout->setContentsMargins(0, 0, 0, 0);
    this->_horizontalLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    this->_horizontalLayout->addItem(this->_horizontalSpacer);
    this->_horizontalLayout->addWidget(this->_closeButton);
}

F_DEFINITION_MOUSEPRESSEVENT(TitleBarBase)
{
    Q_UNUSED(event)
    this->window()->windowHandle()->startSystemMove();
}

SimpleTitleBar::SimpleTitleBar(QWidget *parent)
    : TitleBarBase{parent},
    _minimizeButton{new MinimizeButton(this)},
    _maximizeButton{new MaximizeButton(this)}
{
    this->_horizontalLayout->insertWidget(1, this->_maximizeButton);
    this->_horizontalLayout->insertWidget(1, this->_minimizeButton);
}

F_DEFINITION_MOUSEDOUBLECLICKEVENT(SimpleTitleBar)
{
    if (event->button() == Qt::MouseButton::LeftButton)
    {
        this->_maximizeButton->click();
        if (window()->isMaximized())
            window()->showNormal();
        else
            window()->showMaximized();
    }
}

StandardTitleBar::StandardTitleBar(QWidget *parent)
    : SimpleTitleBar{parent},
    _iconLabel{new QLabel(this)},
    _titleLabel{new TitleTextLabel(this)}

{
    this->_titleLabel->setIsClearBeforeNewPaint(false);
    this->_horizontalLayout->setContentsMargins(8, 0, 0, 0);
    this->_iconLabel->setFixedSize(25, 20);
    this->_iconLabel->setScaledContents(true);
    this->_iconLabel->setContentsMargins(0, 0, 5, 0);
    this->_titleLabel->setFont(QFont("Microsoft YaHei", 9));
    this->_horizontalLayout->insertWidget(0, _titleLabel);
    this->_horizontalLayout->insertWidget(0, _iconLabel);

    QPalette p(this->_titleLabel->palette());
    p.setBrush(QPalette::ColorRole::WindowText,Qt::GlobalColor::black);
    this->_titleLabel->setPalette(p);

    // 设置默认图标和标题
    _iconLabel->setPixmap(this->style()->standardIcon(QStyle::StandardPixmap::SP_ComputerIcon).pixmap(20, 20));
    _titleLabel->setText(QApplication::applicationName());

    this->connect(Theme::themeObject(),&Theme::themeChange,this,&StandardTitleBar::onThemeChange);
}

void StandardTitleBar::setIcon(const QPixmap &icon)
{
    this->_iconLabel->setPixmap(icon);
}

void StandardTitleBar::setTitle(const QString &title)
{
    this->_titleLabel->setText(title);
}

void StandardTitleBar::setTitleFont(const QFont &font)
{
    this->_titleLabel->setFont(font);
}

void StandardTitleBar::setTitleColor(const QColor &globalColor)
{
    this->setTitleColor(globalColor, globalColor);
}

void StandardTitleBar::setTitleColor(const QColor &lightColor, const QColor &darkColor)
{
    this->_lightTitltTextColor = lightColor;
    this->_darkTitltTextColor = darkColor;
    this->_titleLabel->update();
}

void StandardTitleBar::onThemeChange(Theme::Type)
{
    this->_titleLabel->update();
}

F_DEFINITION_PAINTEVENT(TitleTextLabel)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::TextAntialiasing);
    if (this->_isClearBeforeNewPaint)
    {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.eraseRect(this->rect());
    }
    if (Theme::isLigth())
        (const_cast<QBrush&>(this->palette().windowText())).setColor(((StandardTitleBar*)this->parentWidget())->_lightTitltTextColor);
    else
        (const_cast<QBrush&>(this->palette().windowText())).setColor(((StandardTitleBar*)this->parentWidget())->_darkTitltTextColor);
    QLabel::paintEvent(event);
}
