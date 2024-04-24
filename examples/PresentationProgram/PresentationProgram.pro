QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DynamicUnderlineLineEdit/DynamicUnderlineLineEdit.cpp \
    HoverFillButton/custombuttonbase.cpp \
    HoverFillButton/hoverfillbutton.cpp \
    HoverFillButton/hoverfillbuttonbase.cpp \
    main.cpp \
    sidebarwindow/sidebar.cpp \
    sidebarwindow/sidebaroptionsbutton.cpp \
    sidebarwindow/sidebarwindow.cpp \
    widget.cpp

HEADERS += \
    DynamicUnderlineLineEdit/DynamicUnderlineLineEdit.h \
    HoverFillButton/custombuttonbase.h \
    HoverFillButton/hoverfillbutton.h \
    HoverFillButton/hoverfillbuttonbase.h \
    page_ui/ui_DynamicUnderlineLineEdit.h \
    page_ui/ui_HoverFillButton.h \
    page_ui/ui_Set.h \
    sidebarwindow/sidebar.h \
    sidebarwindow/sidebaroptionsbutton.h \
    sidebarwindow/sidebarwindow.h \
    ui_HoverFillButton.h \
    ui_Set.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rec.qrc

DISTFILES +=
