QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algos.cpp \
    config.cpp \
    gridwidget.cpp \
    main.cpp \
    thinningmainwindow.cpp

HEADERS += \
    algos.h \
    config.h \
    globals.h \
    gridwidget.h \
    thinningmainwindow.h

FORMS += \
    thinningmainwindow.ui

INCLUDEPATH += /usr/include/opencv4
LIBS += -L/usr/include/opencv4 -lopencv_core -lopencv_imgproc -lopencv_ximgproc -lopencv_imgcodecs

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
