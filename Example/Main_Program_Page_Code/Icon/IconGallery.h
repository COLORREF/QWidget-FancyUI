#ifndef EXAMPLE_ICONGALLERY_H
#define EXAMPLE_ICONGALLERY_H

#include <QWidget>
#include <optional>
#include "Core/Defs.h"

class IconListModel;
class QListView;
class IconDetailPanel;

/** @brief 图标网格及详情容器，独立于页面导航和全局配色配置 */
class IconGallery final : public QWidget
{
public:
    explicit IconGallery(QWidget *parent = nullptr);

    /** @brief
     * 切换图标库，保留仍属于当前分类的选中图标；空值显示全部
     */
    void setLibrary(std::optional<fancy::IconStyle> library);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void updateAppearance();

    IconListModel *_model;
    QListView *_view;
    IconDetailPanel *_details;
};
#endif
