#include "FancySvg.h"

const QByteArray FancySvg::readSvg(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning()<<"Failed to read SVG file:" << path;
        return QByteArray();
    }
    QByteArray content = file.readAll();
    file.close();
    return content;
}

const QByteArray FancySvg::replaceSvgAttributeValue(const QString &svgPath, const QString &tagName, const QString &attribute, const QString &newValue)
{
    QByteArray svgContent = readSvg(svgPath);

    if (svgContent.isEmpty() || svgContent.isNull())
    {
        qWarning() << "SVG text content is empty.";
        return QByteArray();
    }

    QDomDocument doc;
    if (!doc.setContent(svgContent))
    {
        qWarning() << "Failed to parse SVG content.";
        return svgContent; // 返回原始内容
    }

    QDomElement root = doc.documentElement(); // 获取根标签 <svg>

    // 根标签是否为指定的标签名
    if (root.tagName() == tagName )
    {
        if(root.hasAttribute(attribute))
            root.setAttribute(attribute, newValue);
        else
            qWarning() << "No " << attribute << " attribute found in <svg> elements.";
    }
    else
    {
        bool foundAttribute = false; // 是否找到指定属性
        QDomNodeList elements = root.elementsByTagName(tagName);// 找到所有子标签中指定的标签
        if(elements.isEmpty())
            qWarning() << "No <" + tagName + ">tags found";
        else
        {
            for (int i = 0; i < elements.count(); ++i)
            {
                QDomElement element = elements.at(i).toElement();

                // 检查是否有指定属性
                if (element.hasAttribute(attribute))
                {
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

const QByteArray FancySvg::replaceSvgAttributeValue(const QString &svgPath, const QString &attribute, const QString &newValue)
{
    QString svgString = readSvg(svgPath);
    if (svgString.isEmpty() || svgString.isNull())
    {
        qWarning() << "Failed to read SVG file:" << svgPath;
        return QByteArray();
    }
    static const QRegularExpression regex(R"(\b)"+ attribute + R"(="[^"]*")");
    QRegularExpressionMatch match = regex.match(svgString);// 执行匹配
    if(!match.hasMatch())
    {
        qWarning()<< attribute + "attribute value not found.";
        return QByteArray();
    }
    return svgString.replace(regex,attribute+R"(=")"+newValue+R"(")").toUtf8();
}

const QByteArray FancySvg::replaceSvgBlack(const QString &svgPath, const QString &newColor)
{
    QString svgString = readSvg(svgPath);
    if (svgString.isEmpty() || svgString.isNull())
    {
        qWarning() << "Failed to read SVG file:" << svgPath;
        return QByteArray();
    }
    static const QRegularExpression regex("black");
    QRegularExpressionMatch match = regex.match(svgString);// 执行匹配
    if(!match.hasMatch())
    {
        qWarning()<<"'black' not found in"<<svgPath;
        return QByteArray();
    }
    return svgString.replace(regex, newColor).toUtf8();
}
QPixmap FancySvg::loadSvgFromU8Bt(const QByteArray &svgContent)
{
    QSvgRenderer renderer(svgContent);
    QPixmap pixmap(renderer.defaultSize());
    pixmap.fill(Qt::GlobalColor::transparent);  // 背景透明
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    renderer.render(&painter);
    return pixmap;
}

QPixmap FancySvg::loadSvgFromU8Bt(const QByteArray &svgContent, const QRect& rect)
{
    QSvgRenderer renderer(svgContent);
    QPixmap pixmap(rect.size());
    pixmap.fill(Qt::GlobalColor::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    renderer.render(&painter,rect);
    return pixmap;
}

QPixmap FancySvg::loadSvgFromPath(const QString &path)
{
    QSvgRenderer renderer(path);
    QPixmap pixmap(renderer.defaultSize());
    pixmap.fill(Qt::GlobalColor::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    renderer.render(&painter);
    return pixmap;
}

QPixmap FancySvg::loadSvgFromPath(const QString &path, const QRect& rect)
{
    QSvgRenderer renderer(path);
    QPixmap pixmap(rect.size());
    pixmap.fill(Qt::GlobalColor::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    renderer.render(&painter,rect);
    return pixmap;
}
