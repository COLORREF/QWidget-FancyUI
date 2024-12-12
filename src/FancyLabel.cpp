#include "FancyLabel.h"
#include "FancyStyle.h"

Label::Label(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent,f)
{
    this->setStyle(new FancyStyleBase(this));
}

Label::Label(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text,parent,f)
{
    this->setStyle(new FancyStyleBase(this));
}


RoundLabel::RoundLabel(QWidget *parent):
    QLabel{parent}
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);  // 设置标签的大小策略为自适应
    setScaledContents(true);//启用自动缩放图像以适应标签大小
}

void RoundLabel::setPixmap(const QPixmap &pixmap)
{
    QPixmap pix(pixmap.size());
    pix.fill(Qt::GlobalColor::transparent);
    QPainter painter(&pix);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform); // 抗锯齿+平滑图像变换
    QPainterPath path;
    path.addEllipse(pix.rect());
    painter.setClipPath(path);
    painter.drawPixmap(path.boundingRect(), pixmap, pixmap.rect());
    QLabel::setPixmap(pix);
}

void RoundLabel::addGraphicsBlurEffect(qreal radius, QGraphicsBlurEffect::BlurHint hints)
{
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect(this);
    blur->setBlurRadius(radius);
    blur->setBlurHints(hints);
    this->setGraphicsEffect(blur);
}

void RoundLabel::resizeEvent(QResizeEvent *event)
{
    //保持横纵比
    QSize newSize = event->size();
    int sideLength = qMin(newSize.width(), newSize.height());
    this->resize(sideLength, sideLength);
}


