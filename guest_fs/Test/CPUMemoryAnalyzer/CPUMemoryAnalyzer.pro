TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    procssnapshot.cpp \
    logger.cpp \
    cpu_memory_analyzer_utils.cpp \
    testguestfs.cpp \
    timelogger.cpp \
    memusage.cpp

LIBS += -lprocps -lguestfs

HEADERS += \
    procssnapshot.h \
    logger.h \
    cpu_memory_analyzer_utils.h \
    testguestfs.h \
    timelogger.h \
    common.h \
    memsnapshot.h
