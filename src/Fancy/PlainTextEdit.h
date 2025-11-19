//
// Created by TX on 2025/11/10.
//

#ifndef QWIDGET_FANCYUI_PLAINTEXTEDIT_H
#define QWIDGET_FANCYUI_PLAINTEXTEDIT_H
#include <QPlainTextEdit>

namespace fancy
{
    class PlainTextEdit : public QPlainTextEdit
    {
        Q_OBJECT

    public:
        explicit PlainTextEdit(QWidget *parent = nullptr);

        explicit PlainTextEdit(const QString &text, QWidget *parent = nullptr);

    protected slots:
        virtual void onAppThemeChange();

    private:
        void init();
    };
} // fancy

#endif //QWIDGET_FANCYUI_PLAINTEXTEDIT_H
