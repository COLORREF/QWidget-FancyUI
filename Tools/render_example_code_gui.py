"""Tkinter editor for the example-code renderer. Run this file to open the UI."""

import copy
import html
import re
from html.parser import HTMLParser
import json
from pathlib import Path
try:
    import tkinter as tk
    from tkinter import ttk, filedialog, messagebox, colorchooser
except ImportError:
    tk = None

TOOL_DIR = Path(__file__).resolve().parent
CATEGORY_LABELS = {
    'text': '普通文本',
    'keyword': '关键字',
    'type': '类型',
    'namespace': '命名空间',
    'enum': '枚举值',
    'string': '字符串与字符',
    'comment': '注释',
    'number': '数字',
}


def category_label(key):
    return f'{key}（{CATEGORY_LABELS[key]}）' if key in CATEGORY_LABELS else key


KEYWORDS = frozenset("""alignas alignof and and_eq asm auto bitand bitor bool break case catch char
char8_t char16_t char32_t class compl concept const consteval constexpr constinit
const_cast continue co_await co_return co_yield decltype default delete do double
dynamic_cast else enum explicit export extern false float for friend goto if inline
int long mutable namespace new noexcept not not_eq nullptr operator or or_eq private
protected public register reinterpret_cast requires return short signed sizeof static
static_assert static_cast struct switch template this thread_local throw true try
typedef typeid typename union unsigned using virtual void volatile wchar_t while xor xor_eq
""".split())

# Order matters: comments/literals are consumed before registered symbols, so their
# contents can never be recolored. Raw strings use their actual delimiter.
TOKEN = re.compile(r"""
    (?P<comment>//(?:\\\n|[^\n])*|/\*[\s\S]*?(?:\*/|\Z))
  | (?P<raw>(?:u8|u|U|L)?R"(?P<delimiter>[^\s()\\]{0,16})\([\s\S]*?\)(?P=delimiter)")
  | (?P<string>(?:u8|u|U|L)?(?:"(?:\\[\s\S]|[^"\\])*"|'(?:\\[\s\S]|[^'\\])*'))
  | (?P<number>(?:
        0[xX][\da-fA-F](?:[\da-fA-F'])*(?:\.[\da-fA-F']*)?(?:[pP][+-]?\d[\d']*)?
      | 0[bB][01][01']*
      | (?:\d[\d']*(?:\.[\d']*)?|\.\d[\d']*)(?:[eE][+-]?\d[\d']*)?
    )(?:_[\w]+|[uUlLfFzZ]*))
  | (?P<identifier>[^\W\d]\w*)
  | (?P<other>[\s\S])
""", re.VERBOSE)


class Renderer:
    """Explicit symbol registration; longest qualified name wins."""

    def __init__(self, config):
        self.colors = {}
        self.symbols = {}
        for category, pair in config["colors"].items():
            self.set_colors(category, *pair)
        for category, names in config.get("symbols", {}).items():
            self.register(category, *names)

    def set_colors(self, category, light, dark):
        if not all(re.fullmatch(r"#[0-9a-fA-F]{6}", c) for c in (light, dark)):
            raise ValueError("Colors must be #RRGGBB")
        self.colors[category] = (light, dark)

    def register(self, category, *names):
        if category not in self.colors:
            raise ValueError("Missing colors for category: " + category)
        for name in names:
            if not re.fullmatch(r"[^\W\d]\w*(?:::[^\W\d]\w*)*(?:::)?", name):
                raise ValueError("Invalid C++ symbol: " + name)
            self.symbols[name] = category
        names = sorted(self.symbols, key=lambda name: (-len(name), name))
        self.symbol_pattern = re.compile(
            "|".join(re.escape(n) + (r"(?!\w)" if not n.endswith("::") else "")
                     for n in names)
        ) if names else None

    def tokens(self, source):
        pos = 0
        while pos < len(source):
            match = TOKEN.match(source, pos)
            category = match.lastgroup
            value = match.group()
            if category == "identifier":
                symbol = getattr(self, "symbol_pattern", None)
                registered = symbol.match(source, pos) if symbol else None
                if registered:
                    value = registered.group()
                    category = self.symbols[value]
                else:
                    category = "keyword" if value in KEYWORDS else "text"
            elif category == "raw":
                category = "string"
            elif category == "other":
                category = "text"
            yield category, value
            pos += len(value)

    def render(self, source, theme):
        if theme not in ("Light", "Dark"):
            raise ValueError("Theme must be Light or Dark")
        index = ("Light", "Dark").index(theme)
        source = source.replace("\r\n", "\n").replace("\r", "\n").expandtabs(4)
        parts = []
        for category, value in self.tokens(source):
            escaped = html.escape(value, quote=True).replace(" ", "&nbsp;").replace("\n", "<br>")
            if category != "text":
                escaped = f'<span style=color:{self.colors[category][index]};>{escaped}</span>'
            parts.append(escaped)
        # The previous renderer used a normal space for the first indentation
        # column. Retain this convention for identical existing Qt rich text.
        body = "".join(parts).replace("<br>&nbsp;", "<br> ")
        color = self.colors["text"][index]
        return (f'<div style="font-family:Consolas,Monaco,monospace; font-size:11pt; '
                f'background-color:rgba(0,0,0,0); color:{color}; padding:8px; '
                f'border-radius:4px;">{body}</div>')


def generate(source, config):
    renderer = Renderer(config)
    result = {}
    for name, code in source.items():
        if isinstance(code, list) and all(isinstance(line, str) for line in code):
            code = "\n".join(code)
        if not isinstance(code, str):
            raise ValueError(f"{name}: expected a string or a list of source lines")
        result[name] = {theme: renderer.render(code, theme) for theme in ("Light", "Dark")}
    return result


def serialize(value, compact=False):
    """Only format JSON: never change whitespace inside the rendered C++ HTML."""
    return json.dumps(value, ensure_ascii=False,
                      **({'separators': (',', ':')} if compact else {'indent': 2}))


class SourceParser(HTMLParser):
    def __init__(self):
        super().__init__(convert_charrefs=True)
        self.parts = []

    def handle_starttag(self, tag, attrs):
        if tag == 'br':
            self.parts.append('\n')

    def handle_data(self, data):
        self.parts.append(data)


def read_sources(data):
    """Accept editable sources or the existing {name: {Light, Dark}} resource."""
    if not isinstance(data, dict):
        raise ValueError('JSON 顶层必须是示例名称到代码的对象')
    result = {}
    for name, code in data.items():
        if isinstance(code, dict):
            if not all(isinstance(code.get(t), str) for t in ('Light', 'Dark')):
                raise ValueError(f'{name}: 渲染资源必须包含 Light 和 Dark 字符串')
            parser = SourceParser()
            parser.feed(code['Light'])
            code = ''.join(parser.parts).replace('\xa0', ' ')
        elif isinstance(code, list) and all(isinstance(line, str) for line in code):
            code = '\n'.join(code)
        if not isinstance(code, str):
            raise ValueError(f'{name}: 代码必须是字符串、逐行数组或 Light/Dark 对象')
        result[name] = code
    return result


class Application:
    def __init__(self, root):
        self.root = root
        source_path = TOOL_DIR / 'ExampleSourceCode.source.json'
        style_path = TOOL_DIR / 'ExampleSourceCode.style.json'
        self.sources = read_sources(self.load(source_path)) if source_path.is_file() else {}
        self.config = self.load(style_path) if style_path.is_file() else {
            'colors': {
                'text': ['#080808', '#BCBEC4'],
                'keyword': ['#0033B3', '#CF8E6D'],
                'type': ['#008080', '#B5B6E3'],
                'namespace': ['#008080', '#B5B6E3'],
                'enum': ['#871094', '#C77DBB'],
                'string': ['#067D17', '#6AAB73'],
                'comment': ['#8C8C8C', '#7A7E85'],
                'number': ['#1750EB', '#2AACB8'],
            },
            'symbols': {},
        }
        self.current = None
        self.dirty = False
        root.title('示例代码高亮工具')
        root.geometry('1150x820')
        root.protocol('WM_DELETE_WINDOW', self.close)
        self.status = tk.StringVar(value='就绪：可通过顶部按钮打开代码 JSON 或导入注册配置')
        toolbar = ttk.Frame(root, padding=6)
        toolbar.pack(fill='x')
        for label, command in [('打开代码/资源 JSON', self.open_sources),
                               ('保存原始代码', self.save_sources),
                               ('导入注册配置', self.open_config),
                               ('保存注册配置', self.save_config)]:
            ttk.Button(toolbar, text=label, command=lambda c=command: self.safe(c)).pack(side='left', padx=3)
        self.tabs = ttk.Notebook(root)
        self.tabs.pack(fill='both', expand=True, padx=6)
        self.build_editor()
        self.build_registry()
        self.build_result()
        ttk.Label(root, textvariable=self.status, padding=6).pack(fill='x')
        self.refresh_names()
        self.refresh_categories()
        self.select_name(next(iter(self.sources), None))

    @staticmethod
    def load(path):
        return json.loads(Path(path).read_text(encoding='utf-8-sig'))

    def safe(self, command):
        try:
            command()
        except (ValueError, TypeError, KeyError, OSError, tk.TclError) as error:
            messagebox.showerror('操作失败', str(error), parent=self.root)

    def button(self, frame, label, command):
        ttk.Button(frame, text=label, command=lambda: self.safe(command)).pack(side='left', padx=3, pady=3)

    @staticmethod
    def text_widget(parent, editable=True):
        frame = ttk.Frame(parent)
        frame.pack(fill='both', expand=True)
        text = tk.Text(frame, wrap='none', width=1, height=1, undo=editable, font=('Consolas', 11))
        y = ttk.Scrollbar(frame, orient='vertical', command=text.yview)
        x = ttk.Scrollbar(frame, orient='horizontal', command=text.xview)
        text.configure(yscrollcommand=y.set, xscrollcommand=x.set)
        text.grid(row=0, column=0, sticky='nsew')
        y.grid(row=0, column=1, sticky='ns')
        x.grid(row=1, column=0, sticky='ew')
        frame.rowconfigure(0, weight=1)
        frame.columnconfigure(0, weight=1)
        if not editable:
            text.configure(state='disabled')
        return text

    @staticmethod
    def put(text, value, readonly=False):
        text.configure(state='normal')
        text.delete('1.0', 'end')
        text.insert('1.0', value)
        if readonly:
            text.configure(state='disabled')

    def build_editor(self):
        page = ttk.Frame(self.tabs, padding=6)
        self.tabs.add(page, text='1. 示例代码')
        bar = ttk.Frame(page)
        bar.pack(fill='x')
        ttk.Label(bar, text='示例').pack(side='left')
        self.names = ttk.Combobox(bar, state='readonly', width=28)
        self.names.pack(side='left', padx=4)
        self.names.bind('<<ComboboxSelected>>', lambda e: self.select_name(self.names.get()))
        self.name = tk.StringVar()
        ttk.Entry(bar, textvariable=self.name, width=26).pack(side='left')
        self.button(bar, '新增/另存为', self.add_source)
        self.button(bar, '重命名', self.rename_source)
        self.button(bar, '删除', self.delete_source)
        ttk.Label(page, text='在这里输入 C++；切换示例会保留编辑。新增/另存为使用右侧名称。').pack(anchor='w', pady=5)
        self.editor = self.text_widget(page)
        self.editor.bind('<<Modified>>', self.on_edit)
        self.button(page, '生成并查看', self.render)

    def on_edit(self, event):
        if self.editor.edit_modified():
            self.dirty = True
            self.status.set('代码已修改；结果需重新生成，原始代码尚未保存')
            self.editor.edit_modified(False)

    def commit(self):
        if self.current is not None:
            self.sources[self.current] = self.editor.get('1.0', 'end-1c')

    def refresh_names(self):
        self.names['values'] = list(self.sources)

    def select_name(self, name):
        self.commit()
        self.current = name
        self.names.set(name or '')
        self.name.set(name or '')
        self.put(self.editor, self.sources.get(name, ''))
        self.editor.edit_reset()
        self.editor.edit_modified(False)

    def checked_name(self):
        name = self.name.get().strip()
        if not name:
            raise ValueError('请输入示例名称，如 Slider')
        if name in self.sources:
            raise ValueError('该名称已存在，请使用其他名称')
        return name

    def add_source(self):
        name = self.checked_name()
        self.commit()
        self.sources[name] = self.editor.get('1.0', 'end-1c')
        self.refresh_names()
        self.select_name(name)
        self.dirty = True

    def rename_source(self):
        name = self.checked_name()
        if self.current is None:
            raise ValueError('请先选择示例')
        self.commit()
        self.sources = {name if key == self.current else key: value for key, value in self.sources.items()}
        self.current = None
        self.refresh_names()
        self.select_name(name)
        self.dirty = True

    def delete_source(self):
        if self.current and messagebox.askyesno('删除', f'删除示例 {self.current}？', parent=self.root):
            del self.sources[self.current]
            self.current = None
            self.refresh_names()
            self.select_name(next(iter(self.sources), None))
            self.dirty = True

    def build_registry(self):
        page = ttk.Frame(self.tabs, padding=6)
        self.tabs.add(page, text='2. 注册与配色')
        bar = ttk.Frame(page)
        bar.pack(fill='x')
        ttk.Label(bar, text='类别（可输入新类别）').pack(side='left')
        self.category = ttk.Combobox(bar, width=25)
        self.category.pack(side='left', padx=5)
        self.category.bind('<<ComboboxSelected>>', lambda e: self.show_category())
        self.light = tk.StringVar()
        self.dark = tk.StringVar()
        for label, variable in [('Light', self.light), ('Dark', self.dark)]:
            ttk.Label(bar, text=label).pack(side='left')
            ttk.Entry(bar, textvariable=variable, width=10).pack(side='left', padx=3)
            self.button(bar, '选色', lambda v=variable: self.choose_color(v))
        ttk.Label(page, text='符号每行一个。type：类型；namespace：作用域前缀（例如 demo::）；enum：枚举。\n'
                             'keyword/string/comment/number/text 配色也可修改；C++ 关键字自动识别。\n'
                             '修改后点击“应用类别”，再切换类别。新类别需填写两套颜色。').pack(anchor='w', pady=8)
        self.symbols = self.text_widget(page)
        self.button(page, '应用类别（含符号与颜色）', self.apply_category)

    def refresh_categories(self):
        self.category['values'] = [category_label(key) for key in self.config['colors']]
        key = 'type' if 'type' in self.config['colors'] else next(iter(self.config['colors']))
        self.category.set(category_label(key))
        self.show_category()

    def category_key(self):
        value = self.category.get().strip()
        return next((key for key in CATEGORY_LABELS if category_label(key) == value), value)

    def show_category(self):
        category = self.category_key()
        light, dark = self.config['colors'].get(category, ['#008080', '#B5B6E3'])
        self.light.set(light)
        self.dark.set(dark)
        self.put(self.symbols, '\n'.join(self.config.get('symbols', {}).get(category, [])))

    def choose_color(self, variable):
        color = colorchooser.askcolor(color=variable.get() or '#008080', parent=self.root)[1]
        if color:
            variable.set(color.upper())

    def apply_category(self):
        category = self.category_key()
        if not category:
            raise ValueError('请输入类别名称')
        config = copy.deepcopy(self.config)
        names = list(dict.fromkeys(self.symbols.get('1.0', 'end-1c').split()))
        # A symbol has one category; moving it removes its previous registration.
        for key, values in config.setdefault('symbols', {}).items():
            config['symbols'][key] = [n for n in values if n not in names]
        config['colors'][category] = [self.light.get(), self.dark.get()]
        config['symbols'][category] = names
        Renderer(config)
        self.config = config
        self.category['values'] = [category_label(key) for key in config['colors']]
        self.category.set(category_label(category))
        self.dirty = True
        self.status.set(f'已应用 {category}：{len(names)} 个符号；请重新生成，配置可单独保存')

    def build_result(self):
        page = ttk.Frame(self.tabs, padding=6)
        self.tabs.add(page, text='3. 预览与导出')
        bar = ttk.Frame(page)
        bar.pack(fill='x')
        self.scope = tk.StringVar(value='当前示例')
        self.mode = tk.StringVar(value='JSON 格式化')
        for variable, values in [(self.scope, ['当前示例', '全部示例']),
                                  (self.mode, ['JSON 格式化', 'JSON 压缩', 'Light HTML', 'Dark HTML'])]:
            box = ttk.Combobox(bar, textvariable=variable, values=values, state='readonly', width=18)
            box.pack(side='left', padx=3)
            box.bind('<<ComboboxSelected>>', lambda e: self.safe(self.render))
        self.button(bar, '生成/刷新', self.render)
        self.button(bar, '复制结果', self.copy_result)
        self.button(bar, '导出结果', self.export_result)
        ttk.Label(page, text='JSON 输出保留示例名称及 Light/Dark；压缩只移除 JSON 空白，不改变 C++ 缩进。').pack(anchor='w', pady=3)
        panes = ttk.Panedwindow(page, orient='vertical')
        panes.pack(fill='both', expand=True)
        preview = ttk.Frame(panes)
        panes.add(preview, weight=1)
        ttk.Label(preview, text='当前示例配色预览（Tk 文本模拟；Qt 最终排版以 HTML 为准）').pack(anchor='w')
        row = ttk.Panedwindow(preview, orient='horizontal')
        row.pack(fill='both', expand=True)
        self.previews = {}
        for theme in ('Light', 'Dark'):
            frame = ttk.LabelFrame(row, text=theme)
            row.add(frame, weight=1)
            self.previews[theme] = self.text_widget(frame, False)
        output = ttk.LabelFrame(panes, text='生成结果（可选择文本）')
        panes.add(output, weight=2)
        self.output = self.text_widget(output, False)

    def result(self):
        self.commit()
        if self.current is None:
            raise ValueError('请先新增或选择一个示例')
        mode = self.mode.get()
        if mode.endswith('HTML'):
            return Renderer(self.config).render(self.sources[self.current], mode.split()[0])
        sources = self.sources if self.scope.get() == '全部示例' else {self.current: self.sources[self.current]}
        return serialize(generate(sources, self.config), mode == 'JSON 压缩')

    def render(self):
        result = self.result()
        self.put(self.output, result, True)
        renderer = Renderer(self.config)
        code = self.sources[self.current].replace('\r\n', '\n').replace('\r', '\n').expandtabs(4)
        for index, theme in enumerate(('Light', 'Dark')):
            text = self.previews[theme]
            self.put(text, '')
            text.configure(bg='#FFFFFF' if index == 0 else '#1E1F22', fg=renderer.colors['text'][index])
            for category, colors in renderer.colors.items():
                text.tag_configure(category, foreground=colors[index])
            for category, value in renderer.tokens(code):
                text.insert('end', value, (category,))
            text.configure(state='disabled')
        self.tabs.select(2)
        self.status.set(f'已生成：{self.current} / {self.scope.get()} / {self.mode.get()}')
        return result

    def copy_result(self):
        self.render()
        self.root.clipboard_clear()
        self.root.clipboard_append(self.output.get('1.0', 'end-1c'))
        self.status.set('已复制最新生成结果')

    def write_dialog(self, value, name, extension='.json'):
        path = filedialog.asksaveasfilename(parent=self.root, initialfile=name,
                                          defaultextension=extension,
                                          filetypes=[(extension, '*' + extension), ('所有文件', '*.*')])
        if path:
            Path(path).write_text(value + '\n', encoding='utf-8')
            self.status.set('已保存：' + path)

    def export_result(self):
        self.render()
        extension = '.html' if self.mode.get().endswith('HTML') else '.json'
        self.write_dialog(self.output.get('1.0', 'end-1c'), 'ExampleSourceCode' + extension, extension)

    def open_sources(self):
        if self.dirty and not messagebox.askyesno('打开文件', '当前存在编辑，请确认已保存代码和配置。继续打开？', parent=self.root):
            return
        path = filedialog.askopenfilename(parent=self.root, filetypes=[('JSON', '*.json')])
        if path:
            sources = read_sources(self.load(path))
            self.sources = sources
            self.current = None
            self.refresh_names()
            self.select_name(next(iter(sources), None))
            self.status.set('已载入：' + path)

    def save_sources(self):
        self.commit()
        self.write_dialog(serialize({key: value.split('\n') for key, value in self.sources.items()}),
                          'ExampleSourceCode.source.json')

    def open_config(self):
        path = filedialog.askopenfilename(parent=self.root, filetypes=[('JSON', '*.json')])
        if path:
            config = self.load(path)
            Renderer(config)
            for category in ('text', 'keyword', 'string', 'comment', 'number'):
                if category not in config['colors']:
                    raise ValueError('配置缺少基础配色：' + category)
            self.config = config
            self.refresh_categories()
            self.dirty = True
            self.status.set('配置已载入；请重新生成')

    def save_config(self):
        self.write_dialog(serialize(self.config), 'ExampleSourceCode.style.json')

    def close(self):
        if self.dirty and not messagebox.askyesno('退出', '请确认需要的原始代码和注册配置已保存。退出？', parent=self.root):
            return
        self.root.destroy()


def main():
    if tk is None:
        raise SystemExit('当前 Python 缺少 Tkinter。请在 Python 安装程序的 Modify 中安装 Tcl/Tk and IDLE，然后重新运行。')
    root = tk.Tk()
    Application(root)
    root.mainloop()


if __name__ == '__main__':
    main()
