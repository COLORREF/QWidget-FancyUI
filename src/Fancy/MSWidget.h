//
// Created by TX on 2025/10/22.
//

#ifndef QWIDGET_FANCYUI_MSWIDGET_H
#define QWIDGET_FANCYUI_MSWIDGET_H
#include <QWidget>
class QVariantAnimation;

namespace fancy
{
    enum class Theme;

    class MSWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit MSWidget(QWidget *parent = nullptr);

    protected:
        void paintEvent(QPaintEvent *event) override;

    private :
        void onThemeChanged();

        void updateBkgColor(const QVariant &value);

        QVariantAnimation *_animation;
        QColor _color;
    };
} // fancy

#endif //QWIDGET_FANCYUI_MSWIDGET_H
