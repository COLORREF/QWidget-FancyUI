//
// Created by TX on 2025/11/10.
//

#ifndef QWIDGET_FANCYUI_TEXTEDIT_H
#define QWIDGET_FANCYUI_TEXTEDIT_H
#include <QTextEdit>

namespace fancy
{
    class TextEdit : public QTextEdit
    {
        Q_OBJECT

    public:
        explicit TextEdit(QWidget *parent = nullptr);

        explicit TextEdit(const QString &text, QWidget *parent = nullptr);

    protected slots:
        virtual void onAppThemeChange();

    private:
        void init();
    };
} // fancy

#endif //QWIDGET_FANCYUI_TEXTEDIT_H
