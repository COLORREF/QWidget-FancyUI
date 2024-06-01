QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    border.cpp \
    borderlesswindow.cpp \
    main.cpp \
    mainarea.cpp \
    titlebale/closebutton.cpp \
    titlebale/colormanagement.cpp \
    titlebale/maximizebutton.cpp \
    titlebale/minimizebutton.cpp \
    titlebale/simpletitlebar.cpp \
    titlebale/standardtitlebar.cpp \
    titlebale/titlebarbase.cpp \
    titlebale/titlebarbutton.cpp

HEADERS += \
    border.h \
    borderlesswindow.h \
    mainarea.h \
    titlebale/closebutton.h \
    titlebale/colormanagement.h \
    titlebale/maximizebutton.h \
    titlebale/minimizebutton.h \
    titlebale/simpletitlebar.h \
    titlebale/standardtitlebar.h \
    titlebale/titlebarbase.h \
    titlebale/titlebarbutton.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rcc.qrc \
    titlebale/rec.qrc

FORMS += \
    form.ui
