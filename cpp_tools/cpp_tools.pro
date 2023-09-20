TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
#        ctimel.cpp \
#        main.cpp \
#        main.c \
#        main_Tlist.c \
#        main_Tlist_test.c \
#        stm.cpp \
#         main_Ttimer.cpp \
#        main_Tvlog.cpp \
#        main_th.cpp \
#        main_ctime.cpp \
 \#        main_stm.cpp
    main_format.cpp

HEADERS += \
    ../include/Tlist_Tapi.h \
    ../include/Tsingle.h \
    ../include/Ttimer.h \
    ../include/Tvlog.h \
    ../include/ctimel.h \
    ../include/format.h \
    ../include/map_th.h \
    ../include/queue_th.h \
    ../include/stm.h \
    ../include/ux_epoll.h \
    ../include/vector_th.h \
    ../include/vpool_th.h \

