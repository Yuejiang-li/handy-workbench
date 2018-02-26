#-------------------------------------------------
#
# Project created by QtCreator 2017-12-24T10:07:40
#
#-------------------------------------------------

QT       += core gui

QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DLL QWT_DLL

LIBS += -L"D:\Qt\Qt5_6_1\5.6\mingw49_32\lib" -lqwtd
LIBS += -L"D:\Qt\Qt5_6_1\5.6\mingw49_32\lib" -lqwt
INCLUDEPATH += D:\Qt\Qt5_6_1\5.6\mingw49_32\include\Qwt


TARGET = myOscilloscope
TEMPLATE = app


SOURCES += \
    datarcvthread.cpp \
    main.cpp \
    plot.cpp \
    threaddata.cpp \
    widget.cpp \
    maintabwidget.cpp \
    signalwidget.cpp

HEADERS  += \
    datarcvthread.h \
    plot.h \
    threaddata.h \
    widget.h \
    maintabwidget.h \
    signalwidget.h
