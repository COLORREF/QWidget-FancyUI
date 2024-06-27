SOURCES += \
    $$PWD/acryliceffectwindow.cpp \
    $$PWD/adaptivelayoutwindow.cpp \
    $$PWD/areowindow.cpp \
    $$PWD/closebutton.cpp \
    $$PWD/framelesswindow.cpp \
    $$PWD/framelesswindowbase.cpp \
    $$PWD/fullytransparentwindow.cpp \
    $$PWD/lightanddarkwidget.cpp \
    $$PWD/maximizebutton.cpp \
    $$PWD/micaalteffectwindow.cpp \
    $$PWD/micaeffectwindow.cpp \
    $$PWD/minimizebutton.cpp \
    $$PWD/mouseeventcolormanagement.cpp \
    $$PWD/mswindow.cpp \
    $$PWD/ntqqwindow.cpp \
    $$PWD/simpleframelesswindow.cpp \
    $$PWD/simpletitlebar.cpp \
    $$PWD/standardtitlebar.cpp \
    $$PWD/systemcolorwindow.cpp \
    $$PWD/theme.cpp \
    $$PWD/themecolormanagement.cpp \
    $$PWD/titlebarbase.cpp \
    $$PWD/titlebarbutton.cpp \
    $$PWD/toolbutton.cpp \
    $$PWD/transparenteffectwindowbase.cpp \

HEADERS += \
    $$PWD/acryliceffectwindow.h \
    $$PWD/adaptivelayoutwindow.h \
    $$PWD/areowindow.h \
    $$PWD/closebutton.h \
    $$PWD/framelesswindow.h \
    $$PWD/framelesswindowbase.h \
    $$PWD/fullytransparentwindow.h \
    $$PWD/lightanddarkwidget.h \
    $$PWD/maximizebutton.h \
    $$PWD/micaalteffectwindow.h \
    $$PWD/micaeffectwindow.h \
    $$PWD/minimizebutton.h \
    $$PWD/mouseeventcolormanagement.h \
    $$PWD/mswindow.h \
    $$PWD/ntqqwindow.h \
    $$PWD/simpleframelesswindow.h \
    $$PWD/simpletitlebar.h \
    $$PWD/standardtitlebar.h \
    $$PWD/systemcolorwindow.h \
    $$PWD/theme.h \
    $$PWD/themecolormanagement.h \
    $$PWD/titlebarbase.h \
    $$PWD/titlebarbutton.h \
    $$PWD/toolbutton.h \
    $$PWD/transparenteffectwindowbase.h \

RESOURCES += \
    $$PWD/framelesswindowrcc.qrc

win32
{
    LIBS += -ladvapi32
}


include( ../../utils/win32_utils/win32_utils.pri)
