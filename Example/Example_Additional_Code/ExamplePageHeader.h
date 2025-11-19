//
// Created by TX on 2025/11/17.
//

#ifndef QWIDGET_FANCYUI_EXAMPLEPAGEHEADER_H
#define QWIDGET_FANCYUI_EXAMPLEPAGEHEADER_H
#include <QWidget>

class QLabel;

namespace fancy
{
    class TextLabel;
    class PushButton;
}

class ExamplePageHeader : public QWidget
{
    Q_OBJECT
    friend class BasicInput;
    friend class IconPage;
    friend class PalettePage;

public:
    explicit ExamplePageHeader(QWidget *parent);

    void setMainText(const QString &text);

    void setSubText(const QString &text);

private:
    fancy::TextLabel *_mainTitle;
    QLabel *_subTitle;
    fancy::PushButton *_hyperlinkDocument;
    fancy::PushButton *_hyperlinkGithub;
    fancy::PushButton *_theme;
    fancy::PushButton *_supportAuthor;
};


#endif //QWIDGET_FANCYUI_EXAMPLEPAGEHEADER_H
