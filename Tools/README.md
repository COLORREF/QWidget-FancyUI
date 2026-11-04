# 项目工具

两个工具都使用 Python 自带的 Tkinter。在项目根目录运行下方命令。可以用 `py -m tkinter` 检查 Tkinter 是否可用。

## 示例代码高亮

```powershell
py .\Tools\render_example_code_gui.py
```

1. 在「示例代码」中编辑 C++ 代码，或打开已有的源码或渲染资源 JSON。
2. 在「注册与配色」中选择类别，每行输入一个符号，设置深浅主题颜色后点击「应用类别」。
3. 点击「生成并查看」，预览双主题效果。可复制或导出格式化 JSON、压缩 JSON，也可导出单主题 HTML。

| 文件 | 格式 |
| --- | --- |
| `ExampleSourceCode.source.json` | 示例名称对应 C++ 字符串或逐行数组，例如 `{"Slider": ["int value = 0;"]}` |
| `ExampleSourceCode.style.json` | `colors` 中的颜色对按浅色、深色排列，使用 `#RRGGBB`。`symbols` 按类别列出名称，例如 `{"namespace": ["fancy::"]}` |
| `Example/Example_Resource/ExampleSourceCode.json` | Qt 读取的渲染结果，例如 `{"Slider": {"Light": "<div>...</div>", "Dark": "<div>...</div>"}}` |

前两个 JSON 随工具放在 `Tools/` 下，启动时自动加载。资源 JSON 可通过界面手动打开。

## SVG 图标枚举

```powershell
py .\Tools\generate_icon_enums_gui.py
```

1. 选择图标根目录，默认是 `src/resource/icons`。填写命名空间，默认是 `fancy`。
2. 点击「检查并生成」。可按分类查看枚举，也可查看完整头文件。
3. 点击「复制当前分页」或「导出头文件」。导出文件包含所有分类。

输入目录格式为 `图标根目录/分类名/图标名.svg`，例如 `src/resource/icons/IconPark/ZoomIn.svg`。分类名成为枚举类型，文件名去掉 `.svg` 后成为枚举成员。名称须以 ASCII 字母开头，只使用 ASCII 字母、数字或下划线。非法名称会列出路径和原因，修正后可重新生成。

输出是 C++ 头文件。每次按名称排序并从 0 编号，增删图标可能改变编号。
