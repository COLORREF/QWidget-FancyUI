//
// Created by TX on 2025/10/7.
//

#include "SidebarButton.h"

#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>

#include "Defs.hpp"
#include "Palette.h"
#include "Sidebar.h"
#include "SvgWidget.h"
#include "TextLabel.h"
#include "TextLabelStyle.h"

namespace fancy
{
    SidebarButton::SidebarButton(QWidget *parent, int sidebarWidth) :
        TransparentButton(parent),
        _svgWidget(new SvgWidget(this)),
        _imageWidget(new QLabel(this)),
        _textLabel(new QLabel(this)),
        _topToMiddle(new QVariantAnimation(this)),
        _middleToBottom(new QVariantAnimation(this)),
        _drawIndicator(true),
        _adjustIconCoordinates(true)
    {
        setCheckable(true);
        setRadius(5);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
        setSizePolicy(sizePolicy);
        setSidebarWidth(sidebarWidth);
        QFont font = _textLabel->font();
        font.setPixelSize(13);
        setFont(font);
        connect(_topToMiddle, &QVariantAnimation::valueChanged, this, &SidebarButton::setIndicator);
        connect(_middleToBottom, &QVariantAnimation::valueChanged, this, &SidebarButton::setIndicator);
        connect(this, &SidebarButton::toggled, this, &SidebarButton::correctIconCoordinates);
    }

    SidebarButton::SidebarButton(const QString &text, QWidget *parent, int sidebarWidth) :
        SidebarButton(parent, sidebarWidth)
    {
        setText(text);
    }

    SidebarButton::SidebarButton(IconId id, const QString &text, QWidget *parent, int sidebarWidth) :
        SidebarButton(text, parent, sidebarWidth)
    {
        setIcon(id);
    }

    void SidebarButton::setIcon(const QPixmap &icon)
    {
        _svgWidget->clear();
        _imageWidget->setPixmap(icon);
    }

    void SidebarButton::setIcon(const QPicture &icon)
    {
        _svgWidget->clear();
        _imageWidget->setPicture(icon);
    }

    void SidebarButton::setIcon(IconId id)
    {
        _imageWidget->clear();
        _svgWidget->reload(id);
    }

    void SidebarButton::setText(const QString &text)
    {
        _textLabel->setText(text);
    }

    void SidebarButton::setFont(const QFont &font)
    {
        _textLabel->setFont(font);
    }

    void SidebarButton::setSidebarWidth(int sidebarWidth)
    {
        setMinimumWidth(sidebarWidth);
        setFixedHeight(sidebarWidth);
        resize(sidebarWidth, sidebarWidth);
        int ilWH = sidebarWidth / 2;
        const int ily = (height() - ilWH) / 2;
        const int ilx = (width() - ilWH) / 2;

        _imageWidget->resize({ilWH, ilWH});
        _imageWidget->move(ilx, ily);
        _svgWidget->resize({ilWH, ilWH});
        _svgWidget->move(ilx, ily);

        const int tlH = ilWH;
        const int tlX = sidebarWidth + ContentsMargins_Right + ContentsMargins_Left;
        const int tlY = (height() - tlH) / 2;
        _textLabel->setStyle(new TextLabelStyle(_textLabel));
        _textLabel->setFixedHeight(tlH);
        _textLabel->setScaledContents(true);
        _textLabel->move(tlX, tlY);

        constexpr int Lx = LINE_WIDTH + LINE_WIDTH;
        const int Ly1 = height() / 4;
        const int Ly2 = Ly1 * 2 + Ly1;
        _indicator = QLine(Lx, Ly1, Lx, Ly2);

        constexpr int x = LINE_WIDTH * 2;
        _upIndicator = QLine(x, 0, x, 0);
        _unIndicator = QLine(x, height(), x, height());

        _topToMiddle->setStartValue(_upIndicator);
        _topToMiddle->setEndValue(_indicator);
        _topToMiddle->setDuration(400);

        _middleToBottom->setStartValue(_indicator);
        _middleToBottom->setEndValue(_unIndicator);
        _middleToBottom->setDuration(400);
        _indicator = _upIndicator;
    }

    void SidebarButton::paintEvent(QPaintEvent *event)
    {
        TransparentButton::paintEvent(event);
        if (_drawIndicator)
        {
            auto &pal = Palette::palette();
            QPainter painter(this);
            painter.setRenderHints(QPainter::RenderHint::Antialiasing);
            QPen pen(pal[ColorRole::AppAccent]);
            pen.setCapStyle(Qt::PenCapStyle::RoundCap);
            pen.setWidth(LINE_WIDTH);
            painter.setPen(pen);
            painter.setBrush(Qt::BrushStyle::NoBrush);
            painter.drawLine(_indicator);
        }
    }

    void SidebarButton::setIndicator(const QVariant &value)
    {
        _indicator = value.toLine();
        update();
    }

    void SidebarButton::correctIconCoordinates(bool checked)
    {
        if (_adjustIconCoordinates)
        {
            const int ily = minimumWidth() / 4;
            const int ilx = checked ? ily + LINE_WIDTH : ily;
            _svgWidget->move(ilx, ily);
            _imageWidget->move(ilx, ily);
        }
    }
}
