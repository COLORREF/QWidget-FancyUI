#include "sidebar.h"
#include <QLibraryInfo>
Sidebar::Sidebar(QWidget *parent, int initialWidth)
    : QWidget{parent},
      animation{new QPropertyAnimation(this, "size")},
      verticalLayout{new QVBoxLayout(this)}
{
    this->verticalLayout->setSpacing(4);
    this->verticalLayout->setContentsMargins(5, 5, 5, 5);
    this->animation->setDuration(150);
    this->borderLinePen.setWidth(2);

    connect(this->animation, &QPropertyAnimation::finished, this, [&]
            {
        if(this->initialSize.height()!=this->window()->height())
        {
            //动画结束,高度和父窗口不等,说明动画进行的过程中改变了窗口大小,需重新设置高度
            this->initialSize.rheight()=this->window()->height();
            this->endSize.rheight() = this->initialSize.rheight();
            this->animation->setStartValue(this->initialSize);
            this->animation->setEndValue(this->endSize);
            this->resize(this->width(),this->window()->height());
        }
        //设置展开状态和动画运行状态,根据展开状态发射相应信号
        this->isExpanded = !this->isExpanded;
        this->isAnimationRunning = false;
        if(this->isExpanded)
            emit this->expandFinished();
        else
            emit this->shrinkFinished();
        emit this->finished(); });

    // 动画起始高度和父窗口高度一样,宽度为初始宽度,之后不再更改宽度
    this->initialSize.rwidth() = initialWidth;
    this->initialSize.rheight() = this->window()->height();

    // 动画结束高度和动画起始高度始终相等,动画结束宽度为动画起始宽度加上展开增加宽度
    this->endSize.rwidth() = this->initialSize.rwidth() + 270;
    this->endSize.rheight() = this->initialSize.rheight();

    this->resize(initialWidth, this->window()->height());
}

Sidebar::~Sidebar()
{
    this->animation->deleteLater();
}

void Sidebar::autoExpand()
{
    this->expand();
    this->shrink();
}

void Sidebar::expand()
{
    // 当前不是展开状态
    if (!this->isExpanded && !this->isAnimationRunning)
    {
        this->animation->setDirection(QAbstractAnimation::Direction::Forward); // 正向
        this->animation->start();
        this->isAnimationRunning = true;
        emit this->expandStart();
    }
}

void Sidebar::shrink()
{
    if (this->isExpanded && !this->isAnimationRunning)
    {
        this->animation->setDirection(QAbstractAnimation::Direction::Backward); // 逆向
        this->animation->start();
        this->isAnimationRunning = true;
        emit this->shrinkStart();
    }
}

void Sidebar::addItem(QLayoutItem *item)
{
    this->verticalLayout->addItem(item);
}

void Sidebar::setIncreasedWidth(quint32 increasedWidth)
{
    // increasedWidth - (this->endSize.rwidth() - this->initialSize.rwidth());
    this->endSize.rwidth() += increasedWidth - this->endSize.rwidth() + this->initialSize.rwidth();
}

void Sidebar::setExpandTime(int ms)
{
    this->animation->setDuration(ms);
}

void Sidebar::setBackgroundBrush(const QBrush &brush)
{
    this->backgroundBrush = brush;
}

void Sidebar::setBorderLinePen(const QPen &pen)
{
    this->borderLinePen = pen;
}

int Sidebar::childrenCumulativeHeight()
{
    int cumulativeHeight = 0;                                                                  // 累计高度

#if (QT_VERSION >= QT_VERSION_CHECK(6,3,0))
    auto children(this->findChildren<QWidget *>(Qt::FindChildOption::FindDirectChildrenOnly)); // 查找直接子控件
#else
    auto children(this->findChildren<QWidget *>(QRegularExpression(QString(R"([\s\S]+)")),Qt::FindChildOption::FindDirectChildrenOnly));
#endif
    for (auto &child : children)
        cumulativeHeight += child->height(); // 累加直接子控件高度

    // 累加布局边距和控件间距
    cumulativeHeight += layout()->contentsMargins().top();
    cumulativeHeight += layout()->contentsMargins().bottom();
    cumulativeHeight += layout()->spacing() * (children.count() - 1);

    return cumulativeHeight;
}

void Sidebar::addWidget(QWidget *btn)
{
    this->verticalLayout->addWidget(btn);
}

void Sidebar::resizeEvent(QResizeEvent *)
{
    // 动画未运行时改变,父窗口高度改变,需重设动画起始和结束高度
    // 这里不判断高度是否相等(性能差不多?? 但是需要增加额外的初始化代码)
    if (!this->isAnimationRunning) //&& this->height()!=this->window()->height()
    {
        this->initialSize.rheight() = this->height();
        this->endSize.rheight() = this->initialSize.rheight();
        this->animation->setStartValue(this->initialSize);
        this->animation->setEndValue(this->endSize);
    }
}

void Sidebar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(this->backgroundBrush);
    painter.setPen(Qt::PenStyle::NoPen);
    painter.drawRect(this->rect());
    painter.setPen(this->borderLinePen);
    painter.drawLine(this->width(), 0, this->width(), this->height());
}
