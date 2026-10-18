#include "IconListModel.h"

namespace
{
    template<class Enum>
    void appendLibrary(QVector<IconEntry> &entries, const QString &library, const QString &type)
    {
        for (const auto value : magic_enum::enum_values<Enum>())
        {
            const auto name = magic_enum::enum_name(value);
            const QString text = QString::fromLatin1(name.data(), static_cast<int>(name.size()));
            entries.append({fancy::iconId(value), text, library, QStringLiteral("fancy::%1::%2").arg(type, text)});
        }
    }
}

IconListModel::IconListModel(QObject *parent) : QAbstractListModel(parent)
{
    appendLibrary<fancy::IconPark>(_entries, QStringLiteral("IconPark"), QStringLiteral("IconPark"));
    appendLibrary<fancy::AntDesignIcons>(_entries, QStringLiteral("Ant Design"), QStringLiteral("AntDesignIcons"));
    appendLibrary<fancy::BootstrapIcons>(_entries, QStringLiteral("Bootstrap"), QStringLiteral("BootstrapIcons"));
    setLibrary(std::nullopt);
}

int IconListModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(_rows.size());
}

QVariant IconListModel::data(const QModelIndex &index, int role) const
{
    const auto *item = entry(index);
    if (!item)
        return {};
    if (role == Qt::DisplayRole || role == Qt::AccessibleTextRole)
        return item->name;
    if (role == Qt::AccessibleDescriptionRole)
        return item->enumName;
    return {};
}

void IconListModel::setLibrary(std::optional<fancy::IconStyle> library)
{
    beginResetModel();
    _rows.clear();
    for (int i = 0; i < _entries.size(); ++i)
        if (!library || _entries[i].id.style == *library)
            _rows.append(i);
    endResetModel();
}

const IconEntry *IconListModel::entry(const QModelIndex &index) const
{
    if (!index.isValid() || index.model() != this || index.column() != 0 || index.row() < 0 || index.row() >= _rows.size())
        return nullptr;
    return &_entries[_rows[index.row()]];
}
