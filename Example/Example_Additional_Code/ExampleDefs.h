//
// Created by TX on 2025/11/18.
//

#ifndef QWIDGET_FANCYUI_EXAMPLEDEFS_H
#define QWIDGET_FANCYUI_EXAMPLEDEFS_H
#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>
#include <QMetaEnum>
#include <QString>

#include "Defs.h"


inline QString loadExampleCode(const QString &widgetName, fancy::Theme theme)
{
    const QString theme_s = QMetaEnum::fromType<fancy::Theme>().valueToKey(static_cast<int>(theme));
    QFile file(":/ExampleSourceCode.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};
    const QByteArray data = file.readAll();
    file.close();
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject())
        return {};
    QJsonObject root = doc.object();
    if (!root.contains(widgetName))
        return {};
    QJsonObject widgetObj = root[widgetName].toObject();
    if (!widgetObj.contains(theme_s))
        return {};
    return widgetObj[theme_s].toString();
}


#endif //QWIDGET_FANCYUI_EXAMPLEDEFS_H
