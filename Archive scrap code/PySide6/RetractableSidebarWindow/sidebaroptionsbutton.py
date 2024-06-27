from PySide6.QtCore import Signal, Qt, Slot
from PySide6.QtGui import QBrush, QPen, QPainter, QColor, QPixmap
from PySide6.QtWidgets import QRadioButton, QLabel, QSizePolicy


class SidebarOptionsButton(QRadioButton):
    selectedIndex = Signal(int)

    def __init__(self, parent, index: int = 0, minimum_width: int = 40, fixed_height: int = 36):
        super().__init__(parent)
        self.dis_clicked_Color = QColor(243, 243, 243)
        self.clicked_color = QColor(233, 233, 233)
        self.clicked_enter_color = QColor(237, 237, 237)
        self.dis_clicked_enter_color = QColor(233, 233, 233)
        self.pressColor = QColor(236, 236, 236)
        self.prompt_line_color = QColor(0, 159, 170)
        self.background_brush = QBrush(self.dis_clicked_Color)
        self.icon_label = QLabel(self)
        self.text_label = QLabel(self)
        self.index_ = index  # 索引, 默认0
        self.is_draw_prompt_line = True
        self.fillet_radius = 4

        size_policy = QSizePolicy(QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Fixed)
        size_policy.setHorizontalStretch(0)
        size_policy.setVerticalStretch(0)
        size_policy.setHeightForWidth(self.sizePolicy().hasHeightForWidth())
        self.setSizePolicy(size_policy)
        self.setMinimumWidth(minimum_width)
        self.setFixedHeight(fixed_height)
        self.resize(minimum_width, fixed_height)
        self.icon_label.resize(self.size() / 2)
        self.icon_label.move(int(self.width() / 4), int(self.height() / 4))
        self.icon_label.setScaledContents(True)
        self.text_label.move(self.width() + parent.layout().contentsMargins().right(), int(self.height() / 4))

        self.toggled.connect(self.on_toggled)

    def setDisClickedColor(self, color: QColor):
        """设置未选中时颜色"""
        self.dis_clicked_Color = color

    def setClickedColor(self, color: QColor):
        """设置选中时颜色"""
        self.clicked_color = color

    def setClickedEnterColor(self, color: QColor):  #
        """设置选中时鼠标进入颜色"""
        self.clicked_enter_color = color

    def setDisClickedEnterColor(self, color: QColor):
        """设置未选中时鼠标进入颜色"""
        self.dis_clicked_enter_color = color

    def setPromptLineColor(self, color: QColor):
        """设置选中时的提示线条颜色"""
        self.prompt_line_color = color

    def setDrawPromptLineEnable(self, enable: bool = True):
        """设置是否绘制选中时的提示线条"""
        self.is_draw_prompt_line = enable

    def setFilletRadius(self, radius):
        """设置圆角半径"""
        self.fillet_radius = radius

    def index(self):
        return self.index_

    @Slot()
    def setIndex(self, index: int):
        """设置索引"""
        self.index_ = index

    @Slot()
    def setIcon(self, icon: QPixmap):
        """设置图标"""
        self.icon_label.setPixmap(icon)

    @Slot()
    def setText(self, text: str):
        """设置展开后显示的文字"""
        self.text_label.setText(text)

    @Slot()
    def on_toggled(self, is_click: bool):
        if is_click:
            self.selectedIndex.emit(self.index_)
            self.background_brush.setColor(self.clicked_color)
        else:
            self.background_brush.setColor(self.dis_clicked_Color)
        self.update()

    def hitButton(self, event):
        """有效范围重设"""
        return True

    def enterEvent(self, event):
        super().enterEvent(event)
        if self.isChecked():
            self.background_brush.setColor(self.clicked_enter_color)
        else:
            self.background_brush.setColor(self.dis_clicked_enter_color)
        self.update()

    def leaveEvent(self, event):
        super().leaveEvent(event)
        if self.isChecked():
            self.background_brush.setColor(self.clicked_color)
        else:
            self.background_brush.setColor(self.dis_clicked_Color)
        self.update()

    def mousePressEvent(self, event):
        super().mousePressEvent(event)
        self.background_brush.setColor(self.pressColor)
        self.update()

    def mouseReleaseEvent(self, event):
        super().mouseReleaseEvent(event)
        self.background_brush.setColor(self.clicked_color)
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.RenderHint.Antialiasing)

        # 绘制背景色
        painter.setBrush(self.background_brush)
        painter.setPen(Qt.PenStyle.NoPen)
        painter.drawRoundedRect(self.rect(), self.fillet_radius, self.fillet_radius)

        # 绘制选中提示线条
        if self.isChecked() and self.is_draw_prompt_line:
            pen = QPen(self.prompt_line_color)
            pen.setCapStyle(Qt.PenCapStyle.RoundCap)
            line_width = 3
            pen.setWidth(line_width)
            painter.setPen(pen)
            x = line_width
            y1 = self.height() / 4
            y2 = y1 * 2 + y1
            painter.drawLine(x, int(y1), x, int(y2))
