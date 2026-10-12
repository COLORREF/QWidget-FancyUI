#include "SelectorBarItem.h"

#include <QPainter>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOptionButton>

#include "SelectorBarItemStyle.h"
#include "private/SelectorBarIndicatorLayer.h"
#include "Core/Palette/Palette.h"
#include "Icon/Icon.h"

namespace fancy
{
    SelectorBarItem::SelectorBarItem(QWidget *parent) :
        QAbstractButton(parent)
    {
        setStyle(new SelectorBarItemStyle(this));
        setCheckable(true);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        setAttribute(Qt::WA_Hover);
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
        connect(this, &QAbstractButton::toggled, this, [this](bool checked) {
            if (_indicatorManagedByBar)
                return;
            // 独立使用时按需创建局部覆盖层，加入栏后由栏的共享层接管。
            if (checked && !_localIndicator)
                _localIndicator = new detail::SelectorBarIndicatorLayer(this);
            if (_localIndicator)
                _localIndicator->select(checked ? this : nullptr);
        });
        auto &palette = Palette::palette();
        connect(&palette, &Palette::appColorChange, this, &SelectorBarItem::refreshIconColors);
        connect(&palette, &Palette::appThemeChange, this, &SelectorBarItem::refreshIconColors);
        connect(&palette, &Palette::appColorChange, this, QOverload<>::of(&SelectorBarItem::update));
        connect(&palette, &Palette::appThemeChange, this, QOverload<>::of(&SelectorBarItem::update));
    }

    SelectorBarItem::SelectorBarItem(const QString &text, QWidget *parent) :
        SelectorBarItem(parent)
    {
        setText(text);
    }

    void SelectorBarItem::setIndicatorManagedByBar(bool managed)
    {
        _indicatorManagedByBar = managed;
        if (managed)
        {
            delete _localIndicator;
            _localIndicator = nullptr;
        }
    }

    void SelectorBarItem::setIcon(const QIcon &icon)
    {
        _iconId.reset();
        QAbstractButton::setIcon(icon);
    }

    void SelectorBarItem::setIcon(const IconId &id)
    {
        _iconId = id;
        refreshIconColors();
    }

    void SelectorBarItem::refreshIconColors()
    {
        if (!_iconId)
            return;

        Icon icon;
        auto &palette = Palette::palette();
        // QIcon 的 Mode 与 On/Off 组合分别对应八种交互状态，复用对应的文本色；两种禁用状态共用 DisEnabledText。
        icon.addIconEnum(*_iconId, QIcon::Normal, QIcon::Off, palette[ColorRole::Text]);
        icon.addIconEnum(*_iconId, QIcon::Active, QIcon::Off, palette[ColorRole::SelectorBarItemTextHover]);
        icon.addIconEnum(*_iconId, QIcon::Selected, QIcon::Off, palette[ColorRole::SelectorBarItemTextPressed]);
        icon.addIconEnum(*_iconId, QIcon::Disabled, QIcon::Off, palette[ColorRole::DisEnabledText]);
        icon.addIconEnum(*_iconId, QIcon::Normal, QIcon::On, palette[ColorRole::SelectorBarItemTextSelected]);
        icon.addIconEnum(*_iconId, QIcon::Active, QIcon::On, palette[ColorRole::SelectorBarItemTextSelectedHover]);
        icon.addIconEnum(*_iconId, QIcon::Selected, QIcon::On, palette[ColorRole::SelectorBarItemTextSelectedPressed]);
        icon.addIconEnum(*_iconId, QIcon::Disabled, QIcon::On, palette[ColorRole::DisEnabledText]);
        QAbstractButton::setIcon(icon);
    }

    void SelectorBarItem::setTextElideEnabled(bool enabled)
    {
        if (_textElideEnabled == enabled)
            return;
        _textElideEnabled = enabled;
        update();
    }

    bool SelectorBarItem::isTextElideEnabled() const
    {
        return _textElideEnabled;
    }

    QSize SelectorBarItem::sizeHint() const
    {
        if (auto s = dynamic_cast<SelectorBarItemStyle *>(style()))
            return s->sizeHint();
        return QAbstractButton::sizeHint();
    }

    void SelectorBarItem::paintEvent(QPaintEvent *event)
    {
        Q_UNUSED(event)
        QStyleOptionButton option;
        option.initFrom(this);
        option.text = text();
        option.icon = icon();
        option.iconSize = iconSize();
        option.state |= isChecked() ? QStyle::State_On : QStyle::State_Off;
        if (isDown())
            option.state |= QStyle::State_Sunken;

        QPainter painter(this);
        style()->drawControl(QStyle::CE_PushButton, &option, &painter, this);
    }

} // fancy 命名空间
