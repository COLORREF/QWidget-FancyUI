//
// Created by TX on 2025/9/19.
//

#ifndef QWIDGET_FANCYUI_TEXTLABEL_H
#define QWIDGET_FANCYUI_TEXTLABEL_H
#include <QLabel>

namespace fancy
{
    class TextLabel : public QLabel
    {
        Q_OBJECT

    public:
        explicit TextLabel(QWidget *parent);
        explicit TextLabel(const QString &text, QWidget *parent=nullptr);

        void setClearBeforeNewPaint(bool before) { _clearBeforeNewPaint = before; }
        bool clearBeforeNewPaint() const { return _clearBeforeNewPaint; }

    protected:
        void paintEvent(QPaintEvent *) override;

    private:
        bool _clearBeforeNewPaint;
    };
} // fancy

#endif //QWIDGET_FANCYUI_TEXTLABEL_H
