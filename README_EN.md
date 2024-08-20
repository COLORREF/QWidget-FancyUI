<div align="center">
  <h1>FancyUi - Qt Widget</h1>
</div>


<div align="center">
  <img src="https://img.shields.io/badge/License-GPLv3-green?logoColor=63%2C%20185%2C%2017&label=license&labelColor=63%2C%20185%2C%2017&color=63%2C%20185%2C%2017">
<img src="https://img.shields.io/badge/Language-C++-rgb(243,75,125)">
    <img src="https://img.shields.io/badge/Qt-QMake-rgb(158,106,3)">
    <img src="https://img.shields.io/badge/Qt-Qt%20Widget-63%2C%20185%2C%2017">
</div>
<div align="center">
    <img src="https://img.shields.io/github/stars/BFEMCC/Qt-widget-Fancy_UI?style=default&label=%E2%AD%90%EF%B8%8Fstars">
    <img src="https://img.shields.io/github/forks/BFEMCC/Qt-widget-Fancy_UI?style=default">
    <img src="https://img.shields.io/github/watchers/BFEMCC/Qt-widget-Fancy_UI?style=default">
</div>
<p align="center">
English | <a href="./README.md">简体中文</a>
</p>

# Important Notes

- Only supports Windows OS
- Only supports compilation with MSVC compiler
- Only supports Qt 6
- MinGW compiler has poor support for Windows API. It is theoretically possible to use MinGW for compilation, but it is highly discouraged.
- Some code does not use Windows API, which makes it cross-platform, and some code supports Qt 5. Please test these features yourself.
- Future development and updates will focus exclusively on Windows, without consideration for cross-platform compatibility, Python language support, or Qt 5 adaptation.

# OpenCV Environment Configuration

Some components use the `OpenCV` library, and you need to configure `OpenCV` as follows:

- Download the `OpenCV` installation package, preferably in compressed file format (or use an existing installation). This repository uses the 64-bit Windows version 4.10, but you can choose according to your actual environment.
- Copy the `include` folder from `....path-to-installation-package(or-installation-path)\opencv\build\` to `....\QtWidget-FancyUI\src\utils\OpenCV\`.
- Copy the `lib` and `bin` folders from `....path-to-installation-package(or-installation-path)\opencv\build\x64\vc16\` to `....\QtWidget-FancyUI\src\utils\OpenCV\`.
- Add `....\QtWidget-FancyUI\src\utils\OpenCV\bin` to the system environment variables. If it has already been added, you can skip this step.

If you are using the MinGW compiler, you need to compile the OpenCV source code yourself. (Not recommended, as it is too cumbersome.)

# Implemented Features (See the example folder for details)

- Themes
  - Dark/Light theme switching—automatic switching based on the system theme or manual switching
  - Custom theme colors—customize the theme colors of the controls
- Frameless Windows
  - Base frameless window—only a close button
  - Simple frameless window—minimize, maximize, and close buttons
  - Standard frameless window—common window style: icon, window title, minimize, maximize, and close buttons
  - Frosted glass window
  - Fully transparent window
  - System color window
  - Mica window—Windows 11 only
  - Mica Alt window—Windows 11 only
  - Acrylic window—Windows 11 only
  - Microsoft Store style window
  - NT-QQ style window
  - Adaptive layout window—dynamically adjusts the sidebar width based on the window size
- Basic Controls
  - Radio buttons
  - Checkboxes
  - push buttons
  - Fill animation button
  - Circular image label
- Material Filters
  - Gaussian blur filter—default effect is acrylic material (requires OpenCV configuration)
- `FImage` Image Processing Class (requires OpenCV configuration)
  - Gaussian blur
  - Uniform blur
  - Salt and pepper noise
  - Grayscale conversion
