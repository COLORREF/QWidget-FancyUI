//
// Created by TX on 2025/10/30.
//

#ifndef QWIDGET_FANCYUI_EXAMPLENAVIGATIONCARD_H
#define QWIDGET_FANCYUI_EXAMPLENAVIGATIONCARD_H
#include "IntroductionCard.h"

class QLabel;

namespace fancy
{
    class CenteredImageWidget;

    class ExampleNavigationCard : public IntroductionCard
    {
    public:
        explicit ExampleNavigationCard(QWidget *parent, QWidget *blurredObj = nullptr);

        void setIcon(const QPixmap &pixmap);

    protected:
        void paintEvent(QPaintEvent *event) override;

        QLabel *_mainIconPixmap;
    };
} // fancy

#endif //QWIDGET_FANCYUI_EXAMPLENAVIGATIONCARD_H
