import json
from copy import deepcopy
from enum import Enum
from pathlib import Path
from typing import List
import darkdetect
from PySide6.QtCore import QObject, Signal
from PySide6.QtGui import QColor
def exceptionHandler(*default):
    def outer(func):

        def inner(*args, **kwargs):
            try:
                return func(*args, **kwargs)
            except BaseException as e:
                value = deepcopy(default)
                if len(value) == 0:
                    return None
                elif len(value) == 1:
                    return value[0]

                return value

        return inner

    return outer
class Theme(Enum):
    LIGHT = "Light"
    DARK = "Dark"
    AUTO = "Auto"
class ConfigValidator:
    def validate(self, value):
        return True
    def correct(self, value):
        return value
class RangeValidator(ConfigValidator):
    def __init__(self, min, max):
        self.min = min
        self.max = max
        self.range = (min, max)
    def validate(self, value):
        return self.min <= value <= self.max
    def correct(self, value):
        return min(max(self.min, value), self.max)
class OptionsValidator(ConfigValidator):
    def __init__(self, options):
        if not options:
            raise ValueError("The `options` can't be empty.")
        if isinstance(options, Enum):
            options = options._member_map_.values()
        self.options = list(options)
    def validate(self, value):
        return value in self.options
    def correct(self, value):
        return value if self.validate(value) else self.options[0]
class BoolValidator(OptionsValidator):
    def __init__(self):
        super().__init__([True, False])
class FolderValidator(ConfigValidator):
    def validate(self, value):
        return Path(value).exists()
    def correct(self, value):
        path = Path(value)
        path.mkdir(exist_ok=True, parents=True)
        return str(path.absolute()).replace("\\", "/")
class FolderListValidator(ConfigValidator):
    def validate(self, value):
        return all(Path(i).exists() for i in value)
    def correct(self, value: List[str]):
        folders = []
        for folder in value:
            path = Path(folder)
            if path.exists():
                folders.append(str(path.absolute()).replace("\\", "/"))
        return folders
class ColorValidator(ConfigValidator):
    def __init__(self, default):
        self.default = QColor(default)
    def validate(self, color):
        try:
            return QColor(color).isValid()
        except:
            return False
    def correct(self, value):
        return QColor(value) if self.validate(value) else self.default
class ConfigSerializer:
    def serialize(self, value):
        return value
    def deserialize(self, value):
        return value
class EnumSerializer(ConfigSerializer):
    def __init__(self, enumClass):
        self.enumClass = enumClass
    def serialize(self, value):
        return value.value
    def deserialize(self, value):
        return self.enumClass(value)
class ColorSerializer(ConfigSerializer):
    def serialize(self, value: QColor):
        return value.name(QColor.HexArgb)
    def deserialize(self, value):
        if isinstance(value, list):
            return QColor(*value)
        return QColor(value)
class ConfigItem(QObject):
    valueChanged = Signal(object)
    def __init__(self, group, name, default, validator=None, serializer=None, restart=False):
        super().__init__()
        self.group = group
        self.name = name
        self.validator = validator or ConfigValidator()
        self.serializer = serializer or ConfigSerializer()
        self.__value = default
        self.value = default
        self.restart = restart
        self.defaultValue = self.validator.correct(default)
    @property
    def value(self):
        return self.__value
    @value.setter
    def value(self, v):
        v = self.validator.correct(v)
        ov = self.__value
        self.__value = v
        if ov != v:
            self.valueChanged.emit(v)
    @property
    def key(self):
        return self.group+"."+self.name if self.name else self.group
    def __str__(self):
        return f'{self.__class__.__name__}[value={self.value}]'
    def serialize(self):
        return self.serializer.serialize(self.value)
    def deserializeFrom(self, value):
        self.value = self.serializer.deserialize(value)
class RangeConfigItem(ConfigItem):
    @property
    def range(self):
        return self.validator.range
    def __str__(self):
        return f'{self.__class__.__name__}[range={self.range}, value={self.value}]'
class OptionsConfigItem(ConfigItem):
    @property
    def options(self):
        return self.validator.options
    def __str__(self):
        return f'{self.__class__.__name__}[options={self.options}, value={self.value}]'
class ColorConfigItem(ConfigItem):
    def __init__(self, group, name, default, restart=False):
        super().__init__(group, name, QColor(default), ColorValidator(default),
                         ColorSerializer(), restart)
    def __str__(self):
        return f'{self.__class__.__name__}[value={self.value.name()}]'
class QConfig(QObject):
    appRestartSig = Signal()
    themeChanged = Signal(Theme)
    themeChangedFinished = Signal()
    themeColorChanged = Signal(QColor)
    themeMode = OptionsConfigItem(
        "QFluentWidgets", "ThemeMode", Theme.LIGHT, OptionsValidator(Theme), EnumSerializer(Theme))
    themeColor = ColorConfigItem("QFluentWidgets", "ThemeColor", '#009faa')
    def __init__(self):
        super().__init__()
        self.file = Path("config/config.json")
        self._theme = Theme.LIGHT
        self._cfg = self
    def get(self, item):
        return item.value

    def set(self, item, value, save=True, copy=True):
        if item.value == value:
            return
        item.value = deepcopy(value) if copy else value
        if save:
            self.save()
        if item.restart:
            self._cfg.appRestartSig.emit()
        if item is self._cfg.themeMode:
            self.theme = value
            self._cfg.themeChanged.emit(value)
        if item is self._cfg.themeColor:
            self._cfg.themeColorChanged.emit(value)
    def toDict(self, serialize=True):
        items = {}
        for name in dir(self._cfg.__class__):
            item = getattr(self._cfg.__class__, name)
            if not isinstance(item, ConfigItem):
                continue
            value = item.serialize() if serialize else item.value
            if not items.get(item.group):
                if not item.name:
                    items[item.group] = value
                else:
                    items[item.group] = {}
            if item.name:
                items[item.group][item.name] = value
        return items
    def save(self):
        self._cfg.file.parent.mkdir(parents=True, exist_ok=True)
        with open(self._cfg.file, "w", encoding="utf-8") as f:
            json.dump(self._cfg.toDict(), f, ensure_ascii=False, indent=4)
    @exceptionHandler()
    def load(self, file=None, config=None):
        if isinstance(config, QConfig):
            self._cfg = config
            self._cfg.themeChanged.connect(self.themeChanged)
        if isinstance(file, (str, Path)):
            self._cfg.file = Path(file)
        try:
            with open(self._cfg.file, encoding="utf-8") as f:
                cfg = json.load(f)
        except:
            cfg = {}
        items = {}
        for name in dir(self._cfg.__class__):
            item = getattr(self._cfg.__class__, name)
            if isinstance(item, ConfigItem):
                items[item.key] = item
        for k, v in cfg.items():
            if not isinstance(v, dict) and items.get(k) is not None:
                items[k].deserializeFrom(v)
            elif isinstance(v, dict):
                for key, value in v.items():
                    key = k + "." + key
                    if items.get(key) is not None:
                        items[key].deserializeFrom(value)
        self.theme = self.get(self.themeMode)
    @property
    def theme(self):
        return self._cfg._theme
    @theme.setter
    def theme(self, t):
        if t == Theme.AUTO:
            t = darkdetect.theme()
            t = Theme(t) if t else Theme.LIGHT
        self._cfg._theme = t
qconfig = QConfig()
def isDarkTheme():
    return qconfig.theme == Theme.DARK
def theme():
    return qconfig.theme