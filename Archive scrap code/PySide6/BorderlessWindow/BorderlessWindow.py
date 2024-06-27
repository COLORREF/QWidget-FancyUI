from PySide6.QtCore import Qt, Slot
from PySide6.QtWidgets import QWidget, QGridLayout
from Border import Border, BorderType
from MainArea import MainArea


class BorderlessWindow(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowFlags(self.windowFlags() | Qt.WindowType.FramelessWindowHint)  # 隐藏标题栏
        self.setAttribute(Qt.WidgetAttribute.WA_TranslucentBackground)  # 设置窗口透明
        self.resize(640, 480)

        # 栅格布局
        self.gridLayout = QGridLayout(self)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setContentsMargins(20, 0, 20, 20)

        # 左边框
        self.left_border = Border(self, BorderType.left_border)
        self.gridLayout.addWidget(self.left_border, 1, 1, 1, 1)

        # 左上边框
        self.left_top_border = Border(self, BorderType.left_top_border)
        self.gridLayout.addWidget(self.left_top_border, 0, 1, 1, 1)

        # 右上边框
        self.right_top_border = Border(self, BorderType.right_top_border)
        self.gridLayout.addWidget(self.right_top_border, 0, 3, 1, 1)

        # 上边框
        self.top_border = Border(self, BorderType.top_border)
        self.gridLayout.addWidget(self.top_border, 0, 2, 1, 1)

        # 主区域和标题栏
        self.master_scope = MainArea(self)
        self.gridLayout.addWidget(self.master_scope, 1, 2, 1, 1)

        # 下边框
        self.bottom_border = Border(self, BorderType.bottom_border)
        self.gridLayout.addWidget(self.bottom_border, 2, 2, 1, 1)

        # 左下边框
        self.left_bottom_border = Border(self, BorderType.left_bottom_border)
        self.gridLayout.addWidget(self.left_bottom_border, 2, 1, 1, 1)

        # 右边框
        self.right_border = Border(self, BorderType.right_border)
        self.gridLayout.addWidget(self.right_border, 1, 3, 1, 1)

        # 右下边框
        self.right_bottom_border = Border(self, BorderType.right_bottom_border)
        self.gridLayout.addWidget(self.right_bottom_border, 2, 3, 1, 1)

    @Slot()
    def showMaximized(self):
        self.gridLayout.setContentsMargins(0, 0, 0, 0)
        for border in self.findChildren(Border):
            if not border.isHidden():
                border.hide()
        super().showMaximized()

    @Slot()
    def showNormal(self):
        for border in self.findChildren(Border):
            if border.isHidden():
                border.show()
        self.gridLayout.setContentsMargins(20, 0, 20, 20)
        super().showNormal()
