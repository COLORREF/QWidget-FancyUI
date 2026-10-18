#include "IconGallery.h"
#include "IconListModel.h"

#include <QApplication>
#include <QCache>
#include <QClipboard>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>
#include <QStyledItemDelegate>
#include <QTimer>
#include <cmath>

#include "Core/Palette/Palette.h"
#include "Core/Theme/ThemeModeController.h"
#include "Icon/Icon.h"
#include "Widgets/PushButton/PushButton.h"
#include "Widgets/ScrollBar/ScrollBar.h"

namespace
{
    constexpr int TileSize = 96;
    constexpr int TileSpacing = 8;
    constexpr int IconSize = 28;
    constexpr int GridPadding = 16;
    constexpr int CaptionSize = 12;
    constexpr int ControlRadius = 4;
    constexpr int GalleryRadius = 8;

    struct GalleryColors
    {
        QColor background, card, stroke, text, secondary, hover, pressed, divider, control, controlStroke, innerStroke;
    };

    GalleryColors galleryColors()
    {
        if (fancy::ThemeModeController::controller().isAppLight())
            return {
                QColor(243, 243, 243),
                QColor(255, 255, 255, 179),
                QColor(0, 0, 0, 15),
                QColor(0, 0, 0, 228),
                QColor(0, 0, 0, 158),
                QColor(0, 0, 0, 9),
                QColor(0, 0, 0, 6),
                QColor(0, 0, 0, 15),
                QColor(255, 255, 255, 179),
                QColor(0, 0, 0, 15),
                QColor(255, 255, 255)
            };
        return {
            QColor(32, 32, 32),
            QColor(255, 255, 255, 13),
            QColor(0, 0, 0, 25),
            QColor(255, 255, 255),
            QColor(255, 255, 255, 197),
            QColor(255, 255, 255, 15),
            QColor(255, 255, 255, 10),
            QColor(255, 255, 255, 21),
            QColor(255, 255, 255, 15),
            QColor(255, 255, 255, 18),
            QColor(69, 69, 69)
        };
    }

    QPixmap renderIcon(const fancy::IconId &id, int size, qreal dpr, const QColor &color)
    {
        fancy::Icon icon;
        icon.addIconEnum(id, QIcon::Normal, QIcon::Off, color);
        return icon.pixmap(QSize(size, size), dpr);
    }

    class IconTileDelegate final : public QStyledItemDelegate
    {
    public:
        explicit IconTileDelegate(QObject *parent) :
            QStyledItemDelegate(parent),
            _cache(16 * 1024) {}

        QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override
        {
            return {TileSize + TileSpacing, TileSize + TileSpacing};
        }

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
        {
            const auto *model = static_cast<const IconListModel *>(index.model());
            const auto *item = model->entry(index);
            if (!item)
                return;
            const auto colors = galleryColors();
            const QRectF tile(option.rect.topLeft(), QSizeF(TileSize, TileSize));
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setClipRect(option.rect);
            painter->setPen(QPen(colors.stroke, 1));
            painter->setBrush(colors.card);
            painter->drawRoundedRect(tile.adjusted(0.5, 0.5, -0.5, -0.5), ControlRadius, ControlRadius);
            if (option.state & QStyle::State_MouseOver)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush((QApplication::mouseButtons() & Qt::LeftButton) ? colors.pressed : colors.hover);
                painter->drawRoundedRect(tile, ControlRadius, ControlRadius);
            }
            if (option.state & QStyle::State_Selected)
            {
                painter->setBrush(Qt::NoBrush);
                painter->setPen(QPen(fancy::Palette::palette()[fancy::ColorRole::AppAccent], 3));
                painter->drawRoundedRect(tile.adjusted(1.5, 1.5, -1.5, -1.5), ControlRadius, ControlRadius);
                painter->setPen(QPen(colors.innerStroke, 1));
                painter->drawRoundedRect(tile.adjusted(2.5, 2.5, -2.5, -2.5), 2, 2);
            }

            const qreal dpr = painter->device()->devicePixelRatioF();
            const QString key = QStringLiteral("%1/%2/%3/%4").arg(static_cast<int>(item->id.style))
                    .arg(item->id.value).arg(dpr, 0, 'g', 12).arg(colors.text.rgba());
            const QPixmap *cached = _cache.object(key);
            QPixmap pixmap;
            if (cached)
                pixmap = *cached;
            else
            {
                pixmap = renderIcon(item->id, IconSize, dpr, colors.text);
                const int cost = qMax(1, static_cast<int>(std::ceil(pixmap.width() * pixmap.height() * 4.0 / 1024)));
                _cache.insert(key, new QPixmap(pixmap), cost);
            }
            // Viewbox 的下边距为 16，图标中心位于格子中心上方 8
            painter->drawPixmap(QPointF(tile.x() + 34, tile.y() + 26), pixmap);
            QFont font = option.font;
            font.setPixelSize(CaptionSize);
            painter->setFont(font);
            painter->setPen(colors.secondary);
            const QRectF label = tile.adjusted(8, 72, -8, -8);
            painter->drawText(label, Qt::AlignCenter, QFontMetrics(font).elidedText(item->name, Qt::ElideRight, 80));
            if ((option.state & QStyle::State_HasFocus) && !(option.state & QStyle::State_Selected))
            {
                painter->setBrush(Qt::NoBrush);
                painter->setPen(QPen(colors.text, 1, Qt::DotLine));
                painter->drawRoundedRect(tile.adjusted(2, 2, -2, -2), 2, 2);
            }
            painter->restore();
        }

    private:
        // 按 KiB 限制页面局部缓存；颜色和 DPR 属于键，不改变图标引擎的全局缓存策略
        mutable QCache<QString, QPixmap> _cache;
    };

    class IconGridView final : public QListView
    {
    public:
        explicit IconGridView(QWidget *parent) :
            QListView(parent)
        {
            setFrameShape(QFrame::NoFrame);
            setViewportMargins(GridPadding, GridPadding, GridPadding - TileSpacing, GridPadding - TileSpacing);
            setViewMode(QListView::IconMode);
            setMovement(QListView::Static);
            setResizeMode(QListView::Adjust);
            setWrapping(true);
            setGridSize({TileSize + TileSpacing, TileSize + TileSpacing});
            setUniformItemSizes(true);
            // 等尺寸目录布局不解析 SVG；同步布局保证切换分类后可直接定位末尾的选中项
            setLayoutMode(QListView::SinglePass);
            setSelectionMode(QAbstractItemView::SingleSelection);
            setEditTriggers(QAbstractItemView::NoEditTriggers);
            setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBar(new fancy::ScrollBar(Qt::Vertical, this));
            setMouseTracking(true);
            viewport()->setAttribute(Qt::WA_Hover);
            setMinimumWidth(TileSize + GridPadding * 2 + 12);
            setAccessibleName(QStringLiteral("图标列表"));
        }

        QModelIndex indexAt(const QPoint &point) const override
        {
            const QModelIndex index = QListView::indexAt(point);
            // 网格单元包含右侧和底部间距，间距不参与悬停和点击命中
            return index.isValid() && QRect(visualRect(index).topLeft(), QSize(TileSize, TileSize)).contains(point)
                       ? index
                       : QModelIndex();
        }
    };
}

class IconDetailPanel final : public QWidget
{
public:
    explicit IconDetailPanel(QWidget *parent) :
        QWidget(parent)
    {
        auto *layout = new QVBoxLayout(this);
        layout->setContentsMargins(16, 106, 16, 16);
        layout->setSpacing(8);
        _name = addField(layout, QStringLiteral("图标名称"));
        _library = addField(layout, QStringLiteral("图标库"));
        _enum = addField(layout, QStringLiteral("枚举成员"));
        _copy = new fancy::PushButton(QStringLiteral("复制枚举"), this);
        layout->addWidget(_copy, 0, Qt::AlignLeft);
        layout->addStretch();
        connect(_copy, &QPushButton::clicked, this, [this] {
            QApplication::clipboard()->setText(_entry.enumName);
            _copy->setText(QStringLiteral("已复制"));
            QTimer::singleShot(1200, _copy, [this] { _copy->setText(QStringLiteral("复制枚举")); });
        });
        setMinimumWidth(216);
        updateAppearance();
    }

    void setEntry(const IconEntry *entry)
    {
        _valid = entry != nullptr;
        _entry = entry ? *entry : IconEntry{};
        _name->setText(_entry.name);
        _library->setText(_entry.library);
        // 在命名空间分隔符后允许换行，长枚举不会撑宽详情区
        _enum->setText(QString(_entry.enumName).replace(QStringLiteral("::"), QStringLiteral("::\u200b")));
        _copy->setEnabled(_valid);
        _copy->setText(QStringLiteral("复制枚举"));
        update();
    }

    void updateAppearance()
    {
        const auto colors = galleryColors();
        for (auto *label: findChildren<QLabel *>())
        {
            QPalette pal = label->palette();
            pal.setColor(QPalette::WindowText, label->property("caption").toBool() ? colors.secondary : colors.text);
            label->setPalette(pal);
        }
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        const auto colors = galleryColors();
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath background;
        background.addRoundedRect(QRectF(rect()), GalleryRadius, GalleryRadius);
        background.addRect(QRectF(0, 0, width() / 2.0, height()));
        background.setFillRule(Qt::WindingFill);
        painter.fillPath(background, colors.card);
        painter.setPen(colors.divider);
        painter.drawLine(0, 0, 0, height());
        if (!_valid)
            return;
        painter.setPen(QPen(colors.controlStroke, 1));
        painter.setBrush(colors.control);
        painter.drawRoundedRect(QRectF(16.5, 16.5, 65, 65), ControlRadius, ControlRadius);
        painter.drawPixmap(QPointF(25, 25), renderIcon(_entry.id, 48, devicePixelRatioF(), colors.text));
    }

private:
    QLabel *addField(QVBoxLayout *layout, const QString &title)
    {
        auto *caption = new QLabel(title, this);
        caption->setProperty("caption", true);
        QFont font = caption->font();
        font.setPixelSize(CaptionSize);
        caption->setFont(font);
        layout->addWidget(caption);
        auto *value = new QLabel(this);
        value->setTextFormat(Qt::PlainText);
        value->setWordWrap(true);
        value->setTextInteractionFlags(Qt::TextSelectableByMouse);
        font.setPixelSize(14);
        value->setFont(font);
        layout->addWidget(value);
        layout->addSpacing(16);
        return value;
    }

    IconEntry _entry;
    bool _valid = false;
    QLabel *_name;
    QLabel *_library;
    QLabel *_enum;
    fancy::PushButton *_copy;
};

IconGallery::IconGallery(QWidget *parent) :
    QWidget(parent),
    _model(new IconListModel(this)),
    _view(new IconGridView(this)),
    _details(new IconDetailPanel(this))
{
    setObjectName(QStringLiteral("iconGallery"));
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1);
    layout->setSpacing(0);
    layout->addWidget(_view, 1);
    // 详情较 Gallery 精简，采用 Fluent 图标页的窄侧栏思路，保留独立滚动
    auto *detailsScroll = new QScrollArea(this);
    detailsScroll->setFrameShape(QFrame::NoFrame);
    detailsScroll->setFixedWidth(240);
    detailsScroll->setWidgetResizable(true);
    detailsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    detailsScroll->setVerticalScrollBar(new fancy::ScrollBar(Qt::Vertical, detailsScroll));
    detailsScroll->setWidget(_details);
    _details->setAutoFillBackground(false);
    detailsScroll->setAutoFillBackground(false);
    detailsScroll->viewport()->setAutoFillBackground(false);
    layout->addWidget(detailsScroll);
    _view->setModel(_model);
    _view->setItemDelegate(new IconTileDelegate(_view));
    connect(_view->selectionModel(), &QItemSelectionModel::currentChanged, this,
            [this](const QModelIndex &index) { _details->setEntry(_model->entry(index)); });
    connect(&fancy::Palette::palette(), &fancy::Palette::appThemeChange, this, &IconGallery::updateAppearance);
    connect(&fancy::Palette::palette(), &fancy::Palette::appColorChange, this, &IconGallery::updateAppearance);
    updateAppearance();
    setLibrary(std::nullopt);
}

void IconGallery::setLibrary(std::optional<fancy::IconStyle> library)
{
    const auto *entry = _model->entry(_view->currentIndex());
    const std::optional<fancy::IconId> previous = entry ? std::optional<fancy::IconId>(entry->id) : std::nullopt;
    _model->setLibrary(library);
    QModelIndex selected = _model->index(0, 0);
    if (previous)
        for (int row = 0; row < _model->rowCount(); ++row)
            if (_model->entry(_model->index(row, 0))->id == *previous)
            {
                selected = _model->index(row, 0);
                break;
            }
    _view->setCurrentIndex(selected);
    _view->scrollTo(selected);
    _details->setEntry(_model->entry(selected));
}

void IconGallery::updateAppearance()
{
    const auto colors = galleryColors();
    QPalette pal = palette();
    pal.setColor(QPalette::Base, colors.background);
    pal.setColor(QPalette::Window, colors.background);
    pal.setColor(QPalette::Text, colors.text);
    setPalette(pal);
    _view->setPalette(pal);
    _details->updateAppearance();
    _view->viewport()->update();
    update();
}

void IconGallery::paintEvent(QPaintEvent *)
{
    const auto colors = galleryColors();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(colors.stroke, 1));
    painter.setBrush(colors.background);
    painter.drawRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), GalleryRadius, GalleryRadius);
}
