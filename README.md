<div align="center">
  <h1>FancyUi - Qt Widget</h1>
</div>


<div align="center">
  <img src="https://img.shields.io/github/stars/COLORREF/QWidget-FancyUI?style=default&label=%E2%AD%90%EF%B8%8Fstars">
  <img src="https://img.shields.io/github/forks/COLORREF/QWidget-FancyUI?style=default">
  <img src="https://img.shields.io/github/watchers/COLORREF/QWidget-FancyUI?style=default">
  <img src="https://img.shields.io/badge/License-GPLv3-green?logoColor=63%2C%20185%2C%2017&label=License&labelColor=63%2C%20185%2C%2017&color=63%2C%20185%2C%2017">
</div>
<div align="center">
  <img src="https://img.shields.io/badge/Language-C++-rgb(243,75,125)">
  <img src="https://img.shields.io/badge/Language-Python-rgb(53,114,165)">
  <img src="https://img.shields.io/badge/Qt-QMake-rgb(158,106,3)">
  <img src="https://img.shields.io/badge/Qt-Qt%20Widget-63%2C%20185%2C%2017">
</div>

<p align="center">
 简体中文 | <a href="./README_EN.md">English</a>
</p>
<h4>
    控件目录
</h4>
<ul>
  <li>按钮
    <ul>
      <li>悬浮填充按钮 • HoverFillButton</li>
    </ul>
  </li>
  <li>单行文本框</li>
    <ul>
      <li>动态下划线单行文本框 • DynamicUnderlineLineEdit</li>
    </ul>
  <li>其余控件整理中......</li>
</ul>

<hr>
封装和重构了：云母效果窗口、云母Alt效果窗口、Areo效果窗口、修改标题栏颜色的方法、修改标题栏文字颜色的方法以及修改深浅主题的方法，添加了亚克力效果窗口。详见：windowEffect文件夹
<br>
相关文档整理中
<hr>


# 按钮

## 悬浮填充按钮 - HoverFillButton

### GIF示例

<img src="./GIF/HoverFillButton.gif" style="zoom:150%;" />

### 构造函数和枚举类

枚举类：

- `AnimationType` 

  作用域：`HoverFillButtonBase`、`HoverFillButton`

  ```c++
  enum class AnimationType
  {
      CircularFill,      // 进入点圆形填充
      CrossFill,         // 左右两个小圆交叉填充
      DiagonalRectangle, // 斜着的矩形填充
      BottomCircle,      // 底部圆形填充
      LeftRectangle      // 左侧矩形填充
  };
  ```

构造函数：

- 同`QPushButton`类，但额外添加参数：`AnimationType`枚举，用于控制动画类型，默认动画类型为`AnimationType::CircularFill`
- 在designer中通过拖拽或“提升为”使用时，可以通过`void setAnimationType`设置动画类型

### 成员函数说明

<table>
    <thead>
        <tr>
            <th>成员函数</th>
            <th>功能说明</th>
            <th>参数</th>
            <th>参数说明</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="2">void addShadow</td>
            <td rowspan="2">为按钮添加阴影效果</td>
            <td>int</td>
            <td>阴影右偏移量</td>
        </tr>
        <tr>
            <td>int</td>
            <td>阴影下偏移量</td>
        </tr>
        <tr>
            <td rowspan="2">void setTextColor</td>
            <td rowspan="2">设置按钮文字默认颜色和悬浮时颜色</td>
            <td>const QColor&</td>
            <td>默认文字颜色</td>
        </tr>
        <tr>
            <td>const QColor&</td>
            <td>悬浮时文字颜色</td>
        </tr>
        <tr>
            <td>QPalette &palette</td>
            <td>返回按钮对象的QPalette的非const引用</td>
            <td>无</td>
            <td>无</td>
        </tr>
        <tr>
            <td>void refreshRadius</td>
            <td>使用样式表修改按钮的圆角半径后,需调用此函数更新圆角半径</td>
            <td>int</td>
            <td>对应样式表中的圆角半径值</td>
        </tr>
        <tr>
            <td>void setFillSpeed</td>
            <td>控制填充速度</td>
            <td>int</td>
            <td>越小填充越快,最小值为1</td>
        </tr>
        <tr>
            <td>void setFillBrush</td>
            <td>设置填充内容绘制笔刷</td>
            <td>const QBrush &</td>
            <td>自定义的笔刷样式</td>
        </tr>
        <tr>
            <td>void setAnimationType</td>
            <td>设置动画类型,预提供了5种动画</td>
            <td>AnimationType枚举类</td>
            <td>动画类型枚举</td>
        </tr>
    </tbody>
    </table>

# 单行文本框

## 动态下划线单行文本框 - DynamicUnderlineLineEdit

### GIF示例

<img src="./GIF/DynamicUnderlineLineEdit.gif" style="zoom:150%;" />

### 构造函数

- 同`QLineEdit`

### 成员函数说明

<table>
    <thead>
        <tr>
            <th>成员函数</th>
            <th>功能说明</th>
            <th>参数</th>
            <th>参数说明</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="2">void setLinePen</td>
            <td rowspan="2">设置线条的绘制画笔</td>
            <td>const QPen&</td>
            <td>获取焦点时的绘制画笔</td>
        </tr>
        <tr>
            <td>const QPen&</td>
            <td>失去焦点时的绘制画笔</td>
        </tr>
        <tr>
            <td>void setTextColor</td>
            <td>设置输入文字颜色</td>
            <td>const QColor&</td>
            <td>输入文字的颜色</td>
        </tr>
        <tr>
            <td>void setPlaceholderTextColor</td>
            <td>设置预设背景文字颜色</td>
            <td>const QColor &</td>
            <td>背景提示文字颜色</td>
        </tr>
        <tr>
            <td>void setLineSpeed</td>
            <td>设置线条填填充速度</td>
            <td>int</td>
            <td>越小填充越快,最小为1</td>
        </tr>
    </tbody>
</table>



<div align="center">
  <h1>其余控件编写中......</h1>
</div>
