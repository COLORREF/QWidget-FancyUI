- 改写自PyQt-Fluent-Widgets组件库
- 请遵循 GPL-3.0 开源协议
- Qt官方的流式布局示例代码：[Flow Layout Example | Qt Widgets 5.15.16](https://doc.qt.io/qt-5/qtwidgets-layouts-flowlayout-example.html)



- 原作者：bilibili@呆唯男友，uid：471587058
- PyQt-Fluent-Widgets开源地址：https://github.com/zhiyiYo/PyQt-Fluent-Widgets

- 原`Python`代码：

  ```python
  from typing import List
  from PySide6.QtCore import QSize, QPoint, Qt, QRect, QPropertyAnimation, QParallelAnimationGroup, QEasingCurve
  from PySide6.QtWidgets import QLayout, QWidgetItem, QLayoutItem, QApplication, QWidget, QPushButton
  import sys
  
  
  class FlowLayout(QLayout):
      """ Flow layout """
  
      def __init__(self, parent=None, needAni=False, isTight=False):
          """
          Parameters
          ----------
          parent:
              parent window or layout
  
          needAni: bool
              whether to add moving animation
  
          isTight: bool
              whether to use the tight layout when widgets are hidden
          """
          super().__init__(parent)
          self._items = []  # type: List[QLayoutItem]
          self._anis = []
          self._aniGroup = QParallelAnimationGroup(self)
          self._verticalSpacing = 10
          self._horizontalSpacing = 10
          self.duration = 300
          self.ease = QEasingCurve.Linear
          self.needAni = needAni
          self.isTight = isTight
  
      def addItem(self, item):
          self._items.append(item)
  
      def addWidget(self, w):
          super().addWidget(w)
          if not self.needAni:
              return
  
          ani = QPropertyAnimation(w, b'geometry')
          ani.setEndValue(QRect(QPoint(0, 0), w.size()))
          ani.setDuration(self.duration)
          ani.setEasingCurve(self.ease)
          w.setProperty('flowAni', ani)
          self._anis.append(ani)
          self._aniGroup.addAnimation(ani)
  
      def setAnimation(self, duration, ease=QEasingCurve.Linear):
          """ set the moving animation
  
          Parameters
          ----------
          duration: int
              the duration of animation in milliseconds
  
          ease: QEasingCurve
              the easing curve of animation
          """
          if not self.needAni:
              return
  
          self.duration = duration
          self.ease = ease
  
          for ani in self._anis:
              ani.setDuration(duration)
              ani.setEasingCurve(ease)
  
      def count(self):
          return len(self._items)
  
      def itemAt(self, index: int):
          if 0 <= index < len(self._items):
              return self._items[index]
  
          return None
  
      def takeAt(self, index: int):
          if 0 <= index < len(self._items):
              item = self._items[index]  # type: QWidgetItem
              ani = item.widget().property('flowAni')
              if ani:
                  self._anis.remove(ani)
                  self._aniGroup.removeAnimation(ani)
                  ani.deleteLater()
  
              return self._items.pop(index).widget()
  
          return None
  
      def removeWidget(self, widget):
          for i, item in enumerate(self._items):
              if item.widget() is widget:
                  return self.takeAt(i)
  
      def removeAllWidgets(self):
          """ remove all widgets from layout """
          while self._items:
              self.takeAt(0)
  
      def takeAllWidgets(self):
          """ remove all widgets from layout and delete them """
          while self._items:
              w = self.takeAt(0)
              if w:
                  w.deleteLater()
  
      def expandingDirections(self):
          return Qt.Orientation(0)
  
      def hasHeightForWidth(self):
          return True
  
      def heightForWidth(self, width: int):
          """ get the minimal height according to width """
          return self._doLayout(QRect(0, 0, width, 0), False)
  
      def setGeometry(self, rect: QRect):
          super().setGeometry(rect)
          self._doLayout(rect, True)
  
      def sizeHint(self):
          return self.minimumSize()
  
      def minimumSize(self):
          size = QSize()
  
          for item in self._items:
              size = size.expandedTo(item.minimumSize())
  
          m = self.contentsMargins()
          size += QSize(m.left() + m.right(), m.top() + m.bottom())
  
          return size
  
      def setVerticalSpacing(self, spacing: int):
          """ set vertical spacing between widgets """
          self._verticalSpacing = spacing
  
      def verticalSpacing(self):
          """ get vertical spacing between widgets """
          return self._verticalSpacing
  
      def setHorizontalSpacing(self, spacing: int):
          """ set horizontal spacing between widgets """
          self._horizontalSpacing = spacing
  
      def horizontalSpacing(self):
          """ get horizontal spacing between widgets """
          return self._horizontalSpacing
  
      def _doLayout(self, rect: QRect, move: bool):
          """ adjust widgets position according to the window size """
          aniRestart = False
          margin = self.contentsMargins()
          x = rect.x() + margin.left()
          y = rect.y() + margin.top()
          rowHeight = 0
          spaceX = self.horizontalSpacing()
          spaceY = self.verticalSpacing()
  
          for i, item in enumerate(self._items):
              if item.widget() and not item.widget().isVisible() and self.isTight:
                  continue
  
              nextX = x + item.sizeHint().width() + spaceX
  
              if nextX - spaceX > rect.right() and rowHeight > 0:
                  x = rect.x() + margin.left()
                  y = y + rowHeight + spaceY
                  nextX = x + item.sizeHint().width() + spaceX
                  rowHeight = 0
  
              if move:
                  target = QRect(QPoint(x, y), item.sizeHint())
                  if not self.needAni:
                      item.setGeometry(target)
                  elif target != self._anis[i].endValue():
                      self._anis[i].stop()
                      self._anis[i].setEndValue(target)
                      aniRestart = True
  
              x = nextX
              rowHeight = max(rowHeight, item.sizeHint().height())
  
          if self.needAni and aniRestart:
              self._aniGroup.stop()
              self._aniGroup.start()
  
          return y + rowHeight + margin.bottom() - rect.y()
  
  
  class Demo(QWidget):
  
      def __init__(self):
          super().__init__()
          layout = FlowLayout(self, needAni=True)
  
          # customize animation
          layout.setAnimation(250, QEasingCurve.OutQuad)
  
          layout.setContentsMargins(30, 30, 30, 30)
          layout.setVerticalSpacing(20)
          layout.setHorizontalSpacing(10)
  
          layout.addWidget(QPushButton('aiko'))
          layout.addWidget(QPushButton('刘静爱'))
          layout.addWidget(QPushButton('柳井爱子'))
          layout.addWidget(QPushButton('aiko 赛高'))
          layout.addWidget(QPushButton('aiko 太爱啦😘'))
  
          self.resize(250, 300)
          self.setStyleSheet('Demo{background: white} QPushButton{padding: 5px 10px; font:15px "Microsoft YaHei"}')
  
  
  if __name__ == '__main__':
      app = QApplication(sys.argv)
      w = Demo()
      w.show()
      app.exec()
  
  ```

  