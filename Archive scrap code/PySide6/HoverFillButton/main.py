from PySide6.QtCore import QRect, QPointF
from PySide6.QtGui import QColor, Qt, QRadialGradient, QBrush
from PySide6.QtWidgets import QWidget, QApplication
from HoverFillButton import HoverFillButton


class Ui:
    def __init__(self):
        self.pushButton_6 = None
        self.pushButton_5 = None
        self.pushButton_4 = None
        self.pushButton_3 = None
        self.pushButton_2 = None
        self.pushButton_1 = None

    def setupUi(self, parent: QWidget):
        parent.resize(420, 314)

        self.pushButton_1 = HoverFillButton(parent=parent)
        self.pushButton_2 = HoverFillButton(parent=parent)
        self.pushButton_3 = HoverFillButton(parent=parent)
        self.pushButton_4 = HoverFillButton(parent=parent)
        self.pushButton_5 = HoverFillButton(parent=parent)
        self.pushButton_6 = HoverFillButton(parent=parent)

        self.pushButton_1.setGeometry(QRect(70, 90, 130, 50))
        self.pushButton_2.setGeometry(QRect(210, 90, 130, 50))
        self.pushButton_3.setGeometry(QRect(70, 150, 130, 50))
        self.pushButton_4.setGeometry(QRect(210, 150, 130, 50))
        self.pushButton_5.setGeometry(QRect(70, 210, 130, 50))
        self.pushButton_6.setGeometry(QRect(210, 210, 130, 50))

        self.pushButton_1.setText("CircularFill")
        self.pushButton_2.setText("CrossFill")
        self.pushButton_3.setText("DiagonalRectangle")
        self.pushButton_4.setText("BottomCircle")
        self.pushButton_5.setText("LeftRectangle")
        self.pushButton_6.setText("CircularFill")


if __name__ == '__main__':
    a = QApplication()
    w = QWidget()
    w.setWindowTitle("HoverFillButton")
    w.setObjectName("qw")
    w.setStyleSheet("#qw{background-color:rgb(125,125,125)}")

    ui = Ui()
    ui.setupUi(w)

    for btn in w.findChildren(HoverFillButton):
        btn.setFillSpeed(4)

    ui.pushButton_1.setFillBrush(QColor(255, 124, 118))
    ui.pushButton_1.setStyleSheet("background-color:rgba(240,152,152,50);"
                                  "border-radius:10;")
    ui.pushButton_1.refreshRadius(10)
    ui.pushButton_1.addShadow(0, 0, 100, QColor(255, 150, 150))

    # 默认值 # ui.pushButton_1.setAnimationType(HoverFillButton.AnimationType.CircularFill) # 进入点圆形填充
    ui.pushButton_2.setAnimationType(HoverFillButton.AnimationType.CrossFill)  # 两侧圆形填充
    ui.pushButton_3.setAnimationType(HoverFillButton.AnimationType.DiagonalRectangle)  # 斜着的矩形填充
    ui.pushButton_4.setAnimationType(HoverFillButton.AnimationType.BottomCircle)  # 底部圆形填充
    ui.pushButton_5.setAnimationType(HoverFillButton.AnimationType.LeftRectangle)  # 左侧矩形填充

    # 设置文字颜色示例
    ui.pushButton_2.setTextColor(Qt.GlobalColor.red, Qt.GlobalColor.green)  # 普通状态下的颜色, 悬浮状态下的颜色

    # 设置自定义填充画刷示例
    # 绘制桃红色和淡绿色的渐变背景
    point = QPointF(ui.pushButton_2.width(), ui.pushButton_2.height())  # 渐变中心点

    gradient = QRadialGradient(point, ui.pushButton_2.width() / 1.3, point)  # QRadialGradient(圆形渐变区域中心，渐变区域半径，聚焦点坐标)
    gradient.setColorAt(0.5, QColor(170, 255, 127))
    gradient.setColorAt(1.0, QColor(255, 150, 150))
    ui.pushButton_2.setFillBrush(QBrush(gradient))  # 设置渐变画刷

    w.show()
    a.exec()
