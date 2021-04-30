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
    capacity_transformer.h \
    common.h \
    converter.h \
    prime.h

