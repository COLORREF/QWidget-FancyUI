//
// Created by TX on 2025/9/19.
//

#include "TitleBar.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSpacerItem>
#include <QStyle>
#include <QWindow>

#include "CloseButton.h"
#include "MaximizeButton.h"
#include "MinimizeButton.h"
#include "TextLabel.h"

namespace fancy
{
    TitleBar::TitleBar(QWidget *parent) :
        QWidget(parent),
        _doubleClickMaximize(true),
        _horizontalLayout(new QHBoxLayout(this)),
        _closeButton(new CloseButton(this)),
        _maximizeButton(new MaximizeButton(this)),
        _minimizeButton(new MinimizeButton(this)),
        _horizontalSpacer(new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum)),
        _textLabel(new TextLabel(this)),
        _iconLabel(new QLabel(this))
    {
        setMinimumHeight(32);
        setLayout(_horizontalLayout);
        QSizePolicy size_policy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        size_policy.setHorizontalStretch(0);
        size_policy.setVerticalStretch(0);
        size_policy.setHeightForWidth(sizePolicy().hasHeightForWidth());
        setSizePolicy(size_policy);

        _textLabel->setFont(QFont(_textLabel->font().family(), 9));
        _iconLabel->setFixedSize(25, 20);
        _iconLabel->setScaledContents(true);
        _iconLabel->setContentsMargins(0, 0, 5, 0);

        _horizontalLayout->setSpacing(0);
        _horizontalLayout->setContentsMargins(8, 0, 0, 0);
        _horizontalLayout->setAlignment(Qt::AlignmentFlag::AlignTop);

        _horizontalLayout->addItem(_horizontalSpacer);
        _horizontalLayout->addWidget(_closeButton);
        _horizontalLayout->insertWidget(1, _maximizeButton);
        _horizontalLayout->insertWidget(1, _minimizeButton);
        _horizontalLayout->insertWidget(0, _textLabel);
        _horizontalLayout->insertWidget(0, _iconLabel);

        // 设置默认图标和标题
        _iconLabel->setPixmap(style()->standardIcon(QStyle::StandardPixmap::SP_TitleBarMenuButton).pixmap(20, 20));
        _textLabel->setText(QApplication::applicationName());
    }

    void TitleBar::setIcon(const QPixmap &icon)
    {
        _iconLabel->setPixmap(icon);
    }

    void TitleBar::setTitleText(const QString &text)
    {
        _textLabel->setText(text);
    }

    // void TitleBar::setTitleTextColor(const QColor &color)
    // {
    //     _textLabel->setTextColor(color);
    // }

    void TitleBar::setTitleTextFont(const QFont &font)
    {
        _textLabel->setFont(font);
    }

    void TitleBar::removeIconLabel()
    {
        _horizontalLayout->removeWidget(_iconLabel);
        _iconLabel->hide();
    }

    void TitleBar::removeTextLabel()
    {
        _horizontalLayout->removeWidget(_textLabel);
        _textLabel->hide();
    }

    void TitleBar::removeMinimizeButton()
    {
        _horizontalLayout->removeWidget(_minimizeButton);
        _minimizeButton->hide();
    }

    void TitleBar::removeMaximizeButton()
    {
        _horizontalLayout->removeWidget(_maximizeButton);
        _maximizeButton->hide();
    }

    void TitleBar::removeCloseButton()
    {
        _horizontalLayout->removeWidget(_closeButton);
        _closeButton->hide();
    }

    void TitleBar::setDoubleClickMaximize(bool allow)
    {
        _doubleClickMaximize = allow;
    }

    void TitleBar::resetTitleBtnFixedSize(const QSize &size)
    {
        _minimizeButton->setFixedSize(size);
        _maximizeButton->setFixedSize(size);
        _closeButton->setFixedSize(size);
    }

    QSize TitleBar::titleBtnFixedSize() const
    {
        return _minimizeButton->size();
    }

    void TitleBar::insertWidget(int index, QWidget *widget, int stretch, Qt::Alignment alignment)
    {
        _horizontalLayout->insertWidget(index, widget, stretch, alignment);
    }

    void TitleBar::mousePressEvent(QMouseEvent *event)
    {
        Q_UNUSED(event)
        window()->windowHandle()->startSystemMove();
    }

    void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::MouseButton::LeftButton && _doubleClickMaximize)
        {
            // this->_maximizeButton->click();
            if (window()->isMaximized())
                window()->showNormal();
                // ::ShowWindow((HWND) (this->window()->winId()), SW_RESTORE); //还原窗口
            else
                window()->showMaximized();
            // ::ShowWindow((HWND) (this->window()->winId()), SW_MAXIMIZE); //最大化窗口
        }
    }

    void TitleBar::setTransparency(bool set) const
    {
        _minimizeButton->setClearBeforeNewPaint(set);
        _maximizeButton->setClearBeforeNewPaint(set);
        _closeButton->setClearBeforeNewPaint(set);
        _textLabel->setClearBeforeNewPaint(set);
    }
}
