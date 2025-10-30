//
// Created by TX on 2025/9/17.
//

#include "SvgWidget.h"
#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 3000
#include <magic_enum.hpp>
#include "IconEngine.h"
#include "Palette.h"
#include "ThemeModeController.h"

namespace fancy
{
    SvgWidget::SvgWidget(QWidget *parent) :
        QSvgWidget(parent)
    {
        _cache.first = QByteArray(R"(<svg xmlns="http://www.w3.org/2000/svg" width="1" height="1"></svg>)");
        _cache.second = _cache.first;
        connect(&Palette::palette(), &Palette::appThemeChange, this, &SvgWidget::load);
    }

    SvgWidget::SvgWidget(const QString &file, QWidget *parent) :
        QSvgWidget(file, parent)
    {
        _cache.first = QByteArray(R"(<svg xmlns="http://www.w3.org/2000/svg" width="1" height="1"></svg>)");
        _cache.second = _cache.first;
        connect(&Palette::palette(), &Palette::appThemeChange, this, &SvgWidget::load);
    }

    SvgWidget::SvgWidget(const IconId &id, QWidget *parent) :
        QSvgWidget(parent)
    {
        _cache.first = IconEngine::iconData(id);
        _cache.second = IconEngine::singleRender(id, Qt::GlobalColor::white);
        load();
        connect(&Palette::palette(), &Palette::appThemeChange, this, &SvgWidget::load);
    }

    void SvgWidget::clear()
    {
        QSvgWidget::load(QByteArray(R"(<svg xmlns="http://www.w3.org/2000/svg" width="1" height="1"></svg>)"));
    }

    QPair<QByteArray, QByteArray> &SvgWidget::cache()
    {
        return _cache;
    }

    void SvgWidget::reload(const IconId &id)
    {
        _cache.first = IconEngine::iconData(id);
        _cache.second = IconEngine::singleRender(id, Qt::GlobalColor::white);
        load();
    }

    void SvgWidget::load()
    {
        ThemeModeController::controller().isAppLight() ? QSvgWidget::load(_cache.first) : QSvgWidget::load(_cache.second);
    }
} // fancy
