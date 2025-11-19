//
// Created by TX on 2025/10/10.
//

#include "Sidebar.h"

#include <QButtonGroup>
#include <QVariantAnimation>
#include <QVBoxLayout>

#include "ScrollArea.h"
#include "ScrollBar.h"
#include "SidebarButton.h"

namespace fancy
{
    Sidebar::Sidebar(QWidget *parent) :
        QWidget(parent),
        _verticalLayout(new QVBoxLayout(this)),
        _btnGroup(new QButtonGroup(this)),
        _verticalSpacer(new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding)),
        _scrollArea(new ScrollArea(this)),
        _scrollContentWidget(new QWidget(_scrollArea)),
        _scrollContentWidgetVerticalLayout(new QVBoxLayout(_scrollContentWidget)),
        _expandAndRetractAni(new QVariantAnimation(this)),
        _isExpanded(false),
        _level(1)
    {
        _verticalLayout->setSpacing(Sidebar_Spacing);
        _verticalLayout->setContentsMargins(Sidebar_ContentsMargins_Left, Sidebar_ContentsMargins_Top, Sidebar_ContentsMargins_Right, Sidebar_ContentsMargins_Bottom);
        _btnGroup->setExclusive(true); //启用互斥
        _scrollArea->setWidgetResizable(true);
        _scrollArea->setWidget(_scrollContentWidget);
        _scrollContentWidgetVerticalLayout->setSpacing(Sidebar_Spacing);
        _scrollContentWidgetVerticalLayout->setContentsMargins(0, 0, 0, 0);

        // 惯性平滑滚动
        // QScroller::grabGesture(_scrollArea, QScroller::LeftMouseButtonGesture);
        // QScroller *scroller = QScroller::scroller(_scrollArea);
        // QScrollerProperties scrollerProperties;
        // scrollerProperties.setScrollMetric(QScrollerProperties::ScrollMetric::ScrollingCurve, QEasingCurve::OutQuad);
        // scrollerProperties.setScrollMetric(QScrollerProperties::ScrollMetric::DecelerationFactor, 0.9); // 减小减速，增加惯性效果
        // scroller->setScrollerProperties(scrollerProperties);

        _scrollContentWidgetVerticalLayout->addItem(_verticalSpacer);
        _scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        _verticalLayout->addWidget(_scrollArea);
        _checked.enqueue(0);

        setFixedWidth(50);
        _expandAndRetractAni->setDuration(200);
        _expandAndRetractAni->setStartValue(50);
        _expandAndRetractAni->setEndValue(300);

        connect(_expandAndRetractAni, &QVariantAnimation::valueChanged, this, &Sidebar::expandOrRetract);
        connect(_expandAndRetractAni, &QVariantAnimation::finished, this, &Sidebar::expandOrRetractFinished);
        connect(_btnGroup, &QButtonGroup::idClicked, this, [this](int id) { emit optionChecked(id); });
        connect(_btnGroup, &QButtonGroup::idToggled, this, &Sidebar::startIndicatorAnimation);
    }

    void Sidebar::addOption(SidebarButton *option)
    {
        int id = static_cast<int>(_btnGroup->buttons().size());
        _btnGroup->addButton(option, id);
        int index = _scrollContentWidgetVerticalLayout->count() - 1;
        _scrollContentWidgetVerticalLayout->insertWidget(index, option, 0, Qt::Alignment());
        if (id == 0)
            option->setChecked(true);
    }

    void Sidebar::expand()
    {
        if (_expandAndRetractAni->state() != QVariantAnimation::State::Running && !_isExpanded)
        {
            _expandAndRetractAni->setDirection(QVariantAnimation::Direction::Forward);
            _expandAndRetractAni->start();
            emit startExpand();
        }
    }

    void Sidebar::retract()
    {
        if (_expandAndRetractAni->state() != QVariantAnimation::State::Running && _isExpanded)
        {
            _expandAndRetractAni->setDirection(QVariantAnimation::Direction::Backward);
            _expandAndRetractAni->start();
            emit startRetract();
        }
    }

    void Sidebar::autoExpandRetract()
    {
        _isExpanded ? retract() : expand();
    }

    void Sidebar::setMinimumWidth(int width)
    {
        setFixedWidth(width);
    }

    void Sidebar::setMaximumWidth(int width)
    {
        _expandAndRetractAni->setEndValue(width);
    }

    void Sidebar::setFixedWidth(int width)
    {
        QWidget::setFixedWidth(width);
        _expandAndRetractAni->setStartValue(width);
    }

    int Sidebar::maximumWidth()
    {
        return _expandAndRetractAni->endValue().toInt();
    }

    int Sidebar::minimumWidth()
    {
        return _expandAndRetractAni->startValue().toInt();
    }

    void Sidebar::addOptionToGroup(SidebarButton *option)
    {
        _btnGroup->addButton(option, static_cast<int>(_btnGroup->buttons().size()));
    }

    void Sidebar::removeOptionFromGroup(SidebarButton *option)
    {
        _btnGroup->removeButton(option);
    }

    void Sidebar::addOptionToLayout(SidebarButton *option)
    {
        _scrollContentWidgetVerticalLayout->insertWidget(_scrollContentWidgetVerticalLayout->count() - 1, option, 0, Qt::Alignment());
    }

    void Sidebar::setExpandRetractDuration(int msecs)
    {
        _expandAndRetractAni->setDuration(msecs);
    }

    void Sidebar::hideScrollbar()
    {
        _scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    }

    void Sidebar::restoreScrollBarPolicy()
    {
        _scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
    }

    int Sidebar::buttonCount()
    {
        return static_cast<int>(_btnGroup->buttons().size());
    }

    void Sidebar::startIndicatorAnimation(int id, bool checked)
    {
        if (checked)
        {
            int leaveId = this->_checked.dequeue();
            _checked.enqueue(id);
            auto *enter = qobject_cast<SidebarButton *>(_btnGroup->button(id));
            auto *leave = qobject_cast<SidebarButton *>(_btnGroup->button(leaveId));
            if (id >= leaveId)
            {
                leave->_middleToBottom->setEasingCurve(QEasingCurve::InQuart);
                leave->_middleToBottom->setDirection(QVariantAnimation::Direction::Forward);
                leave->_middleToBottom->start();
                enter->_indicator = enter->_upIndicator;
                enter->_topToMiddle->setEasingCurve(QEasingCurve::InQuart);
                enter->_topToMiddle->setDirection(QVariantAnimation::Direction::Forward);
                enter->_topToMiddle->start();
            }
            else
            {
                leave->_topToMiddle->setEasingCurve(QEasingCurve::OutQuart);
                leave->_topToMiddle->setDirection(QVariantAnimation::Direction::Backward);
                leave->_topToMiddle->start();
                enter->_indicator = enter->_unIndicator;
                enter->_middleToBottom->setEasingCurve(QEasingCurve::OutQuart);
                enter->_middleToBottom->setDirection(QVariantAnimation::Direction::Backward);
                enter->_middleToBottom->start();
            }
        }
    }

    void Sidebar::expandOrRetract(const QVariant &value)
    {
        QWidget::setFixedWidth(value.value<int>());
    }

    void Sidebar::expandOrRetractFinished()
    {
        _isExpanded = !_isExpanded;
        emit expandStateChange(_isExpanded);
    }
} // fancy
