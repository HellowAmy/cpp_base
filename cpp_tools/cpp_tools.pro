TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ctimel.cpp \
        main.cpp \
        stm.cpp

HEADERS += \
    ctimel.h \
    queue_th.h \
    stm.h \
    vector_th.h
