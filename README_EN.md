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
English | <a href="./README.md">简体中文</a>
</p>


# Important Notes

- Only supports Windows systems and Qt 6
- Supports MSVC and MinGW compilers for both release and debug builds
- Third-party libraries: [OpenCV](https://github.com/opencv/opencv), [magic-enum](https://github.com/Neargye/magic_enum)

# OpenCV Environment Configuration

Click here to download [QWidget-FancyUI.zip — Complete project environment](https://github.com/COLORREF/QWidget-FancyUI/releases/tag/example-2.0), no manual configuration or OpenCV compilation required.

## Environment Variables Setup

- **MSVC**: Add `project_path...\QWidget-FancyUI\src\include\OpenCV\bin\MSVC` to the environment variables.
- **MinGW**: Add `project_path...\QWidget-FancyUI\src\include\OpenCV\bin\MinGW` to the environment variables.

# Class and Component Descriptions

| Class                               | Description                                      | Notes                                                        |
| ----------------------------------- | ------------------------------------------------ | ------------------------------------------------------------ |
| `MouseEventColorManagement`         | Provides colors and brushes                      | Specifically for title bar buttons                           |
| `TitleBarButton`                    | Base class for title bar buttons                 |                                                              |
| `MinimizeButton`                    | Minimize button                                  |                                                              |
| `MaximizeButton`                    | Maximize button                                  | Handles messages using signals and slots, as Qt messages cannot be obtained in windows |
| `CloseButton`                       | Close button                                     |                                                              |
| `TitleBarBase`                      | Base class for title bars                        | Horizontal layout with a spring on the left and a close button on the right |
| `SimpleTitleBar`                    | Simple title bar                                 | Only contains minimize, maximize, and close buttons without icons or titles |
| `TitleTextLabel`                    | Window title label                               | Displays the window title                                    |
| `StandardTitleBar`                  | Standard title bar                               | From left to right: icon, title, horizontal spring, minimize, maximize, and close buttons |
| `WindowManager`                     | Simple wrapper for common Windows APIs           |                                                              |
| `WindowsVersion` (struct)           | Stores Windows system version information        |                                                              |
| `CompositionWindowEffect`           | Sets window effects                              | Includes system colors, acrylic, and full transparency effects |
| `DwmWindowEffect`                   | Sets DWM window effects                          | Includes mica, mica alt, acrylic, border color, etc.         |
| `ThemeColorManagement`              | Manages background color animations              |                                                              |
| `SimpleAnimation`                   | Simple wrapper for `QVariantAnimation`           |                                                              |
| `ParallelAnimationGroupPool`        | Factory-based parallel animation group pool      |                                                              |
| `ClickRippleAnimation`              | Ripple animation                                 |                                                              |
| `FramelessWindowBase`               | Base class for frameless windows                 | Only contains a close button                                 |
| `SimpleFramelessWindow`             | Simple frameless window                          | Contains minimize, maximize, and close buttons without icons or titles |
| `TransparentEffectWindowBase`       | Base class for windows with transparency effects |                                                              |
| `AcrylicEffectWindow`               | Acrylic effect window                            | Supports Windows 11 only                                     |
| `FullyTransparentWindow`            | Fully transparent window                         |                                                              |
| `MicaEffectWindow`                  | Mica effect window                               | Supports Windows 11 only                                     |
| `MicaAltEffectWindow`               | Mica alt effect window                           | Supports Windows 11 only                                     |
| `SystemColorWindow`                 | System color-following window                    |                                                              |
| `AreoWindow`                        | Aero glass effect window                         |                                                              |
| `NTQQWindow`                        | NTQQ-style window                                | Acrylic effect for Windows 11, fully transparent for Windows 10 |
| `MSWindow`                          | Microsoft Store-style window                     |                                                              |
| `AdaptiveLayoutWindow`              | Adaptive layout sidebar in Microsoft Store style |                                                              |
| `InertiaScrollArea`                 | Vertical layout with inertia scrolling           | Includes a default control area, cannot be promoted to UI elements |
| `ListeningSystemTheme`              | Subthread for listening to system theme changes  |                                                              |
| `Theme`                             | Dark and light theme class                       |                                                              |
| `FancySvg`                          | Provides basic SVG processing functions          | Supports tag value replacement, color modifications, etc.    |
| `ControlColors`                     | Manages theme and control interaction colors     |                                                              |
| `ControlState` (struct)             | Basic control state                              |                                                              |
| `CheckableControlState` (struct)    | Checkable control state with indicators          | For example, radio buttons and checkboxes                   |
| `UnCheckableControlState` (struct)  | Uncheckable control state                        | For example, the default `QPushButton`                      |
| `CheckableWidgetState` (struct)     | Checkable widget-based control state without indicators | For example, checkable `QPushButton`                        |
| `FancyStyleBase`                    | Base style for controls                          | Automatically adjusts text color based on light/dark themes  |
| `RadioButtonStyle`                  | Style for radio buttons                          |                                                              |
| `CheckBoxStyle`                     | Style for checkboxes                             |                                                              |
| `PushButtonStyleBase`               | Base style for buttons                           |                                                              |
| `ThemePushButtonSyle`               | Theme color button style                         |                                                              |
| `RipplePushButtonStyle`             | Ripple effect button style                       |                                                              |
| `TransparentPushButtonStyle`        | Transparent button style                         |                                                              |
| `ToolButtonStyleBase`               | Base style for tool buttons                      | Incomplete                                                   |
| `ScrollAreaStyle`                   | Style for scroll areas                           | Fully transparent, borderless scroll area                   |
| `OptionButtonBase`                  | Sidebar option button                            | Specifically for sidebars, indicator without animations      |
| `SidebarBase`                       | Base class for sidebars                          |                                                              |
| `StandardSidebar`                   | Standard sidebar                                 |                                                              |
| `AnimationOptionButton`             | Sidebar option button with animated indicator    |                                                              |
| `AnimationIndicatorSidebar`         | Sidebar with animated indicators                 | Only supports `AnimationOptionButton`                       |
| `RadioButton`                       | Radio button                                     |                                                              |
| `CheckBox`                          | Checkbox                                         |                                                              |
| `PushButton`                        | Push button                                      |                                                              |
| `ThemeButton`                       | Theme color button                               |                                                              |
| `RippleButton`                      | Ripple button                                    |                                                              |
| `TransparentButton`                 | Transparent button                               |                                                              |
| `ToolButtonBase`                    | Base class for tool buttons                      | Incomplete                                                   |
| `FillAnimationButton`               | Button with fill animation effect                |                                                              |
| `FlowLayout`                        | Flow layout                                      |                                                              |
| `RoundLabel`                        | Circular image label                             |                                                              |
| `FImage`                            | OpenCV-based image class                         |                                                              |
| `FancyIcon` (enum class)            | Icon enumeration                                 |                                                              |
| `FancyIconEngine`                   | Icon engine                                      |                                                              |
| `Icon`                              | Icon class                                       |                                                              |
| `FancyIconWidget`                   | High-resolution SVG display widget               | Supports common image formats                                |
