from PySide6.QtCore import Qt
from PySide6.QtGui import QPaintEvent, QPainter, QColor
from PySide6.QtWidgets import QVBoxLayout, QWidget, QGraphicsDropShadowEffect
from TitleBar import TitleBar


class CustomerAreaWidget(QWidget):
    """客户区"""

    def __init__(self, parent):
        super().__init__(parent=parent)


class MainArea(QWidget):
    """主区域，包含客户区和标题栏"""

    def __init__(self, parent: QWidget):
        super().__init__(parent)
        self.verticalLayout = QVBoxLayout(self)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setSpacing(0)

        self.title_bar = TitleBar(self)
        self.customer_area_widget = CustomerAreaWidget(self)

        self.verticalLayout.addWidget(self.title_bar)
        self.verticalLayout.addWidget(self.customer_area_widget)

        self.shadow = QGraphicsDropShadowEffect(self)
        self.shadow.setBlurRadius(35)
        self.shadow.setColor(QColor(43, 45, 48, 100))
        self.shadow.setOffset(0, 5)
        self.setGraphicsEffect(self.shadow)

    def TitleBar(self):
        return self.title_bar

    def paintEvent(self, event: QPaintEvent):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.RenderHint.Antialiasing)
        painter.setPen(Qt.PenStyle.NoPen)
        painter.setBrush(QColor(240, 240, 240))
        if not (self.title_bar.isMax and self.window().isMaximized()):
            painter.drawRoundedRect(self.rect(), 10, 10)
        elif self.title_bar.isMax or self.window().isMaximized():
            painter.drawRect(self.rect())
