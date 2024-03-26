from PySide6.QtWidgets import QApplication, QPushButton

from BorderlessWindow import BorderlessWindow

if __name__ == '__main__':
    a = QApplication()
    w = BorderlessWindow()
    B = QPushButton()
    w.show()
    a.exec()
