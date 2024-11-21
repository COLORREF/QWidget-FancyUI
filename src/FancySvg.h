#ifndef FANCYSVG_H
#define FANCYSVG_H

#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QDomElement>
#include <QByteArray>
#include <QDomDocument>
#include <QDomNodeList>
#include <QString>
#include <QFile>
#include <QPixmap>
#include <QSvgRenderer>
#include <QPainter>

class FancySvg
{
    friend class FancyIconWidget;
private:
    static const QByteArray readSvg(const QString& path);

public:

    //Use an XML parser to parse SVG files and replace the values of all specified attributes in the specified tags
    //For example, replacing the values of all "fill" attributes in all "<path>" tags with "red" or RGB values such as "rgb (255,0,0)"
    //The performance of this function may not be ideal, and the author did not test them. It is not recommended to use it on large SVG files or frequently.
    static const QByteArray replaceSvgAttributeValue(const QString& SvgPath, const QString& tagName, const QString& attribute, const QString& newValue);


    //Match all specified attribute values in SVG text using regular expressions and replace them with new values
    static const QByteArray replaceSvgAttributeValue(const QString& SvgPath,const QString& attribute, const QString& newValue);

    static const QByteArray replaceSvgBlack(const QString &svgPath, const QString &newColor);

    inline static const QByteArray replacePathFillColor(const QString& SvgPath, const QString& color)
    {
        return replaceSvgAttributeValue(SvgPath,"path","fill",color);
    }

    inline static const QByteArray replaceGStrokeColor(const QString& SvgPath, const QString& color)
    {
        return replaceSvgAttributeValue(SvgPath,"g","stroke",color);
    }

    static QPixmap loadSvgFromU8Bt(const QByteArray& svgContent);
    static QPixmap loadSvgFromU8Bt(const QByteArray& svgContent,const QRect& rect);
    static QPixmap loadSvgFromPath(const QString& path);
    static QPixmap loadSvgFromPath(const QString& path,const QRect& rect);
};



#endif // FANCYSVG_H
