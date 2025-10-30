//
// Created by TX on 2025/10/29.
//

#include "IntroductionCard.h"

#include <QDesktopServices>
#include <QMouseEvent>
#include <QStyleOption>

#include "ControlState.h"
#include "Palette.h"
#include "SvgWidget.h"
#include "TextLabel.h"
#include "ThemeModeController.h"


namespace fancy
{
    IntroductionCard::IntroductionCard(QWidget *parent, QWidget *blurredObj) :
        BlurCard(parent, blurredObj),
        _mainIcon(new SvgWidget(this)),
        _shareIcon(new SvgWidget(IconPark::Share, this)),
        _mainText(new TextLabel(this)),
        _subText(new TextLabel(this)),
        _state(VisualState::Normal)
    {
        setFixedSize(235, 175);
        QRect icon{25, 25, 50, 50};
        QRect shardIcon{210, 150, 16, 16};
        QRect text_1{15, 80, 220, 30};
        QRect text_2{15, 100, 220, 60};
        QFont boldFont;
        boldFont.setPointSize(11);
        boldFont.setBold(true);
        QFont font;
        font.setPointSize(9);

        _mainIcon->setGeometry(icon);
        _shareIcon->setGeometry(shardIcon);
        _mainText->setGeometry(text_1);
        _subText->setGeometry(text_2);
        _mainText->setFont(boldFont);
        _subText->setFont(font);
        setMouseTracking(true);

        connect(this, &QPushButton::clicked, this, [this] {if (!_url.isEmpty())QDesktopServices::openUrl(_url);});
        connect(this, &QPushButton::pressed, this, [this] {_state = VisualState::Pressed;update();});
    }

    void IntroductionCard::setIcon(const IconId &id)
    {
        _mainIcon->reload(id);
    }

    void IntroductionCard::setMainText(const QString &text)
    {
        _mainText->setText(text);
    }

    void IntroductionCard::setSubText(const QString &text)
    {
        _subText->setText(text);
    }

    void IntroductionCard::mouseReleaseEvent(QMouseEvent *event)
    {
        BlurCard::mouseReleaseEvent(event);
        if (event->button() == Qt::LeftButton)
        {
            if (rect().contains(event->pos()))
                _state = VisualState::Hover;
            else
                _state = VisualState::Normal;
            update();
        }
    }

    void IntroductionCard::leaveEvent(QEvent *event)
    {
        BlurCard::leaveEvent(event);
        _state = VisualState::Normal;
        update();
    }

    void IntroductionCard::enterEvent(QEnterEvent *event)
    {
        BlurCard::enterEvent(event);
        _state = VisualState::Hover;
        update();
    }

    void IntroductionCard::paintEvent(QPaintEvent *event)
    {
        BlurCard::paintEvent(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::SmoothPixmapTransform);
        bool isLight = ThemeModeController::controller().isAppLight();
        QColor mask = isLight ? QColor(208, 217, 228, 200) : QColor(2, 11, 32, 150);
        QColor mixColor = isLight ? QColor(255, 255, 255, 200) : QColor(0, 0, 0, 50);
        switch (_state)
        {
            case VisualState::Normal :
                mask = Palette::mix(mask, mixColor);
                break;
            case VisualState::Pressed :
                mask = Palette::mix(mask, mixColor, 0.3f);
                break;
            case VisualState::Hover :
                mask = Palette::mix(mask, mixColor, 0.7f);
                break;
            default :
                break;
        }
        painter.setPen(Qt::NoPen);
        painter.setBrush(mask);
        painter.drawRoundedRect(rect(), radius(), radius());
    }
}
