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

- 仅支持Windows系统
- 仅支持MSVC编译器编译
- 仅支持Qt 6
- MinGW编译器对Windows API支持非常差，理论上可以使用MinGW编译，但非常不建议
- 部分代码未使用Windows API，支持跨平台，部分代码支持Qt 5，详细请自行测试
- 后续将专为Windows系统开发和更新，不再考虑跨平台、Python语言支持以及适配Qt 5
- 后续将对部分使用`GPL-3.0`协议的代码进行重新实现，待完成后，此仓库会改为`MIT`协议

# OpenCV环境配置

部分控件使用了`OpenCV`库，需要配置`OpenCV`，详细步骤：

- 下载`OpenCV（此仓库使用的是 Windows 64位 4.10版本）`安装包，选择以压缩包方式打开（或已安装）
- 将`....安装包路径(或安装路径)\opencv\build\`中的`include`文件夹拷贝至：`....\QtWidget-FancyUI\src\utils\image_utils\OpenCV\`文件夹中
- 将`....安装包路径(或安装路径)\opencv\build\x64\vc16\`中的`lib`和`bin`文件夹拷贝至：`....\QtWidget-FancyUI\src\utils\image_utils\OpenCV\`文件夹中
- 将`....\QtWidget-FancyUI\src\utils\image_utils\OpenCV\bin`添加到系统环境变量，若已经安装并添加过，请忽略这一步

若使用MinGW编译器，需要自行使用MinGW编译OpenCV源码。（不推荐，过于繁琐）

# 已实现（详见example文件夹）

- 深浅主题切换——支持跟随系统主题自动切换，支持手动切换

- 无边框窗口
  - 基类无边框窗口——仅有关闭按钮
  - 简单的无边框窗口——仅有最小化、最大化、关闭按钮
  - 标准无边框窗口——常见窗口样式：图标、窗口标题、最小化、最大化、关闭按钮
  - 毛玻璃窗口
  - 全透明窗口
  - 跟随系统色窗口
  - 云母窗口——仅支持Windows11系统
  - 云母Alt窗口——仅支持Windows11系统
  - 亚克力窗口——仅支持Windows11系统
  - 微软商店风格窗口
  - NT-QQ风格窗口
  - 自适应布局窗口——根据窗口大小动态调整侧边栏宽度

- 按钮
  - 填充动画按钮

- 标签
  - 圆形图片标签——高质量无锯齿，圆形显示图片，支持高斯模糊（需要配置OpenCV）

- 材质滤镜
  - 高斯模糊滤镜——默认效果为亚克力材质（需要配置OpenCV）
