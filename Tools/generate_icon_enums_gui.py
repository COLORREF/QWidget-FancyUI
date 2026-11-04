"""Generate FancyUI icon enums from a directory of SVG collections."""

import re
from pathlib import Path
from textwrap import dedent

ROOT = Path(__file__).resolve().parents[1]
DEFAULT_ICONS = ROOT / 'src/resource/icons'

RESERVED = set("""
alignas alignof and and_eq asm auto bitand bitor bool break case catch char char8_t
char16_t char32_t class compl concept const consteval constexpr constinit const_cast
continue co_await co_return co_yield decltype default delete do double dynamic_cast
else enum explicit export extern false float for friend goto if inline int long mutable
namespace new noexcept not not_eq nullptr operator or or_eq private protected public
register reinterpret_cast requires return short signed sizeof static static_assert
static_cast struct switch template this thread_local throw true try typedef typeid
typename union unsigned using virtual void volatile wchar_t while xor xor_eq
import module final override signals slots emit foreach forever SIGNAL SLOT
""".split())


def identifier_error(name):
    if not re.fullmatch(r'[A-Za-z][A-Za-z0-9_]*', name):
        return '名称须以 ASCII 字母开头，且只包含 ASCII 字母、数字或下划线'
    if '__' in name or name in RESERVED:
        return 'C++ 保留名称、关键字或项目宏'
    return None


def scan_icons(root):
    groups, errors = {}, []
    seen_groups = set()
    for folder in sorted(root.iterdir(), key=lambda path: (path.name.casefold(), path.name)):
        if not folder.is_dir():
            if folder.name.lower().endswith('.svg'):
                errors.append(f'{folder.name}：SVG 须放在分类文件夹中')
            continue
        files = sorted((p for p in folder.rglob('*') if p.is_file() and p.name.lower().endswith('.svg')),
                       key=lambda path: (path.name.casefold(), path.name))
        if not files:
            continue
        problem = identifier_error(folder.name)
        if folder.name == 'IconStyle':
            problem = '与生成的 IconStyle 类型重名'
        if folder.name.casefold() in seen_groups:
            problem = '分类名称忽略大小写后重复'
        seen_groups.add(folder.name.casefold())
        if problem:
            errors.append(f'{folder.name}/：{problem}')
        names, seen_names = [], set()
        for file in files:
            name = file.stem
            problem = identifier_error(name)
            if file.parent != folder:
                problem = 'SVG 须直接位于分类文件夹内'
            elif file.suffix != '.svg':
                problem = '扩展名必须为小写 .svg'
            elif name == folder.name:
                problem = '与所属枚举类型重名'
            elif name.casefold() in seen_names:
                problem = '同一分类中图标名忽略大小写后重复'
            seen_names.add(name.casefold())
            if problem:
                errors.append(f'{file.relative_to(root).as_posix()}：{problem}')
            else:
                names.append(name)
        groups[folder.name] = names
    if not groups and not errors:
        errors.append('未找到含 SVG 的分类文件夹')
    return groups, errors


def enum_lines(name, members):
    lines = [f'    enum class {name} : unsigned int', '    {']
    lines.extend(f'        {member},' for member in members)
    lines.extend(['    };'])
    return lines


def generate_header(groups, namespace='fancy'):
    if any(identifier_error(part) for part in namespace.split('::')):
        raise ValueError('命名空间必须是合法的 C++ 标识符，可用 :: 分隔多级名称')
    lines = ['// 此文件中的枚举类由 Python 脚本自动生成', '', f'namespace {namespace}', '{']
    lines.extend(enum_lines('IconStyle', groups))
    for name, members in groups.items():
        lines.extend(['', *enum_lines(name, members)])
    lines.extend(['}', ''])
    return '\n'.join(lines)


def main():
    try:
        import tkinter as tk
        from tkinter import filedialog, messagebox, ttk
    except ImportError:
        raise SystemExit('当前 Python 缺少 Tkinter，请安装 Tcl/Tk and IDLE。')

    root = tk.Tk()
    root.title('SVG 图标枚举生成器')
    root.geometry('950x600')
    root.minsize(550, 280)
    root.columnconfigure(0, weight=1)
    root.rowconfigure(3, weight=1)
    folder = tk.StringVar(value=str(DEFAULT_ICONS) if DEFAULT_ICONS.is_dir() else '')
    namespace = tk.StringVar(value='fancy')
    status = tk.StringVar(value='请选择图标根目录。每个直接子文件夹对应一个枚举。')
    toolbar = ttk.Frame(root, padding=8)
    toolbar.grid(row=0, column=0, sticky='ew')

    def choose_folder():
        path = filedialog.askdirectory(parent=root)
        if path:
            folder.set(path)

    ttk.Label(toolbar, text='图标根目录').grid(row=0, column=0, sticky='w')
    ttk.Entry(toolbar, textvariable=folder).grid(row=0, column=1, sticky='ew', padx=6, pady=3)
    ttk.Button(toolbar, text='选择…', command=choose_folder).grid(row=0, column=2)
    ttk.Label(toolbar, text='命名空间').grid(row=1, column=0, sticky='w')
    ttk.Entry(toolbar, textvariable=namespace).grid(row=1, column=1, sticky='ew', padx=6, pady=3)
    toolbar.columnconfigure(1, weight=1)
    ttk.Label(root, textvariable=status, padding=8).grid(row=1, column=0, sticky='ew')
    page_choice = ttk.Combobox(root, state='readonly')
    page_choice.grid(row=2, column=0, sticky='ew', padx=8)
    tabs = ttk.Notebook(root)
    tabs.grid(row=3, column=0, sticky='nsew', padx=8)
    page_ids = {}
    page_contents = {}

    def add_page(label, content):
        pane = ttk.Frame(tabs)
        output = tk.Text(pane, width=1, height=1, wrap='none', font=('Consolas', 11))
        y = ttk.Scrollbar(pane, orient='vertical', command=output.yview)
        x = ttk.Scrollbar(pane, orient='horizontal', command=output.xview)
        output.configure(yscrollcommand=y.set, xscrollcommand=x.set)
        output.grid(row=0, column=0, sticky='nsew')
        y.grid(row=0, column=1, sticky='ns')
        x.grid(row=1, column=0, sticky='ew')
        pane.rowconfigure(0, weight=1)
        pane.columnconfigure(0, weight=1)
        output.insert('1.0', content)
        output.configure(state='disabled')
        tabs.add(pane, text=label)
        page_ids[label] = pane

    def show_pages(pages):
        previous = page_choice.get()
        for pane in page_ids.values():
            pane.destroy()
        page_ids.clear()
        page_contents.clear()
        page_contents.update(pages)
        for label, content in pages.items():
            add_page(label, content)
        page_choice['values'] = list(page_ids)
        label = previous if previous in page_ids else next(iter(page_ids))
        page_choice.set(label)
        tabs.select(page_ids[label])

    def sync_page(event):
        selected = tabs.select()
        if selected:
            page_choice.set(tabs.tab(selected, 'text'))

    tabs.bind('<<NotebookTabChanged>>', sync_page)
    page_choice.bind('<<ComboboxSelected>>', lambda event: tabs.select(page_ids[page_choice.get()]))
    show_pages({'完整头文件': '选择图标根目录后，点击「检查并生成」。'})

    def refresh():
        try:
            if not folder.get().strip():
                raise ValueError('请选择图标根目录')
            groups, errors = scan_icons(Path(folder.get().strip()))
            result = generate_header(groups, namespace.get().strip()) if not errors else None
        except (OSError, ValueError) as error:
            errors, result = [str(error)], None
        if errors:
            show_pages({'检查结果': '\n'.join(errors)})
            status.set(f'发现 {len(errors)} 个问题，请修正后重新生成。')
        else:
            pages = {'完整头文件': result}
            for name, members in groups.items():
                pages[name] = dedent('\n'.join(enum_lines(name, members))) + '\n'
            show_pages(pages)
            status.set(f'已生成 {len(groups)} 个图标类别，共 {sum(map(len, groups.values()))} 个图标。')
        return result

    def copy():
        label = page_choice.get()
        root.clipboard_clear()
        root.clipboard_append(page_contents[label])
        status.set(f'已复制当前分页：{label}')

    def export():
        result = refresh()
        if result is None:
            return
        path = filedialog.asksaveasfilename(parent=root, initialfile='IconEnums.h',
                                          defaultextension='.h', filetypes=[('C++ 头文件', '*.h')])
        if path:
            try:
                Path(path).write_text(result, encoding='utf-8')
            except OSError as error:
                messagebox.showerror('导出失败', str(error), parent=root)
            else:
                status.set('已导出 ' + path)

    buttons = ttk.Frame(root, padding=8)
    buttons.grid(row=4, column=0, sticky='ew')
    for label, command in [('检查并生成', refresh), ('复制当前分页', copy), ('导出头文件', export)]:
        ttk.Button(buttons, text=label, command=command).pack(side='left', padx=4)
    root.mainloop()


if __name__ == '__main__':
    main()
