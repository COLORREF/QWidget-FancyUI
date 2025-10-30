//
// Created by TX on 2025/9/7.
//

#ifndef QWIDGET_FANCYUI_FLOWLAYOUT_H
#define QWIDGET_FANCYUI_FLOWLAYOUT_H

#include <QLayout>
#include <QPropertyAnimation>
#include <QVariant>
#include <QWidget>

namespace fancy
{
    class FlowLayout : public QLayout
    {
        Q_OBJECT

    public:
        explicit FlowLayout(QWidget *parent = nullptr, bool animation = true);


        void addItem(QLayoutItem *item) override;

        void addWidget(QWidget *widget);

        [[nodiscard]] int count() const override;

        [[nodiscard]] QLayoutItem *itemAt(int index) const override;

        QLayoutItem *takeAt(int index) override;

        void setAnimationProperty(int msecs, const QEasingCurve &easing);

        void removeWidget(QWidget *widget);

        void clearWidget();

        [[nodiscard]] Qt::Orientations expandingDirections() const override;

        [[nodiscard]] bool hasHeightForWidth() const override;

        [[nodiscard]] int heightForWidth(int width) const override;

        void setGeometry(const QRect &rect) override;

        [[nodiscard]] QSize sizeHint() const override;

        [[nodiscard]] QSize minimumSize() const override;

        void setVerticalSpacing(int spacing);

        [[nodiscard]] int verticalSpacing() const;

        void setHorizontalSpacing(int spacing);

        [[nodiscard]] int horizontalSpacing() const;

    private:
        int doLayout(const QRect &rect, bool count) const;

        bool _enableAnimation;
        int _duration;
        int _verticalSpacing;
        int _horizontalSpacing;
        QList<QLayoutItem *> _items;
        QMap<QWidget *, QPropertyAnimation *> _widgetAnis;
        QEasingCurve _easing; //动画曲线
    };
} // fancy

#endif //QWIDGET_FANCYUI_FLOWLAYOUT_H
