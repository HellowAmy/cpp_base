TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ctimel.cpp \
#        main.cpp \
#        main.c \
    main_Tlist.c \
        stm.cpp

HEADERS += \
    Tlien_c.h \
    ctimel.h \
    map_th.h \
    queue_th.h \
    stm.h \
    vector_th.h
