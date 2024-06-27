from PySide6.QtCore import Qt
from PySide6.QtGui import QPainterPath, QPalette
from PySide6.QtWidgets import QPushButton, QGraphicsDropShadowEffect, QWidget


class CustomButtonBase(QPushButton):
    def __init__(self, text: str = None, parent: QWidget = None):
        super().__init__(text=text, parent=parent)
        self.hover_text_color = Qt.GlobalColor.black
        self.path = QPainterPath()
        self.is_cursor_inside = False
        self.radius = 4
        self.path.addRoundedRect(self.rect(), self.radius, self.radius)

    def addShadow(self, right, down, blurRadius, shadowColor=Qt.GlobalColor.black):
        shadow = QGraphicsDropShadowEffect(self)
        shadow.setOffset(right, down)
        shadow.setColor(shadowColor)
        shadow.setBlurRadius(blurRadius)
        self.setGraphicsEffect(shadow)

    def setTextColor(self, normalTextColor, hoverTextColor):
        self.hover_text_color = hoverTextColor
        self.palette().setColor(QPalette.ColorRole.ButtonText, normalTextColor)

    def refreshRadius(self, radius):
        self.radius = radius
        self.path.clear()
        self.path.addRoundedRect(self.rect(), radius, radius)

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.path.clear()
        self.path.addRoundedRect(self.rect(), self.radius, self.radius)

    def enterEvent(self, event):
        super().enterEvent(event)
        self.is_cursor_inside = True

    def leaveEvent(self, event):
        super().leaveEvent(event)
        self.is_cursor_inside = False
