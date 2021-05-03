TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread -L/usr/boost

SOURCES += \
        converter.cpp \
        main.cpp \
        prime.cpp

HEADERS += \
    common.h \
    converter.h \
    prime.h

