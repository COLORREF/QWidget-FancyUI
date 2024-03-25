from PySide6.QtCore import Qt, Slot, QEvent
from PySide6.QtGui import QColor, QPainter, QMouseEvent
from PySide6.QtWidgets import QWidget, QGridLayout, QGraphicsDropShadowEffect, QPushButton

from Border import Border, BorderType
from MainArea import MainArea


class UI:
    def __init__(self):
        self.right_bottom_border = None
        self.right_border = None
        self.left_bottom_border = None
        self.bottom_border = None
        self.master_scope = None
        self.top_border = None
        self.right_top_border = None
        self.left_top_border = None
        self.left_border = None
        self.gridLayout = None

    def setupUi(self, parent: QWidget):
        parent.resize(640, 480)

        # 栅格布局
        self.gridLayout = QGridLayout(parent)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setContentsMargins(10, 0, 10, 10)

        # 左边框
        self.left_border = Border(parent, BorderType.left_border)
        self.gridLayout.addWidget(self.left_border, 1, 1, 1, 1)

        # 左上边框
        self.left_top_border = Border(parent, BorderType.left_top_border)
        self.gridLayout.addWidget(self.left_top_border, 0, 1, 1, 1)

        # 右上边框
        self.right_top_border = Border(parent, BorderType.right_top_border)
        self.gridLayout.addWidget(self.right_top_border, 0, 3, 1, 1)

        # 上边框
        self.top_border = Border(parent, BorderType.top_border)
        self.gridLayout.addWidget(self.top_border, 0, 2, 1, 1)

        # 主区域和标题栏
        self.master_scope = MainArea(parent)
        self.gridLayout.addWidget(self.master_scope, 1, 2, 1, 1)

        # 下边框
        self.bottom_border = Border(parent, BorderType.bottom_border)
        self.gridLayout.addWidget(self.bottom_border, 2, 2, 1, 1)

        # 左下边框
        self.left_bottom_border = Border(parent, BorderType.left_bottom_border)
        self.gridLayout.addWidget(self.left_bottom_border, 2, 1, 1, 1)

        # 右边框
        self.right_border = Border(parent, BorderType.right_border)
        self.gridLayout.addWidget(self.right_border, 1, 3, 1, 1)

        # 右下边框
        self.right_bottom_border = Border(parent, BorderType.right_bottom_border)
        self.gridLayout.addWidget(self.right_bottom_border, 2, 3, 1, 1)


class BorderlessWindow(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowFlags(Qt.WindowType.FramelessWindowHint)  # 隐藏标题栏
        self.setAttribute(Qt.WidgetAttribute.WA_TranslucentBackground)  # 设置窗口透明
        self.ui = UI()
        self.ui.setupUi(self)

        self.ui.master_scope.TitleBar().MoveWindow.connect(self.MoveWindow)

        self.shadow = QGraphicsDropShadowEffect(self)
        self.shadow.setBlurRadius(35)
        self.shadow.setColor(QColor(43, 45, 48, 100))
        self.shadow.setOffset(0, 5)
        self.setGraphicsEffect(self.shadow)

    @Slot()
    def showMaximized(self):
        self.ui.gridLayout.setContentsMargins(0, 0, 0, 0)
        for border in self.findChildren(Border):
            if not border.isHidden():
                border.hide()
        super().showMaximized()
        self.update()

    @Slot()
    def showNormal(self):
        super().showNormal()
        for border in self.findChildren(Border):
            if border.isHidden():
                border.show()
        self.ui.gridLayout.setContentsMargins(10, 0, 10, 10)
        self.update()

    def MoveWindow(self):
        for border in self.findChildren(Border):
            if border.isHidden():
                border.show()
        if self.isMaximized():
            self.ui.gridLayout.setContentsMargins(10, 0, 10, 10)
        self.windowHandle().startSystemMove()
