QT += core
QT -= gui

TARGET = main
CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    movinglinedetector.cpp \
    ../lib/yzbx_cdnetbenchmark.cpp \
    ../lib/yzbx_frameinput.cpp \
    ../lib/yzbx_utility.cpp \
    ../lib/lbp/histogram.cpp \
    ../lib/lbp/lbp.cpp


unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

HEADERS += \
    movinglinedetector.h \
    ../lib/yzbx_cdnetbenchmark.h \
    ../lib/yzbx_frameinput.h \
    ../lib/yzbx_utility.h \
    ../lib/lbp/histogram.hpp \
    ../lib/lbp/lbp.hpp \
    ../lib/yzbx_config.h

INCLUDEPATH += ../ \
../lib
