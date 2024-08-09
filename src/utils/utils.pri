SOURCES += \
    $$PWD/compositionwindoweffect.cpp \
    $$PWD/dwmwindoweffect.cpp \
    $$PWD/imageutils.cpp \
    $$PWD/mouseeventcolormanagement.cpp \
    $$PWD/theme.cpp \
    $$PWD/themecolormanagement.cpp \
    $$PWD/windowmanager.cpp

HEADERS += \
    $$PWD/compositionwindoweffect.h \
    $$PWD/controlcolors.h \
    $$PWD/dwmwindoweffect.h \
    $$PWD/imageutils.h \
    $$PWD/mouseeventcolormanagement.h \
    $$PWD/precompile.h \
    $$PWD/simpleanimation.h \
    $$PWD/theme.h \
    $$PWD/themecolormanagement.h \
    $$PWD/windowmanager.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/OpenCV/lib/ -lopencv_world4100
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/OpenCV/lib/ -lopencv_world4100d

win32
{
    LIBS += -ladvapi32 -luser32 -ldwmapi
}

INCLUDEPATH += $$PWD/OpenCV/include
DEPENDPATH += $$PWD/OpenCV/include

