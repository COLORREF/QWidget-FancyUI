//
// Created by TX on 2025/10/30.
//

#include "ExampleNavigationCard.h"

#include "CenteredImageWidget.h"
#include "Defs.hpp"
#include "Palette.h"
#include "SvgWidget.h"
#include "TextLabel.h"
#include "ThemeModeController.h"

namespace fancy
{
    ExampleNavigationCard::ExampleNavigationCard(QWidget *parent, QWidget *blurredObj) :
        IntroductionCard(parent, blurredObj),
        _mainIconPixmap(new QLabel(this))
    {
        setFixedSize(300, 100);
        setRadius(10);
        _mainIcon->setGeometry(15, 20, 35, 35);
        _mainIconPixmap->setGeometry(15, 20, 35, 35);
        _mainIconPixmap->setScaledContents(true);

        QFont fontMain;
        fontMain.setPointSize(10);
        fontMain.setBold(true);
        _mainText->setFont(fontMain);
        _mainText->setGeometry(65, 20, 250, 15);

        QFont fontSub;
        fontSub.setPointSizeF(8.5);
        _subText->setFont(fontSub);
        _subText->setGeometry(65, 40, 250, 60);
        _subText->setAlignment(Qt::AlignTop | Qt::AlignLeft);

        _shareIcon->setGeometry(275, 75, 20, 20);
        _shareIcon->reload(IconPark::Link);
    }

    void ExampleNavigationCard::setIcon(const QPixmap &pixmap)
    {
        _mainIconPixmap->setPixmap(pixmap);
    }

    void ExampleNavigationCard::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHints(QPainter::RenderHint::Antialiasing | QPainter::RenderHint::SmoothPixmapTransform);
        QPainterPath path;
        path.addRoundedRect(rect(), _radius, _radius);
        painter.setClipPath(path);
        bool isLight = ThemeModeController::controller().isAppLight();
        QColor mask;
        if (_pixmap.isNull())
        {
            switch (_state)
            {
                case VisualState::Normal :
                    mask = isLight ? QColor(251, 251, 251) : QColor(52, 52, 52);
                    break;
                case VisualState::Pressed :
                    mask = isLight ? QColor(251, 251, 251) : QColor(52, 52, 52);
                    break;
                case VisualState::Hover :
                    mask = isLight ? QColor(253, 253, 253) : QColor(64, 64, 64);
                    break;
                default :
                    break;
            }
            painter.fillPath(path, mask);
            if (_state == VisualState::Hover || _state == VisualState::Pressed)
                painter.setPen(QPen(isLight ? QColor(178, 178, 178) : QColor(75, 75, 75), 0));
            else
                painter.setPen(QPen(isLight ? QColor(234, 234, 234) : QColor(59, 59, 59), 0));
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), _radius, _radius);
        }
        else
        {
            painter.drawPixmap(rect(), _pixmap);
            mask = isLight ? QColor(208, 217, 228, 200) : QColor(2, 11, 32, 150);
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
            painter.fillPath(path, mask);
        }
    }
} // fancy
