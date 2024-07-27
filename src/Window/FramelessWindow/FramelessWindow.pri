SOURCES += \
    $$PWD/titlebar.cpp \
    $$PWD/toolbutton.cpp \
    $$PWD/window.cpp

HEADERS += \
    $$PWD/titlebar.h \
    $$PWD/toolbutton.h \
    $$PWD/window.h

RESOURCES += \
    $$PWD/framelesswindowrcc.qrc


INCLUDEPATH += ../../../src
include( ../../utils/utils.pri)

