from PySide6.QtWidgets import QApplication
from BorderlessWindow import BorderlessWindow

if __name__ == '__main__':
    a = QApplication()
    w = BorderlessWindow()
    w.show()
    a.exec()
