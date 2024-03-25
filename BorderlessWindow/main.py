from PySide6.QtWidgets import QApplication, QWidget, QPushButton, QGraphicsDropShadowEffect

from BorderlessWindow import BorderlessWindow

if __name__ == '__main__':
    a = QApplication()
    w = BorderlessWindow()
    B = QPushButton()
    w.show()
    a.exec()
