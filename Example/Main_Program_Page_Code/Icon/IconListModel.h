#ifndef EXAMPLE_ICONLISTMODEL_H
#define EXAMPLE_ICONLISTMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <optional>
#include "Core/Defs.h"

/** @brief 图标目录中的一项，不持有渲染器或位图 */
struct IconEntry
{
    fancy::IconId id;
    QString name;
    QString library;
    QString enumName;
};

/** @brief 为图标展示提供枚举目录，分类切换仅更新可见行映射 */
class IconListModel final : public QAbstractListModel
{
public:
    explicit IconListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = {}) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    /** @brief
     * 设置显示的图标库；空值表示所有图标
     */
    void setLibrary(std::optional<fancy::IconStyle> library);

    /**
     * @brief 返回当前可见行的数据；无效索引返回 nullptr
     */
    const IconEntry *entry(const QModelIndex &index) const;

private:
    QVector<IconEntry> _entries;
    QVector<int> _rows;
};
#endif
