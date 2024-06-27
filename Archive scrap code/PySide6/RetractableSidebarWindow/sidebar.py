from PySide6.QtCore import QPropertyAnimation, QSize, Signal, Qt, Slot, QAbstractAnimation, QRegularExpression
from PySide6.QtGui import qRgb, QBrush, QPen, QPainter
from PySide6.QtWidgets import QWidget, QVBoxLayout


class Sidebar(QWidget):
    expandStart = Signal()  # 开始展开
    expandFinished = Signal()  # 展开结束
    shrinkStart = Signal()  # 开始收缩
    shrinkFinished = Signal()  # 收缩结束
    finished = Signal()  # 展开或收缩结束

    def __init__(self, parent, initial_width: int = 50):
        super().__init__(parent)
        self.isAnimationRunning = False  # 动画是否正在运行
        self.isExpanded = False  # 是否展开
        self.animation = QPropertyAnimation(self)  # 属性动画
        self.animation.setTargetObject(self)
        self.animation.setPropertyName(bytes("size", "utf-8"))
        self.verticalLayout = QVBoxLayout(self)  # 垂直布局
        # self.increasedWidth = 270  # 展开时增加宽度
        self.initialSize = QSize()  # 初始尺寸
        self.endSize = QSize()  # 终止尺寸
        self.backgroundBrush = QBrush(qRgb(243, 243, 243))  # 背景色绘制笔刷
        self.borderLinePen = QPen(qRgb(229, 229, 229))  # 右侧边界线绘制笔

        self.verticalLayout.setSpacing(4)
        self.verticalLayout.setContentsMargins(5, 5, 5, 5)
        self.animation.setDuration(150)
        self.borderLinePen.setWidth(2)

        self.animation.finished.connect(self.animationFinish)

        # 动画起始高度和父窗口高度一样, 宽度为初始宽度, 之后不再更改宽度
        self.initialSize.setWidth(initial_width)
        self.initialSize.setHeight(self.window().height())

        # 动画结束高度和动画起始高度始终相等, 动画结束宽度为动画起始宽度加上展开增加宽度
        self.endSize.setWidth(self.initialSize.width() + 270)
        self.endSize.setHeight(self.initialSize.height())

        self.resize(initial_width, self.window().height())

    def animationFinish(self):
        if self.initialSize.height() != self.window().height():
            # 动画结束, 高度和父窗口不等, 说明动画进行的过程中改变了窗口大小, 需重新设置高度
            self.initialSize.setHeight(self.window().height())
            self.endSize.setHeight(self.initialSize.height())
            self.animation.setStartValue(self.initialSize)
            self.animation.setEndValue(self.endSize)
            self.resize(self.width(), self.window().height())
        self.isExpanded = not self.isExpanded
        self.isAnimationRunning = False
        if self.isExpanded:
            self.expandFinished.emit()
        else:
            self.shrinkFinished.emit()
        self.finished.emit()

    def addWidget(self, widget: QWidget):
        """添加控件"""
        self.verticalLayout.addWidget(widget)

    def addItem(self, item):
        """添加弹簧"""
        self.verticalLayout.addItem(item)

    def setIncreasedWidth(self, increased_width: int):
        """设置展开增加宽度"""
        self.endSize.setWidth(self.endSize.width() + increased_width - self.endSize.width()+self.initialSize.width())

    def setExpandTime(self, ms: int):
        """设置展开时间"""
        self.animation.setDuration(ms)

    def setBackgroundBrush(self, brush: QBrush):
        """设置背景色绘制笔刷"""
        self.backgroundBrush = brush

    def setBorderLinePen(self, pen: QPen):
        """设置右侧边界线绘制笔"""
        self.borderLinePen = pen

    def childrenCumulativeHeight(self) -> int:
        """计算子控件的累计总高度，包括布局边距和控件间距"""
        cumulative_height = 0  # 累计高度
        # 查找直接子控件
        children = self.findChildren(QWidget, QRegularExpression(), Qt.FindChildOption.FindDirectChildrenOnly)
        for child in children:  # 累加直接子控件高度
            cumulative_height += child.height()
        cumulative_height += self.layout().contentsMargins().top()
        cumulative_height += self.layout().contentsMargins().bottom()
        cumulative_height += self.layout().spacing() * (len(children) - 1)
        return cumulative_height

    @Slot()
    def autoExpand(self):
        """自动控制展开或收缩"""
        self.expand()
        self.shrink()

    @Slot()
    def expand(self):
        """展开"""
        if not self.isExpanded and not self.isAnimationRunning:  # 当前不是展开状态
            self.animation.setDirection(QAbstractAnimation.Direction.Forward)  # 正向
            self.animation.start()
            self.isAnimationRunning = True
            self.expandStart.emit()

    @Slot()
    def shrink(self):
        """收缩"""
        if self.isExpanded and not self.isAnimationRunning:
            self.animation.setDirection(QAbstractAnimation.Direction.Backward)  # 逆向
            self.animation.start()
            self.isAnimationRunning = True
            self.shrinkStart.emit()

    def resizeEvent(self, e):
        # 动画未运行时改变, 父窗口高度改变, 需重设动画起始和结束高度
        if not self.isAnimationRunning:
            self.initialSize.setHeight(self.height())
            self.endSize.setHeight(self.initialSize.height())
            self.animation.setStartValue(self.initialSize)
            self.animation.setEndValue(self.endSize)

    def paintEvent(self, e):
        painter = QPainter(self)
        painter.setBrush(self.backgroundBrush)
        painter.setPen(Qt.PenStyle.NoPen)
        painter.drawRect(self.rect())
        painter.setPen(self.borderLinePen)
        painter.drawLine(self.width(), 0, self.width(), self.height())
