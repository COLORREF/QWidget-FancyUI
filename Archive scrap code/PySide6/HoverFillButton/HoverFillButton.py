import math
from PySide6.QtCore import Slot, QPointF, Qt, QPoint, QRectF
from PySide6.QtGui import QCursor, QPainter
from PySide6.QtWidgets import QWidget
from HoverFillButtonBase import HoverFillButtonBase

qSqrt = math.sqrt
qPow = math.pow


class HoverFillButton(HoverFillButtonBase):
    def __init__(self, text: str = None, parent: QWidget = None):
        super().__init__(text=text, parent=parent)
        self.animationType = self.AnimationType.CircularFill
        self.mouse_point = QPointF()
        self.initializeMemberVariable()

    def setAnimationType(self, animationType: HoverFillButtonBase.AnimationType):
        self.animationType = animationType
        self.initializeMemberVariable()

    @Slot()
    def paintValueReduction(self):
        super().paintValueReduction()
        self.update()

    @Slot()
    def paintValueIncrease(self):
        super().paintValueIncrease()
        self.update()

    def enterEvent(self, event):
        super().enterEvent(event)
        self.timer.timeout.connect(self.paintValueIncrease)
        if self.animationType == HoverFillButtonBase.AnimationType.CircularFill:
            self.mouse_point = self.mapFromGlobal(QCursor.pos())
        self.timer.start()

    def leaveEvent(self, event):
        super().leaveEvent(event)
        self.timer.timeout.connect(self.paintValueReduction)
        if self.animationType == HoverFillButtonBase.AnimationType.CircularFill:
            self.mouse_point = self.mapFromGlobal(QCursor.pos())
        self.timer.start()

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.initializeMemberVariable()

    def paintEvent(self, event):
        super().paintEvent(event)
        painter = QPainter(self)
        painter.setRenderHint(QPainter.RenderHint.Antialiasing | QPainter.RenderHint.TextAntialiasing)
        painter.setBrush(self.fill_brush)
        painter.setPen(Qt.PenStyle.NoPen)
        painter.setClipPath(self.path)
        painter.setFont(self.font())

        if self.animationType == HoverFillButtonBase.AnimationType.CircularFill:
            if not self.mouse_point.isNull():
                painter.drawEllipse(self.mouse_point, self.paint_value, self.paint_value)
        if self.animationType == HoverFillButtonBase.AnimationType.CrossFill:
            painter.drawEllipse(QPoint(0, 0), self.paint_value, self.paint_value)
            painter.drawEllipse(QPoint(self.width(), self.height()), self.paint_value, self.paint_value)
        if self.animationType == HoverFillButtonBase.AnimationType.DiagonalRectangle:
            painter.save()
            painter.translate(self.width() / 2, self.height() / 2)
            painter.rotate(45)
            painter.drawRect(QRectF(-self.paint_value / 2, -self.width() / 2, self.paint_value, self.width()))
            painter.restore()

        if self.animationType == HoverFillButtonBase.AnimationType.BottomCircle:
            painter.save()
            painter.translate(self.width() / 2, int(self.height() * 1.5))
            painter.drawEllipse(QPoint(0, 0), self.paint_value, self.paint_value)
            painter.restore()

        if self.animationType == HoverFillButtonBase.AnimationType.LeftRectangle:
            painter.save()
            if self.is_cursor_inside:
                painter.drawRect(QRectF(0, 0, self.paint_value, self.height()))
            else:
                painter.translate(self.width(), self.height())
                painter.rotate(180)
                painter.drawRect(QRectF(0, 0, self.paint_value, self.height()))
            painter.restore()
        if self.paint_value > 0:
            painter.setPen(self.hover_text_color)
            painter.drawText(self.rect(), Qt.AlignmentFlag.AlignCenter, self.text())

    def initializeMemberVariable(self):
        if self.animationType == HoverFillButtonBase.AnimationType.CircularFill:
            self.max_painted_value = qSqrt(self.width() * self.width() + self.height() * self.height())
            return
        else:
            self.max_painted_value = int(qPow(qPow(self.width(), 2) + qPow(self.height(), 2), 0.5))
            return
