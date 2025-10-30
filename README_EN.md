<div align="center">
  <h1>FancyUi - Qt Widget</h1>
</div>


<div align="center">
  <img src="https://img.shields.io/github/stars/COLORREF/QWidget-FancyUI?style=default&label=%E2%AD%90%EF%B8%8Fstars">
  <img src="https://img.shields.io/github/forks/COLORREF/QWidget-FancyUI?style=default">
  <img src="https://img.shields.io/github/watchers/COLORREF/QWidget-FancyUI?style=default">
  <img src="https://img.shields.io/badge/License-GPLv3-rgb(11,124,189)?style=default">
</div>
<div align="center">
  <img src="https://img.shields.io/badge/Language-C++-rgb(243,75,125)">
  <!-- <img src="https://img.shields.io/badge/Language-Python-rgb(53,114,165)"> -->
  <img src="https://img.shields.io/badge/Language-CMake-rgb(218,52,52)">
  <img src="https://img.shields.io/badge/Qt-%20Widgets-63%2C%20185%2C%2017">
</div>
<p align="center">
English | <a href="./README.md">简体中文</a>
</p>

---

# Update Notes
- A full-scale refactor and update is currently in progress. You can first try out the compilable demo program.
- Complete code refactoring — cleaner, more readable… saying goodbye to spaghetti code 🤔?
- More icons: integrated `IconPark`, `Ant Design`, and `Bootstrap` icon sets, with support for over 5400+ icons.
- Currently supports **Qt 6** and **Windows** only. Planned future updates will include support for **Qt 5**, **Linux**, **Android**, and **WebAssembly**.
- Compared to the previous version, **OpenCV** support and some incomplete classes/features have been temporarily removed.
- Third-party libraries used: ~~[OpenCV](https://github.com/opencv/opencv)~~, [magic-enum](https://github.com/Neargye/magic_enum), [IconPark](https://github.com/bytedance/iconpark), [Bootstrap Icons](https://github.com/twbs/icons), [Ant Design](https://github.com/ant-design/ant-design)
- References and acknowledgements: [SAPC-APCA](https://github.com/Myndex/SAPC-APCA), [apca-w3](https://github.com/Myndex/apca-w3), [WinUI 3](https://github.com/microsoft/microsoft-ui-xaml)
- Replaced `QMake` with `CMake`
- Removed legacy and external Python files from the project

---

# Preview

![HomePreview_Light](./docs/HomePreview_Light.png)

![HomePreview_Dark](.\docs\HomePreview_Dark.png)

# Demo Program Build Guide

Modify the `Qt` path in `CMakeLists.txt`, then you can directly start building in your IDE (Visual Studio, QtCreator, Clion, etc.)!

![SetQtPath](./docs/SetQtPath.png)

---

Last edited: 2025-10-30 by @COLORREF
