from enum import Enum

from PySide6.QtCore import Qt
from PySide6.QtGui import QMouseEvent, QPaintEvent, QPainter, QColor
from PySide6.QtWidgets import QWidget


class BorderType(Enum):
    top_border = 1  # 上
    bottom_border = 2  # 下
    left_border = 3  # 左
    right_border = 4  # 右
    left_top_border = 5  # 左上
    right_top_border = 6  # 右上
    left_bottom_border = 7  # 左下
    right_bottom_border = 8  # 右下


class Border(QWidget):
    def __init__(self, parent: QWidget, border_type: BorderType):
        super().__init__(parent)
        self.setMouseTracking(True)  # 鼠标追踪

        self.border_type = border_type  # 边框类型
        self.border_width = 5  # 边框宽度
        self.FixedSize()

    def mousePressEvent(self, event: QMouseEvent):
        if self.window().isMaximized():
            return super().mousePressEvent(event)
        if event.button() == Qt.LeftButton:
            match self.border_type:
                case BorderType.top_border:  # 上
                    self.window().windowHandle().startSystemResize(Qt.Edge.TopEdge)
                case BorderType.bottom_border:  # 下
                    self.window().windowHandle().startSystemResize(Qt.Edge.BottomEdge)
                case BorderType.left_border:  # 左
                    self.window().windowHandle().startSystemResize(Qt.Edge.LeftEdge)
                case BorderType.right_border:  # 右
                    self.window().windowHandle().startSystemResize(Qt.Edge.RightEdge)
                case BorderType.left_top_border:  # 左上
                    self.window().windowHandle().startSystemResize(Qt.Edge.LeftEdge | Qt.Edge.TopEdge)
                case BorderType.right_bottom_border:  # 右下
                    self.window().windowHandle().startSystemResize(Qt.Edge.RightEdge | Qt.Edge.BottomEdge)
                case BorderType.left_bottom_border:  # 左下
                    self.window().windowHandle().startSystemResize(Qt.Edge.LeftEdge | Qt.Edge.BottomEdge)
                case BorderType.right_top_border:  # 右上
                    self.window().windowHandle().startSystemResize(Qt.Edge.RightEdge | Qt.Edge.TopEdge)
        return super().mousePressEvent(event)

    def mouseMoveEvent(self, event: QMouseEvent):
        if self.window().isMaximized():
            self.setCursor(Qt.CursorShape.ArrowCursor)
            return super().mouseMoveEvent(event)
        match self.border_type:
            case BorderType.top_border | BorderType.bottom_border:  # 上下
                self.setCursor(Qt.CursorShape.SizeVerCursor)
            case BorderType.left_border | BorderType.right_border:  # 左右
                self.setCursor(Qt.CursorShape.SizeHorCursor)
            case BorderType.left_top_border | BorderType.right_bottom_border:  # 左上右下
                self.setCursor(Qt.CursorShape.SizeFDiagCursor)
            case BorderType.left_bottom_border | BorderType.right_top_border:  # 左下右上
                self.setCursor(Qt.CursorShape.SizeBDiagCursor)
        return super().mouseMoveEvent(event)

    def paintEvent(self, event: QPaintEvent):
        painter = QPainter(self)
        painter.setPen(Qt.PenStyle.NoPen)
        painter.setBrush(QColor(255, 255, 255, 1))
        painter.drawRect(self.rect())

    def FixedSize(self):
        """固定大小"""
        if (self.border_type == BorderType.top_border or
                self.border_type == BorderType.bottom_border):
            return self.setFixedHeight(self.border_width)
        elif (self.border_type == BorderType.left_border or
              self.border_type == BorderType.right_border):
            return self.setFixedWidth(self.border_width)
        else:
            return self.setFixedSize(self.border_width, self.border_width)
