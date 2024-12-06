QT += svgwidgets xml svg

RESOURCES += \
    $$PWD/FancyRcc.qrc

HEADERS += \
    $$PWD/FancyButton.h \
    $$PWD/FancyCheckbox.h \
    $$PWD/FancyIcon.h \
    $$PWD/FancyLabel.h \
    $$PWD/FancyLayout.h \
    $$PWD/FancyProgressBar.h \
    $$PWD/FancyRadiobutton.h \
    $$PWD/FancySidebar.h \
    $$PWD/FancySlider.h \
    $$PWD/FancySvg.h \
    $$PWD/FancyTitlebar.h \
    $$PWD/FancyWidget.h \
    $$PWD/FancyWin32.h \
    $$PWD/FancyAnimation.h \
    $$PWD/FancyStyle.h \
    $$PWD/FancyWindow.h \
    $$PWD/acrylicmaterial.h \
    $$PWD/FancyImage.h \
    $$PWD/FancyTheme.h \
    $$PWD/precompile.h
    $$PWD/roundlabel.h \

SOURCES += \
    $$PWD/FancyButton.cpp \
    $$PWD/FancyCheckbox.cpp \
    $$PWD/FancyIcon.cpp \
    $$PWD/FancyLabel.cpp \
    $$PWD/FancyLayout.cpp \
    $$PWD/FancyProgressBar.cpp \
    $$PWD/FancyRadiobutton.cpp \
    $$PWD/FancySidebar.cpp \
    $$PWD/FancySlider.cpp \
    $$PWD/FancySvg.cpp \
    $$PWD/FancyTitlebar.cpp \
    $$PWD/FancyWidget.cpp \
    $$PWD/FancyWin32.cpp \
    $$PWD/FancyAnimation.cpp \
    $$PWD/FancyStyle.cpp \
    $$PWD/FancyWindow.cpp \
    $$PWD/acrylicmaterial.cpp \
    $$PWD/FancyImage.cpp \
    $$PWD/FancyTheme.cpp
    $$PWD/roundlabel.cpp \


INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
LIBS += -luser32 -ldwmapi -ladvapi32

win32
{
    msvc* {
        # MSVC

        INCLUDEPATH += $$PWD/include/OpenCV/include/MSVC/
        DEPENDPATH  += $$PWD/include/OpenCV/include/MSVC/
        INCLUDEPATH += $$PWD/include/OpenCV/include/MSVC/opencv2
        DEPENDPATH  += $$PWD/include/OpenCV/include/MSVC/opencv2
        win32:CONFIG(release, debug|release): LIBS +=  -L$$PWD/include/OpenCV/lib/MSVC -lopencv_world4100
        else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/include/OpenCV/lib/MSVC -lopencv_world4100d
    }
    else {
        # MinGW

        INCLUDEPATH += $$PWD/include/OpenCV/include/MinGW/
        DEPENDPATH  += $$PWD/include/OpenCV/include/MinGW/
        INCLUDEPATH += $$PWD/include/OpenCV/include/MinGW/opencv2
        DEPENDPATH  += $$PWD/include/OpenCV/include/MinGW/opencv2

        win32:CONFIG(release, debug|release): LIBS += $$PWD/include/OpenCV/lib/MinGW/release/libopencv_*.a
        else:win32:CONFIG(debug, debug|release): LIBS += $$PWD/include/OpenCV/lib/MinGW/debug/libopencv_*.a

    }
}
