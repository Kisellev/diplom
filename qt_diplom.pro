#-------------------------------------------------
#
# Project created by QtCreator 2013-05-29T10:26:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_diplom
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static -lpthread

SOURCES += main.cpp\
        mainwindow.cpp \
    generator.cpp \
    bpsk.cpp \
    myvector.cpp \
    noise.cpp \
    convolutional.cpp \
    golay24.cpp \
    tcontainer.cpp \
    arrow.cpp \
    sett.cpp \
    receiver.cpp \
    graph.cpp

HEADERS  += mainwindow.h \
    generator.h \
    bpsk.h \
    myvector.h \
    noise.h \
    convolutional.h \
    golay24.h \
    tcontainer.h \
    arrow.h \
    sett.h \
    receiver.h \
    graph.h

FORMS    += mainwindow.ui
