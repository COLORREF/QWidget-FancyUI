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

- Only supports Windows systems
- Only supports compilation with MSVC compiler
- Only supports Qt 6
- MinGW compiler has very poor support for Windows API. Theoretically, it can be used for compilation, but it is highly discouraged.
- Some code does not use Windows API, supports cross-platform, and some code supports Qt 5. Please test in detail.
- Future development and updates will be exclusively for Windows systems, with no further consideration for cross-platform support, Python language support, or Qt 5 compatibility.

# OpenCV Environment Configuration

Some widgets use the `OpenCV` library, so you need to configure `OpenCV`. Detailed steps:

- Download the 'OpenCV' installer and choose to open it as a compressed package (or already installed). This repository uses Windows 64-bit version 4.10, which can be selected according to your environment.
- Copy the `include` folder from `....package path (or installation path)\opencv\build\` to `....\QtWidget-FancyUI\src\utils\OpenCV\`.
- Copy the `lib` and `bin` folders from `....package path (or installation path)\opencv\build\x64\vc16\` to `....\QtWidget-FancyUI\src\utils\OpenCV\`.
- Add `....\QtWidget-FancyUI\src\utils\image_utils\OpenCV\bin` to the system environment variables. If already installed and added, skip this step.

If using the MinGW compiler, you need to compile the OpenCV source code yourself. (Not recommended, too cumbersome)

# Features Implemented (see the example folder for details)

- Light and Dark Theme Switching - Supports automatic switching based on system theme, as well as manual switching
- Frameless Windows
  - Basic Frameless Window - Only has a close button
  - Simple Frameless Window - Has minimize, maximize, and close buttons
  - Standard Frameless Window - Common window style: icon, window title, minimize, maximize, and close buttons
  - Frosted Glass Window
  - Fully Transparent Window
  - System Color Follow Window
  - Mica Window - Only supports Windows 11
  - Mica Alt Window - Only supports Windows 11
  - Acrylic Window - Only supports Windows 11
  - Microsoft Store Style Window
  - NT-QQ Style Window
  - Adaptive Layout Window - Dynamically adjusts sidebar width based on window size
- Buttons
  - Fill Animation Button
- Labels
  - Circular Image Label - High quality without jaggies, circular image display
- Material Filters
  - Gaussian Blur Filter - Default effect is acrylic material (requires OpenCV configuration)
- `FImage `image processing class

  - Gaussian blur
  - Uniform blur
  - Salt and pepper noise
  - Grayscale image conversion
