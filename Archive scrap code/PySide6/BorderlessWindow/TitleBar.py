from PySide6.QtCore import Qt, Slot
from PySide6.QtWidgets import QHBoxLayout, QLabel, QPushButton, QSizePolicy, QSpacerItem, QWidget
from Border import Border


class TitleBar(QWidget):
    """标题栏"""

    def __init__(self, parent):
        super().__init__(parent)
        self.isMax = False

        self.setFixedHeight(28)

        self.horizontalLayout = QHBoxLayout(self)
        self.horizontalLayout.setSpacing(5)
        self.horizontalLayout.setContentsMargins(10, 0, 10, 0)

        self.title_label = QLabel(text="标题", parent=self)
        self.horizontalLayout.addWidget(self.title_label)

        self.horizontalSpacer = QSpacerItem(214, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
        self.horizontalLayout.addItem(self.horizontalSpacer)

        self.mini_button = QPushButton(text="🗕", parent=self)
        self.horizontalLayout.addWidget(self.mini_button)

        self.max_button = QPushButton(text="🗖", parent=self)
        self.horizontalLayout.addWidget(self.max_button)

        self.close_button = QPushButton(text="🗙", parent=self)
        self.horizontalLayout.addWidget(self.close_button)

        self.mini_button.setFixedSize(35, 20)
        self.max_button.setFixedSize(35, 20)
        self.close_button.setFixedSize(35, 20)

        self.mini_button.setStyleSheet("QPushButton {"
                                       "    color: black;"
                                       "    border: none;"
                                       "    background-color: rgba(0,0,0,0); "
                                       "    border-radius: 5px"
                                       "}"
                                       "QPushButton:hover {"
                                       "    background-color: rgba(209,214,219,255);"
                                       "}")

        self.max_button.setStyleSheet("QPushButton {"
                                      "    color: black;"
                                      "    border: none;"
                                      "    background-color: rgba(0,0,0,0); "
                                      "    border-radius: 5px"
                                      "}"
                                      "QPushButton:hover {"
                                      "    background-color: rgba(209,214,219,255);"
                                      "}")

        self.close_button.setStyleSheet("QPushButton {"
                                        "    color: black;"
                                        "    border: none;"
                                        "    background-color: rgba(0,0,0,0); "
                                        "    border-radius: 5px"
                                        "}"
                                        "QPushButton:hover {"
                                        "    color: white;"
                                        "    background-color: rgba(196,43,28,255);"
                                        "}")

        self.mini_button.clicked.connect(self.window().showMinimized)
        self.max_button.clicked.connect(self.MaximizeButtonClicked)
        self.close_button.clicked.connect(self.window().close)

    def mouseMoveEvent(self, event):
        if event.buttons() & Qt.MouseButton.LeftButton:
            if self.window().isMaximized():
                self.max_button.setText("🗖")
                for border in self.window().findChildren(Border):
                    if border.isHidden():
                        border.show()
                self.window().layout().setContentsMargins(20, 0, 20, 20)
            self.window().windowHandle().startSystemMove()
            self.isMax = False

    def mouseDoubleClickEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            self.MaximizeButtonClicked()

    @Slot()
    def MaximizeButtonClicked(self):
        if not self.window().isMaximized():
            self.max_button.setText("🗗")
            self.window().showMaximized()  # 区别于C++，这里运行的是重写的showMaximized
            self.window().update()
        else:
            self.max_button.setText("🗖")
            self.window().showNormal()  # 区别于C++，这里运行的是重写的showNormal
            self.window().update()
        self.isMax = True
