import darkdetect

def Theme():
    theme = darkdetect.theme()
    return "dark" if theme == "dark" else "light"

def is_dark_mode():
    return darkdetect.isDark()

def is_light_mode():
    return darkdetect.isLight()

print(Theme())
