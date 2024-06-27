SOURCES += \
    $$PWD/compositionwindoweffect.cpp\
    $$PWD/dwmwindoweffect.cpp\
    $$PWD/windowmanager.cpp\


HEADERS += \
    $$PWD/compositionwindoweffect.h\
    $$PWD/dwmwindoweffect.h\
    $$PWD/windowmanager.h\

win32
{
    LIBS += -luser32 -ldwmapi
}
INCLUDEPATH += ../../src/utils
