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

# 重要说明

- 仅支持Windows系统，仅支持Qt 6
- 支持MSVC、MinGW，release和debug编译
- 第三方库：[OpenCV](https://github.com/opencv/opencv)、[magic-enum](https://github.com/Neargye/magic_enum)

# OpenCV环境配置

点击此处下载[QWidget-FancyUI.zip——获取完整项目环境](https://github.com/COLORREF/QWidget-FancyUI/releases/tag/example-2.0)，无需手动配置或编译OpenCV。

## 环境变量设置

- MSVC：将`项目路径...\QWidget-FancyUI\src\include\OpenCV\bin\MSVC`添加到环境变量

- MinGW：将`项目路径...\QWidget-FancyUI\src\include\OpenCV\bin\MinGW`添加到环境变量



# 类和支持组件说明

| 类                                  | 说明                                             | 备注                                                         |
| ----------------------------------- | ------------------------------------------------ | ------------------------------------------------------------ |
| `MouseEventColorManagement`         | 提供一些颜色和画刷                               | 专用于标题栏按钮                                             |
| `TitleBarButton`                    | 标题栏按钮基类                                   |                                                              |
| `MinimizeButton`                    | 最小化按钮                                       |                                                              |
| `MaximizeButton`                    | 最大化按钮                                       | 窗口中无法获取Qt消息，使用信号和槽处理消息                   |
| `CloseButton`                       | 关闭按钮                                         |                                                              |
| `TitleBarBase`                      | 标题栏基类                                       | 水平布局，左侧弹簧，右侧仅有关闭按钮                         |
| `SimpleTitleBar`                    | 简单的标题栏                                     | 仅有最大化、最小化、关闭按钮，没有图标和标题                 |
| `TitleTextLabel`                    | 窗口标题标签                                     | 用于显示窗口标题                                             |
| `StandardTitleBar`                  | 标准的标题栏                                     | 从左到右依次：图标、标题、水平弹簧、最小化、最大化、关闭按钮 |
| `WindowManager`                     | 常用Windows-api的简单封装                        |                                                              |
| `WindowsVersion`（结构体）          | 存储Windows系统版本号                            |                                                              |
| `CompositionWindowEffect`           | 设置窗口效果                                     | 系统色、毛玻璃、全透明效果                                   |
| `DwmWindowEffect`                   | 设置窗口效果                                     | 云母、云母Alt、亚克力、边框颜色设置等                        |
| `ThemeColorManagement`              | 背景颜色动画管理类                               |                                                              |
| `SimpleAnimation`                   | 对`QVariantAnimation`的简单封装                  |                                                              |
| `ParallelAnimationGroupPool`        | 基于工厂模式的并行动画组池                       |                                                              |
| `ClickRippleAnimation`              | 涟漪动画                                         |                                                              |
| `FramelessWindowBase`               | 无边框窗口基类                                   | 只有关闭按钮                                                 |
| `SimpleFramelessWindow`             | 简单的无边框窗口                                 | 仅有最大化、最小化、关闭按钮，没有图标和标题                 |
| `TransparentEffectWindowBase`       | 具有透明效果的窗口基类                           |                                                              |
| `AcrylicEffectWindow`               | 亚克力窗口                                       | 仅支持Windows11                                              |
| `FullyTransparentWindow`            | 全透明窗口                                       |                                                              |
| `MicaEffectWindow`                  | 云母效果窗口                                     | 仅支持Windows11                                              |
| `MicaAltEffectWindow`               | 云母Alt效果窗口                                  | 仅支持Windows11                                              |
| `SystemColorWindow`                 | 跟随系统色窗口                                   |                                                              |
| `AreoWindow`                        | 毛玻璃窗口                                       |                                                              |
| `NTQQWindow`                        | NTQQ风格窗口                                     | Windows11侧边栏亚克力效果<br />Windows10侧边栏全透明，效果不佳 |
| `MSWindow`                          | 微软商店风格窗口                                 |                                                              |
| `AdaptiveLayoutWindow`              | 侧边栏自适应布局的微软商店风格窗口               |                                                              |
| `InertiaScrollArea`                 | 垂直布局惯性滚动区域                             | 默认带有控件区域，不可用于ui中“提升为”                       |
| `ListeningSystemTheme`              | 监听系统主题子线程                               |                                                              |
| `Theme`                             | 深浅主题类                                       |                                                              |
| `FancySvg`                          | 提供对svg的基本处理函数                          | 标签值查找替换、颜色修改等                                   |
| `ControlColors`                     | 主题色和控件交互颜色控制类                       |                                                              |
| `ControlState`（结构体）            | 基础的控件状态                                   |                                                              |
| `CheckableControlState`（结构体）   | 可选中的具有指示器的控件状态                     | 如单选框、复选框                                             |
| `UnCheckableControlState`（结构体） | 不可选中控件状态                                 | 如默认的 QPushButton                                         |
| `CheckableWidgetState`（结构体）    | 可选中的以widget为基类，没有选中指示器的控件状态 | 如设置了可选中的 QPushButton                                 |
| `FancyStyleBase`                    | 基础的控件风格                                   | 简单的文字颜色自动随深浅主题更改                             |
| `RadioButtonStyle`                  | 单选框控件风格                                   |                                                              |
| `CheckBoxStyle`                     | 复选框控件风格                                   |                                                              |
| `PushButtonStyleBase`               | 按钮风格基类                                     |                                                              |
| `ThemePushButtonSyle`               | 主题色按钮风格                                   |                                                              |
| `RipplePushButtonStyle`             | 涟漪按钮风格                                     |                                                              |
| `TransparentPushButtonStyle`        | 透明按钮风格                                     |                                                              |
| `ToolButtonStyleBase`               | 工具按钮风格基类                                 | 未完善                                                       |
| `ScrollAreaStyle`                   | 滚动区域风格                                     | 全透明无边框的滚动区                                         |
| `OptionButtonBase`                  | 侧边栏选项按钮                                   | 专用于侧边栏，指示器没有动画                                 |
| `SidebarBase`                       | 侧边栏基类                                       |                                                              |
| `StandardSidebar`                   | 标准侧边栏                                       |                                                              |
| `AnimationOptionButton`             | 动画指示器的侧边栏选项按钮                       |                                                              |
| `AnimationIndicatorSidebar`         | 动画指示器侧侧边栏                               | 仅可添加`AnimationOptionButton`                              |
| `RadioButton`                       | 单选框                                           |                                                              |
| `CheckBox`                          | 复选框                                           |                                                              |
| `PushButton`                        | 普通按钮                                         |                                                              |
| `ThemeButton`                       | 主题色按钮                                       |                                                              |
| `RippleButton`                      | 涟漪按钮                                         |                                                              |
| `TransparentButton`                 | 透明按钮                                         |                                                              |
| `ToolButtonBase`                    | 工具按钮基类                                     | 未完善                                                       |
| `FillAnimationButton`               | 动画填充效果按钮                                 |                                                              |
| `FlowLayout`                        | 流式布局                                         |                                                              |
| `RoundLabel`                        | 圆形图片标签                                     |                                                              |
| `FImage`                            | 结合OpenCV的图片类                               |                                                              |
| `FancyIcon`（枚举类）               | 图标枚举值                                       |                                                              |
| `FancyIconEngine`                   | 图标引擎                                         |                                                              |
| `Icon`                              | 图标类                                           |                                                              |
| `FancyIconWidget`                   | 高清svg显示控件                                  | 支持普通图片格式                                             |

