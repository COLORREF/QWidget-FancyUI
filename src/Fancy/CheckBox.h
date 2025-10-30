//
// Created by TX on 2025/8/30.
//

#ifndef QWIDGET_FANCYUI_CHECKBOX_H
#define QWIDGET_FANCYUI_CHECKBOX_H
#include <QCheckBox>

namespace fancy
{
    class CheckBox : public QCheckBox
    {
        Q_OBJECT

    public:
        explicit CheckBox(QWidget *parent = nullptr, bool smallIndicatorSize = false);

        explicit CheckBox(const QString &text, QWidget *parent = nullptr, bool smallIndicatorSize = false);
    };
} // fancy

#endif //QWIDGET_FANCYUI_CHECKBOX_H
