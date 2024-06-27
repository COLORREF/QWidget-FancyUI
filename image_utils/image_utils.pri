SOURCES += \
    $$PWD/imageutils.cpp

HEADERS += \
    $$PWD/imageutils.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/OpenCV/lib/ -lopencv_world4100
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/OpenCV/lib/ -lopencv_world4100d

INCLUDEPATH += $$PWD/OpenCV/include
DEPENDPATH += $$PWD/OpenCV/include
