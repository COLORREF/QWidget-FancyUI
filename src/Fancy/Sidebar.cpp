//
// Created by TX on 2025/10/10.
//

#include "Sidebar.h"

#include <QBoxLayout>
#include <QButtonGroup>
#include <QEasingCurve>
#include <QVariantAnimation>

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
        _isExpanded(false)
    {
        _verticalLayout->setSpacing(2);
        _verticalLayout->setContentsMargins(ContentsMargins_Left, ContentsMargins_Top, ContentsMargins_Right, ContentsMargins_Bottom);
        _btnGroup->setExclusive(true); //启用互斥
        _scrollArea->setWidgetResizable(true);
        _scrollArea->setWidget(_scrollContentWidget);
        _scrollContentWidgetVerticalLayout->setSpacing(2);
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
        _scrollArea->setVerticalScrollBar(new ScrollBar(Qt::Orientation::Vertical, _scrollArea));
        _verticalLayout->addWidget(_scrollArea);
        _checked.enqueue(0);

        setFixedWidth(50);
        _expandAndRetractAni->setDuration(200);
        _expandAndRetractAni->setStartValue(50);
        _expandAndRetractAni->setEndValue(300);

        connect(_expandAndRetractAni, &QVariantAnimation::valueChanged, this, &Sidebar::expandOrRetract);
        connect(_expandAndRetractAni, &QVariantAnimation::finished, this, [this] { _isExpanded = !_isExpanded; });
        connect(_btnGroup, &QButtonGroup::idClicked, this, [this](int id) { emit optionChecked(id); });
        connect(_btnGroup, &QButtonGroup::idToggled, this, &Sidebar::startIndicatorAnimation);
    }

    void Sidebar::addOption(SidebarButton *option)
    {
        int id = static_cast<int>(_btnGroup->buttons().size());
        _btnGroup->addButton(option, id);
        _scrollContentWidgetVerticalLayout->insertWidget(id, option, 0, Qt::Alignment());
        if (id == 0)
            option->setChecked(true);
    }

    void Sidebar::expand()
    {
        if (_expandAndRetractAni->state() != QVariantAnimation::State::Running && !_isExpanded)
        {
            _expandAndRetractAni->setDirection(QVariantAnimation::Direction::Forward);
            _expandAndRetractAni->start();
        }
    }

    void Sidebar::retract()
    {
        if (_expandAndRetractAni->state() != QVariantAnimation::State::Running && _isExpanded)
        {
            _expandAndRetractAni->setDirection(QVariantAnimation::Direction::Backward);
            _expandAndRetractAni->start();
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

    void Sidebar::addOptionToLayout(SidebarButton *option)
    {
        int id = static_cast<int>(_btnGroup->buttons().size());
        _scrollContentWidgetVerticalLayout->insertWidget(id, option, 0, Qt::Alignment());
    }

    void Sidebar::setExpandRetractDuration(int msecs)
    {
        _expandAndRetractAni->setDuration(msecs);
    }

    void Sidebar::startIndicatorAnimation(int id, bool checked)
    {
        if (checked)
        {
            int leaveId = this->_checked.dequeue();
            _checked.enqueue(id);
            auto enter = static_cast<SidebarButton *>(_btnGroup->button(id));
            auto leave = static_cast<SidebarButton *>(_btnGroup->button(leaveId));
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
} // fancy
