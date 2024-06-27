from PySide6 import QtWidgets
from PySide6.QtGui import QFont, QColor, QPixmap
from PySide6.QtWidgets import QGridLayout, QLabel, QSizePolicy, QSpacerItem, QWidget
from sidebaroptionsbutton import SidebarOptionsButton
from sidebarwindow import SidebarWindow


class Ui_page(object):
    """这个类由ui文件生成,删除了部分代码"""

    def setupUi(self, page):
        page.resize(502, 335)
        self.gridLayout = QGridLayout(page)
        self.verticalSpacer_2 = QSpacerItem(20, 141, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        self.gridLayout.addItem(self.verticalSpacer_2, 0, 1, 1, 1)
        self.horizontalSpacer = QSpacerItem(212, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        self.gridLayout.addItem(self.horizontalSpacer, 1, 0, 1, 1)
        self.label = QLabel(page)
        font = QFont()
        font.setPointSize(30)
        self.label.setFont(font)
        self.gridLayout.addWidget(self.label, 1, 1, 1, 1)
        self.horizontalSpacer_2 = QSpacerItem(211, 20, QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Minimum)
        self.gridLayout.addItem(self.horizontalSpacer_2, 1, 2, 1, 1)
        self.verticalSpacer = QSpacerItem(20, 141, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        self.gridLayout.addItem(self.verticalSpacer, 2, 1, 1, 1)


class Widget(SidebarWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.page_ui = Ui_page()

# ##########################################################################  设置stackedWidget内容
        self.page_1 = QWidget(self)
        self.page_2 = QWidget(self)
        self.page_3 = QWidget(self)
        self.page_set = QWidget(self)

        # 设置ui布局
        self.page_ui.setupUi(self.page_1)
        self.page_ui.setupUi(self.page_2)
        self.page_ui.setupUi(self.page_3)
        self.page_ui.setupUi(self.page_set)

        # 向多页窗口添加页面
        self.stackedWidget().addWidget(self.page_1)
        self.stackedWidget().addWidget(self.page_2)
        self.stackedWidget().addWidget(self.page_3)
        self.stackedWidget().addWidget(self.page_set)

        # 设置展示页面中label的内容(展示页面只有一个label, 特殊处理🤔)
        self.page_1.findChildren(QLabel)[0].setText("给up点个😚赞可以吗~~")
        self.page_2.findChildren(QLabel)[0].setText("😚投个币吧~~🪙")
        self.page_3.findChildren(QLabel)[0].setText("要不要😍收藏一下❓")
        self.page_set.findChildren(QLabel)[0].setText("设置~~")

        self.stackedWidget().setCurrentIndex(0)  # 默认显示第一页
        self.stackedWidget().setStyleSheet("background-color: rgb(249,249,249)")  # 简单设置一下背景色


# ################################################################################ 设置侧边栏
        self.btn_page_1 = SidebarOptionsButton(self.sidebar(), 0)
        self.btn_page_2 = SidebarOptionsButton(self.sidebar(), 1)
        self.btn_page_3 = SidebarOptionsButton(self.sidebar(), 2)
        self.verticalSpacer = QSpacerItem(0, 0, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)
        self.btn_page_set = SidebarOptionsButton(self.sidebar(), 3)
        self.btn_expand = SidebarOptionsButton(self.sidebar())

        # 设置选中提示线颜色
        self.btn_page_1.setPromptLineColor(QColor(255, 50, 114))
        self.btn_page_2.setPromptLineColor(QColor(0, 174, 236))
        self.btn_page_3.setPromptLineColor(QColor(255, 198, 46))
        self.btn_page_set.setPromptLineColor(QColor(113, 124, 134))

        # 设置按钮图标
        import rcc
        self.btn_page_1.setIcon(QPixmap(":/button_icon/Like.svg"))
        self.btn_page_2.setIcon(QPixmap(":/button_icon/coins.svg"))
        self.btn_page_3.setIcon(QPixmap(":/button_icon/collect.svg"))
        self.btn_page_set.setIcon(QPixmap(":/button_icon/gear.svg"))
        self.btn_expand.setIcon(QPixmap(":/button_icon/three_lines.svg"))

        # 设置右侧文本
        self.btn_page_1.setText("😚给up点个赞可以吗~~")
        self.btn_page_2.setText("🪙投个币吧~~")
        self.btn_page_3.setText("😍要不要收藏一下❓")
        self.btn_page_set.setText("设置")

        # 添加到sidebar中（Sidebar默认的垂直布局, 注意添加顺序: 从上到下）
        self.sidebar().addWidget(self.btn_page_1)
        self.sidebar().addWidget(self.btn_page_2)
        self.sidebar().addWidget(self.btn_page_3)
        self.sidebar().addItem(self.verticalSpacer)
        self.sidebar().addWidget(self.btn_page_set)
        self.sidebar().addWidget(self.btn_expand)


# ################################################################################ 其他一些设置
        self.btn_page_1.setChecked(True)  # 默认第一个按钮被选中
        self.btn_expand.setFixedSize(40, 36)  # 展开按钮固定大小
        self.btn_expand.setDrawPromptLineEnable(False)  # 展开按钮不绘制选中提示线条
        self.btn_expand.setCheckable(False)  # 展开按钮不可选中(可点击但不可选中)
        self.setMinimumHeight(self.sidebar().childrenCumulativeHeight())
        self.setWindowTitle("伸缩侧边栏窗口")
        self.resize(780, 580)
        self.sidebar().setIncreasedWidth(100)


# ################################################################################ 连接信号和槽
        self.btn_page_1.selectedIndex.connect(self.stackedWidget().setCurrentIndex)
        self.btn_page_2.selectedIndex.connect(self.stackedWidget().setCurrentIndex)
        self.btn_page_3.selectedIndex.connect(self.stackedWidget().setCurrentIndex)
        self.btn_page_set.selectedIndex.connect(self.stackedWidget().setCurrentIndex)
        self.btn_expand.clicked.connect(self.sidebar().autoExpand)
        self.clicked.connect(self.sidebar().shrink)


if __name__ == '__main__':
    app = QtWidgets.QApplication()
    w = Widget()
    w.show()
    app.exec()
