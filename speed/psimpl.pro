TARGET = psimpl-speed
TEMPLATE = app
CONFIG += \
    console \
    qtestlib

SOURCES += \
    main.cpp

HEADERS += \
    ../lib/psimpl.h \
    ../demo/psimpl_reference.h \
    cycle_p.h
