//
// Created by TX on 2025/10/13.
//

#include "MSWindow.h"

#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QStackedWidget>
#include <QVariantAnimation>

#include "Defs.h"
#include "MSWidget.h"
#include "Sidebar.h"
#include "SidebarButton.h"
#include "TitleBar.h"

namespace fancy
{
    MSWindow::MSWindow(QWidget *parent) :
        Window(parent),
        _globalLayout(new QHBoxLayout(Window::setupUiWidget())),
        _msClientArea(new MSWidget(Window::setupUiWidget())),
        _clientLayout(new QHBoxLayout(_msClientArea)),
        _sidebar(new Sidebar(Window::setupUiWidget())),
        _stackWidget(new QStackedWidget(_clientArea))
    {
        _titleBar->setFixedHeight(50);
        _globalLayout->setSpacing(0);
        _globalLayout->setContentsMargins(0, 1, 0, 0);
        _globalLayout->addWidget(_sidebar);
        _globalLayout->addWidget(_msClientArea);
        _sidebar->setFixedWidth(75);
        _clientLayout->setSpacing(0);
        _clientLayout->setContentsMargins(0, 0, 0, 0);
        _clientLayout->addWidget(_stackWidget);

        connect(_sidebar, &Sidebar::optionChecked, this, &MSWindow::setCurrentIndex);
    }

    void MSWindow::addWidget(QWidget *widget)
    {
        if (widget)
        {
            _stackWidget->addWidget(widget);
            if (auto *ani = widget->property("geometryAnimation").value<QPropertyAnimation *>(); !ani)
            {
                ani = new QPropertyAnimation(widget, "geometry");
                ani->setDuration(200);
                widget->setProperty("geometryAnimation", QVariant::fromValue(ani));
            }
        }
    }

    void MSWindow::setCurrentIndex(int index)
    {
        if (index == _stackWidget->currentIndex() || index > _stackWidget->count() - 1)
            return;
        _stackWidget->setCurrentIndex(index);

        if (const QWidget *w = _stackWidget->currentWidget())
        {
            if (auto *ani = w->property("geometryAnimation").value<QPropertyAnimation *>())
            {
                const QRect endRect = w->rect();
                if (const auto startRect = QRect({0, 100}, w->size());
                    ani->startValue().toRect() != startRect || ani->endValue().toRect() != endRect)
                {
                    ani->setStartValue(startRect);
                    ani->setEndValue(endRect);
                }
                ani->start();
            }
        }
    }

    void MSWindow::addSidebarOption(SidebarButton *btn)
    {
        btn->setSidebarWidth(65);
        _sidebar->addOption(btn);
    }
} // fancy
