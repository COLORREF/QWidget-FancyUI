//
// Created by TX on 2025/8/31.
//

#ifndef QWIDGET_FANCYUI_RADIOBUTTON_H
#define QWIDGET_FANCYUI_RADIOBUTTON_H
#include <QRadioButton>

namespace fancy
{
    class RadioButton : public QRadioButton
    {
        Q_OBJECT

    public:
        explicit RadioButton(QWidget *parent = nullptr);

        explicit RadioButton(const QString &text, QWidget *parent = nullptr);
    };
} // fancy

#endif //QWIDGET_FANCYUI_RADIOBUTTON_H
