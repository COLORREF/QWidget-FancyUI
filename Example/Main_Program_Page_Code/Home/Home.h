//
// Created by TX on 2025/11/18.
//

#ifndef QWIDGET_FANCYUI_HOME_H
#define QWIDGET_FANCYUI_HOME_H
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace Ui
{
    class Home;
}

QT_END_NAMESPACE

namespace fancy
{
    class IntroductionCard;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent);

    ~HomePage() override;

private:
    static constexpr int FastBlurMergeCount = 2;

    void initializeUi();

private slots:
    void onFastPixmapUpdate();

    void updateSmoothBlur();

    void onThemeChanged();

private:
    Ui::Home *_uiHome;
    fancy::IntroductionCard *_githubCard;
    fancy::IntroductionCard *_bilibiliCard;
    int _fastBlurSignalCount;
};


#endif //QWIDGET_FANCYUI_HOME_H
