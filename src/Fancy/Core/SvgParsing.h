//
// Created by TX on 2025/9/2.
//

#ifndef QWIDGET_FANCYUI_SVGPARSING_H
#define QWIDGET_FANCYUI_SVGPARSING_H
#include <QByteArray>
#include <QDebug>
#include <QDomElement>
#include <QFile>
#include <QString>

namespace fancy
{
    class SvgParsing
    {
    public:
        static QByteArray loadFromFile(const QString &path)
        {
            QFile file(path);
            if (!file.open(QIODevice::OpenModeFlag::ReadOnly | QIODevice::OpenModeFlag::Text))
            {
                qWarning() << "Failed to read SVG file:" << path;
                return {};
            }
            return file.readAll();
        }

        /**
         * @brief 替换SVG文件中指定标签的属性值
         *
         * 读取SVG文件，查找指定标签名（tagName）的元素，并将其属性（attribute）的值替换为新值（newValue）。
         * 如果根元素匹配标签名，则直接修改根元素的属性；否则在文档中搜索所有匹配标签名的元素并修改它们的属性。
         *
         * @param path SVG文件路径
         * @param tagName 要查找的XML标签名（如"svg", "rect", "circle"等）
         * @param attribute 要修改的属性名（如"width", "height", "fill"等）
         * @param newValue 要设置的新属性值
         * @return QByteArray 修改后的SVG内容（原始内容或修改后的内容）
         * @note 使用DOM解析SVG文件，并替换指定标记中所有指定属性的值
         * 例如，将所有“<path>”标签中的所有“fill”属性的值替换为“red”或RGB值，如“RGB（255,0,0）”
         * 此功能的性能可能并不理想，作者也没有对其进行测试。不建议在大型SVG文件上或频繁使用它。
         */
        static QByteArray replaceAttributeValue(const QString &path, const QString &tagName, const QString &attribute, const QString &newValue)
        {
            return replaceAttributeValue(loadFromFile(path), tagName, attribute, newValue);
        }


        /**
        * @brief 替换SVG文件中指定标签的属性值
        * @param data SVG文件二进制数据
        * @param tagName 要查找的XML标签名（如"svg", "rect", "circle"等）
        * @param attribute 要修改的属性名（如"width", "height", "fill"等）
        * @param newValue 要设置的新属性值
        * @return QByteArray 修改后的SVG内容（原始内容或修改后的内容）
        */
        static QByteArray replaceAttributeValue(const QByteArray &data, const QString &tagName, const QString &attribute, const QString &newValue)
        {
            if (data.isEmpty() || data.isNull())
            {
                qWarning() << "SVG text content is empty.";
                return {};
            }

            // 解析SVG内容为DOM文档
            static QDomDocument doc;
            if (!doc.setContent(data))
            {
                qWarning() << "Failed to parse SVG content.";
                return data; // 返回原始内容
            }

            // 获取根元素
            // 根标签是否为指定的标签名
            if (QDomElement root = doc.documentElement(); root.tagName() == tagName)
            {
                // 根元素匹配指定标签名，直接修改根元素的属性
                if (root.hasAttribute(attribute))
                    root.setAttribute(attribute, newValue);
                else
                    qWarning() << "No " << attribute << " attribute found in <svg> elements.";
            }
            // 根元素不匹配指定标签名
            else
            {
                // 查找文档中所有指定标签名的元素
                if (const QDomNodeList elements = root.elementsByTagName(tagName); elements.isEmpty())
                    qWarning() << "No <" + tagName + ">tags found";
                else
                {
                    bool foundAttribute = false; // 是否找到指定属性

                    // 遍历所有找到的元素
                    for (int i = 0; i < elements.count(); ++i)
                    {
                        // 检查是否有指定属性
                        if (QDomElement element = elements.at(i).toElement(); element.hasAttribute(attribute))
                        {
                            // 修改属性值
                            element.setAttribute(attribute, newValue);
                            foundAttribute = true;
                        }
                    }
                    if (!foundAttribute)
                        qWarning() << "No '" + attribute + "' attribute found in any <" + tagName + "> elements.";
                }
            }
            return doc.toByteArray(-1); // 紧凑格式返回
        }


        /**
         * @brief 替换或添加SVG 中指定标签的属性值
         *
         * 该函数解析SVG数据，递归遍历所有元素，针对目标标签修改或添加属性值。
         * 支持精确控制目标标签类型和添加行为，返回修改后的SVG字节数据。
         *
         * @param data 包含SVG内容的字节数组（XML格式）
         * @param attributesMap 属性映射表（键值对），定义需要修改的属性名和新值
         * @param targetTags 需要操作的目标标签列表（如["path", "rect"]）
         * - 空列表表示处理所有标签（默认）
         * @param addIfMissing 是否添加元素上不存在的属性（默认false）
         *
         * @return 修改后的SVG字节数组，解析失败时返回原始数据，空输入返回空数组
         *
         * @note：
         * - 大小写不敏感的目标标签匹配（支持"PATH"、"path"等）
         * - 仅修改目标标签的属性（非目标标签完全跳过）
         * - 紧凑格式输出（无缩进）
         * - 修改状态跟踪（输出警告信息）
         */
        static QByteArray replaceAttributesValues(const QByteArray &data,
                                                  const QMap<QString, QString> &attributesMap,
                                                  const QStringList &targetTags = {},
                                                  const bool addIfMissing = false)
        {
            if (data.isEmpty())
            {
                qWarning() << "SVG text content is empty.";
                return {};
            }

            QDomDocument doc;
            if (!doc.setContent(data))
            {
                qWarning() << "Failed to parse SVG content.";
                return data; // 返回原始内容
            }

            QDomElement root = doc.documentElement();
            bool modified = false;

            // 递归遍历元素
            updateAttributes(root, attributesMap, modified, targetTags, addIfMissing);

            if (!modified)
                qWarning() << "No attributes modified or added in target tags.";

            return doc.toByteArray(-1); // 紧凑格式返回
        }

    private:
        /**
         * @brief 递归更新DOM元素的属性值
         *
         * 该函数递归遍历DOM树，对目标标签执行属性更新/添加操作。采用深度优先遍历策略
         *
         * @param element 当前处理的DOM元素（引用传递，可直接修改）
         * @param attributesMap 属性映射表（键：属性名，值：新属性值）
         * @param modified [out] 修改状态标志（引用传递，用于跟踪全局修改状态）
         * @param targetTags 目标标签列表（空列表表示处理所有标签）
         * @param addIfMissing 是否添加元素上不存在的属性
         *
         * @note
         * 1. 检查当前元素是否为目标标签
         * 2. 是目标标签 → 遍历属性映射表并更新/添加属性
         * 3. 递归处理所有子元素（无论当前元素是否目标标签）
         */
        static void updateAttributes(QDomElement &element,
                                     const QMap<QString, QString> &attributesMap,
                                     bool &modified,
                                     const QStringList &targetTags,
                                     const bool addIfMissing)
        {
            if (targetTags.contains(element.tagName(), Qt::CaseSensitivity::CaseInsensitive))
            {
                for (auto it = attributesMap.constBegin(); it != attributesMap.constEnd(); ++it)
                {
                    const QString &attr = it.key();
                    const QString &newValue = it.value();

                    if (element.hasAttribute(attr))
                    {
                        element.setAttribute(attr, newValue);
                        modified = true;
                    }
                    else if (addIfMissing)
                    {
                        element.setAttribute(attr, newValue);
                        modified = true;
                    }
                }
            }

            // 遍历子节点
            QDomNode child = element.firstChild();
            while (!child.isNull())
            {
                if (child.isElement())
                {
                    QDomElement childElem = child.toElement();
                    updateAttributes(childElem, attributesMap, modified, targetTags, addIfMissing);
                }
                child = child.nextSibling();
            }
        }
    };
} // fancy

#endif //QWIDGET_FANCYUI_SVGPARSING_H
