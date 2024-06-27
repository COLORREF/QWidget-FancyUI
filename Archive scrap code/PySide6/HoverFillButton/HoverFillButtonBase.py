from enum import Enum
from PySide6.QtCore import QTimer, Qt, Slot
from PySide6.QtGui import QBrush
from PySide6.QtWidgets import QWidget
from CustomButtonBase import CustomButtonBase


class HoverFillButtonBase(CustomButtonBase):
    class AnimationType(Enum):
        CircularFill = 0,
        CrossFill = 1,
        DiagonalRectangle = 2,
        BottomCircle = 3,
        LeftRectangle = 4

    def __init__(self, text: str = None, parent: QWidget = None):
        super().__init__(text=text, parent=parent)
        self.timer = QTimer(self)
        self.fill_brush = QBrush(Qt.red)
        self.max_painted_value = 0
        self.paint_variation = 2
        self.paint_value = 0
        self.timer.setInterval(10)

    def setFillSpeed(self, fillSpeed):
        self.timer.setInterval(fillSpeed)

    def setFillBrush(self, fillBrush):
        self.fill_brush = fillBrush

    def enterEvent(self, event):
        super().enterEvent(event)
        # self.timer.disconnect()  # 非常奇怪，VsCode中这样写不会报错，可以正常运行,c++中这么写也可以，但是PyCharm会报错，无法运行
        self.timer.disconnect(self.timer, None, None, None)

    def leaveEvent(self, event):
        super().leaveEvent(event)
        self.timer.disconnect(self.timer, None, None, None)

    @Slot()
    def paintValueReduction(self):
        self.paint_value -= self.paint_variation
        if self.paint_value <= 0:
            self.timer.stop()

    @Slot()
    def paintValueIncrease(self):
        self.paint_value += self.paint_variation
        if self.paint_value > self.max_painted_value:
            self.timer.stop()
