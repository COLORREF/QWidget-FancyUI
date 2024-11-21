#include "FancySidebar.h"
#include "FancyStyle.h"

static constexpr int LINE_WHID = 3;
static constexpr int ContentsMargins_Left = 2;
static constexpr int ContentsMargins_Top = 5;
static constexpr int ContentsMargins_Right = 2;
static constexpr int ContentsMargins_Bottom = 5;

OptionButtonBase::OptionButtonBase(QWidget *parent, int sidebarWidth):
    TransparentButton(parent),
    _iconWidget{new FancyIconWidget(this)},
    _textLabel{new QLabel(this)}
{
    this->setCheckable(true);
    this->preserveTransparency(true);
    QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    this->setSizePolicy(sizePolicy);

    int minimumWidth = static_cast<int>(static_cast<float>(sidebarWidth)/1.25);
    int fixedHeight = minimumWidth;

    this->setMinimumWidth(minimumWidth);
    this->setFixedHeight(fixedHeight);
    this->resize(minimumWidth,fixedHeight);
    int ilWH = minimumWidth/2;
    int ily = (this->height()-ilWH)/2;
    int ilx = (this->width()-ilWH)/2;
    this->_iconWidget->resize({ilWH,ilWH});
    this->_iconWidget->move(ilx, ily);


    int tlH = ilWH;
    int tlX = sidebarWidth + ContentsMargins_Right + ContentsMargins_Left;
    int tlY = (this->height()-tlH)/2;
    this->_textLabel->setFixedHeight(tlH);
    this->_textLabel->setScaledContents(true);
    this->_textLabel->move(tlX, tlY);
    this->_textLabel->setStyle(new FancyStyleBase(nullptr));

    constexpr int Lx = LINE_WHID + LINE_WHID;
    int Ly1 = this->height() / 4;
    int Ly2 = Ly1 * 2 + Ly1;
    this->_indicator = QLine(Lx, Ly1, Lx, Ly2);
}

OptionButtonBase::OptionButtonBase(const QString &text, QWidget *parent, int sidebarWidth):
    OptionButtonBase(parent,sidebarWidth)
{
    this->setText(text);
}

OptionButtonBase::OptionButtonBase(const QString &text, QWidget *parent, FancyIcon iconEnum, int sidebarWidth):
    OptionButtonBase(text,parent,sidebarWidth)
{
    this->setIcon(iconEnum);
}


void OptionButtonBase::setIcon(const QPixmap &icon)
{
    this->_iconWidget->loadPixmap(icon);
}

void OptionButtonBase::setIcon(FancyIcon iconEnum)
{
    this->_iconWidget->loadFancyIcon(iconEnum);
}

void OptionButtonBase::setIcon(const QPicture &picture)
{
    this->_iconWidget->loadPicture(picture);
}

void OptionButtonBase::setSvgIcon(const QString &path)
{
    this->_iconWidget->loadSvg(path);
}

void OptionButtonBase::setSvgIcon(const QByteArray &data)
{
    this->_iconWidget->loadSvg(data);
}

void OptionButtonBase::correctIconCoordinates()
{
    int minimumWidth = this->minimumWidth();
    int ilWH = minimumWidth/2;
    int ily = (this->height()-ilWH)/2;
    int ilx = (this->width()-ilWH)/2;
    this->_iconWidget->move(ilx + LINE_WHID, ily);
}

void OptionButtonBase::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::RenderHint::Antialiasing); // 抗锯齿
    if (this->isChecked() && this->_drawIndicator)
    {
        QPen pen(ControlColors::controlColors()->theme());
        pen.setCapStyle(Qt::PenCapStyle::RoundCap);
        pen.setWidth(LINE_WHID);
        painter.setPen(pen);
        painter.drawLine(this->_indicator);
    }
    return TransparentButton::paintEvent(event);
}



SidebarBase::SidebarBase(QWidget *parent):
    QWidget(parent),
    _verticalLayout(new QVBoxLayout(this)),
    _btnGroup(new QButtonGroup(this)),
    _verticalSpacer(new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding))
{
    this->_verticalLayout->setSpacing(2);
    this->_verticalLayout->setContentsMargins(ContentsMargins_Left, ContentsMargins_Top, ContentsMargins_Right, ContentsMargins_Bottom);
    this->_btnGroup->setExclusive(true);//启用互斥
    this->_verticalLayout->addItem(_verticalSpacer);
    connect(this->_btnGroup,&QButtonGroup::idClicked,this,[=](int id){emit this->optionChecked(id);});
}

void SidebarBase::addOption(QAbstractButton *option)
{
    int id = _btnGroup->buttons().size();
    this->_btnGroup->addButton(option,id);
    this->_verticalLayout->insertWidget(id,option);
}

StandardSidebar::StandardSidebar(QWidget *parent, int upperAreaHeight, int underrAreaHeight):
    SidebarBase(parent),
    _upperArea(new QWidget(this)),
    _scrollArea(new InertiaScrollArea(this)),
    _underrArea(new QWidget(this))

{
    this->_verticalLayout->removeItem(this->_verticalSpacer);
    this->_verticalLayout->setContentsMargins(0, 0, 0, 0);

    this->_scrollArea->addItem(this->_verticalSpacer);
    this->_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->_upperArea->setFixedHeight(upperAreaHeight);
    this->_underrArea->setFixedHeight(underrAreaHeight);

    this->_verticalLayout->addWidget(this->_upperArea);
    this->_verticalLayout->addWidget(this->_scrollArea);
    this->_verticalLayout->addWidget(this->_underrArea);

    QVBoxLayout* vup = new QVBoxLayout(this->_upperArea);
    vup->setContentsMargins(2,2,2,2);
    vup->setSpacing(2);

    QVBoxLayout* vun = new QVBoxLayout(this->_underrArea);
    vun->setContentsMargins(2,2,2,2);
    vun->setSpacing(2);
}


AnimationOptionButton::AnimationOptionButton(QWidget *parent, int sidebarWidth):
    OptionButtonBase(parent,sidebarWidth)
{
    constexpr int x = LINE_WHID * 2;
    this->_upIndicator = QLine(x,0,x,0);
    this->_unIndicator = QLine(x,this->height(),x,this->height());
    this->_topToMiddle = new QVariantAnimation(this);
    this->_topToMiddle->setStartValue(this->_upIndicator);
    this->_topToMiddle->setEndValue(this->_indicator);
    this->_topToMiddle->setDuration(400);

    this->_middleToBottom = new QVariantAnimation(this);
    this->_middleToBottom->setStartValue(this->_indicator);
    this->_middleToBottom->setEndValue(this->_unIndicator);
    this->_middleToBottom->setDuration(400);

    this->installEventFilter(this);

    _drawIndicator = false;
    connect(this->_topToMiddle,&QVariantAnimation::valueChanged,this,[this](const QVariant &value){this->_indicator = value.toLine();update();});
    connect(this->_middleToBottom,&QVariantAnimation::valueChanged,this,[this](const QVariant &value){this->_indicator = value.toLine();update();});

    int minimumWidth = static_cast<int>(static_cast<float>(sidebarWidth)/1.25);
    int ilWH = minimumWidth/2;
    int ily = (this->height()-ilWH)/2;
    int ilx = (this->width()-ilWH)/2;
    connect(this,&OptionButtonBase::toggled,this,[ilx,ily,this](bool checked){
        if(checked)
            this->_iconWidget->move(ilx + LINE_WHID, ily);
        else
            this->_iconWidget->move(ilx, ily);
    });
}

AnimationOptionButton::AnimationOptionButton(const QString &text, QWidget *parent, int sidebarWidth):
    AnimationOptionButton(parent,sidebarWidth)
{
    this->setText(text);
}

AnimationOptionButton::AnimationOptionButton(const QString &text, QWidget *parent, FancyIcon iconEnum, int sidebarWidth):
    AnimationOptionButton(text,parent,sidebarWidth)
{
    this->setIcon(iconEnum);
}

bool AnimationOptionButton::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this && (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick) && this->isChecked() )
        return true;
    return TransparentButton::eventFilter(obj,event);
}
void AnimationOptionButton::paintEvent(QPaintEvent *event)
{
    if(_drawIndicator)
    {
        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing);
        QPen pen(ControlColors::controlColors()->theme());
        pen.setCapStyle(Qt::PenCapStyle::RoundCap);
        pen.setWidth(LINE_WHID);
        painter.setPen(pen);
        painter.drawLine(this->_indicator);
    }
    TransparentButton::paintEvent(event);
}


AnimationIndicatorSidebar::AnimationIndicatorSidebar(QWidget *parent, int upperAreaHeight, int underrAreaHeight):
    StandardSidebar(parent,upperAreaHeight,underrAreaHeight)

{
    this->_checked.enqueue(0);

    connect(this->_btnGroup,&QButtonGroup::idToggled,this,[this](int id, bool checked){
        if(checked)
        {
            int leaveId = this->_checked.dequeue();
            this->_checked.enqueue(id);
            AnimationOptionButton* enter = (AnimationOptionButton*)(this->_btnGroup->button(id));
            AnimationOptionButton* leave = (AnimationOptionButton*)(this->_btnGroup->button(leaveId));
            enter->_drawIndicator = true;
            leave->_drawIndicator = true;
            if(id > leaveId)
            {
                leave->middleToBottom();
                enter->_indicator = enter->_upIndicator;
                enter->topToMiddle();
            }
            if(id < leaveId)
            {
                leave->middleToTop();
                enter->_indicator = enter->_unIndicator;
                enter->bottomToMiddle();
            }
        }
    });
}

void AnimationIndicatorSidebar::addOption(AnimationOptionButton *option)
{
    int id = _btnGroup->buttons().size();
    this->_btnGroup->addButton(option,id);
    this->_scrollArea->insertWidget(id,option);
    if(id == 0)
        option->setChecked(true);
}

void AnimationIndicatorSidebar::startAnimation(int id, bool checked)
{
    if(checked)
    {
        int leaveId = this->_checked.dequeue();
        this->_checked.enqueue(id);
        AnimationOptionButton* enter = (AnimationOptionButton*)(this->_btnGroup->button(id));
        AnimationOptionButton* leave = (AnimationOptionButton*)(this->_btnGroup->button(leaveId));
        enter->_drawIndicator = true;
        leave->_drawIndicator = true;
        if(id > leaveId)
        {
            leave->middleToBottom();
            enter->_indicator = enter->_upIndicator;
            enter->topToMiddle();
        }
        if(id < leaveId)
        {
            leave->middleToTop();
            enter->_indicator = enter->_unIndicator;
            enter->bottomToMiddle();
        }
    }
}

