//
// Created by TX on 2025/11/10.
//

#ifndef QWIDGET_FANCYUI_LINEEDIT_H
#define QWIDGET_FANCYUI_LINEEDIT_H
#include <QLineEdit>

namespace fancy
{
    enum class Theme;

    class LineEdit : public QLineEdit
    {
        Q_OBJECT

    public:
        explicit LineEdit(QWidget *parent = nullptr);

        explicit LineEdit(const QString &, QWidget *parent = nullptr);

    protected slots:
        virtual void onAppThemeChange();
    };
} // fancy

#endif //QWIDGET_FANCYUI_LINEEDIT_H
