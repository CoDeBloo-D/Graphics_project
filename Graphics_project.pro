#-------------------------------------------------
#
# Project created by QtCreator 2019-10-25T17:43:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphics_project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    circle.cpp \
    controlpanel.cpp \
    curve.cpp \
    drawpad.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
    oval.cpp \
    polygon.cpp \
    rectangle.cpp \
    shape.cpp

HEADERS += \
    circle.h \
    controlpanel.h \
    curve.h \
    drawpad.h \
    line.h \
    mainwindow.h \
    oval.h \
    polygon.h \
    rectangle.h \
    shape.h

FORMS += \
    controlpanel.ui \
    drawpad.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
