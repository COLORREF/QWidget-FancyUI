//
// Created by TX on 2025/10/29.
//

#ifndef QWIDGET_FANCYUI_INTRODUCTIONCARD_H
#define QWIDGET_FANCYUI_INTRODUCTIONCARD_H
#include <qurl.h>

#include "BlurCard.h"


namespace fancy
{
    enum class VisualState;
    struct IconId;
    class TextLabel;
    class SvgWidget;

    class IntroductionCard : public BlurCard
    {
    public:
        explicit IntroductionCard(QWidget *parent, QWidget *blurredObj = nullptr);

        void setUrl(const QUrl &url) { _url = url; }

        void setIcon(const IconId &id);

        void setIcon(const QIcon &) = delete;

        void setMainText(const QString &text);

        void setSubText(const QString &text);

    protected:
        void mouseReleaseEvent(QMouseEvent *event) override;

        void leaveEvent(QEvent *event) override;

        void enterEvent(QEnterEvent *event) override;

        void paintEvent(QPaintEvent *event) override;

        QUrl _url;
        SvgWidget *_mainIcon;
        SvgWidget *_shareIcon;
        TextLabel *_mainText;
        TextLabel *_subText;
        VisualState _state;
    };
}


#endif //QWIDGET_FANCYUI_INTRODUCTIONCARD_H
