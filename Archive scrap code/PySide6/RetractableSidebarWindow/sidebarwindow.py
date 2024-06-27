from PySide6.QtCore import Signal
from PySide6.QtWidgets import QWidget, QHBoxLayout, QSpacerItem, QStackedWidget, QSizePolicy

from sidebar import Sidebar


class SidebarWindow(QWidget):
    clicked = Signal(int)

    def __init__(self, parent=None):
        super().__init__(parent)
        self.sidebar_ = Sidebar(self)  # 侧边栏 无布时, 局默认位置即为(0, 0)
        self.horizontalLayout = QHBoxLayout(self)  # 水平布局
        # 占位弹簧大小需要和侧边栏宽度相等, 默认已相等
        self.placeholderSpring_ = QSpacerItem(50, 0, QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Minimum)  # 占位弹簧
        self.stackedWidget_ = QStackedWidget(self)  # 多页窗口

        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout.addItem(self.placeholderSpring_)
        self.horizontalLayout.addWidget(self.stackedWidget_)

        self.stackedWidget_.setCurrentIndex(-1)  # 没有初始页
        self.sidebar_.raise_()  # 将侧边栏移到最前方(写在其余控件之后)
        self.resize(600, 400)

    def stackedWidget(self):
        """返回占位弹簧对象"""
        return self.stackedWidget_

    def sidebar(self):
        """返回侧边栏对象"""
        return self.sidebar_

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self.sidebar_.resize(self.sidebar_.width(), self.height())

    def mousePressEvent(self, event):
        super().mousePressEvent(event)
        if event.pos().x() > self.sidebar_.width():
            self.clicked.emit(event.pos())
