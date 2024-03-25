from PySide6.QtCore import Qt
from PySide6.QtGui import QPaintEvent, QPainter, QColor
from PySide6.QtWidgets import QVBoxLayout, QWidget

from TitleBar import TitleBar


class CustomerAreaWidget(QWidget):
    """客户区"""

    def __init__(self, parent):
        super().__init__(parent=parent)

    def paintEvent(self, event: QPaintEvent):
        painter = QPainter(self)
        painter.setPen(Qt.PenStyle.NoPen)
        painter.setBrush(QColor(240, 240, 240))
        painter.drawRect(self.rect())


class MainArea(QWidget):
    """主区域，包含客户区和标题栏"""

    def __init__(self, parent: QWidget):
        super().__init__(parent)
        self.verticalLayout = QVBoxLayout(self)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setSpacing(0)

        self.title_bar = TitleBar(self)
        self.customer_area = CustomerAreaWidget(self)

        self.verticalLayout.addWidget(self.title_bar)
        self.verticalLayout.addWidget(self.customer_area)

    def TitleBar(self):
        return self.title_bar


