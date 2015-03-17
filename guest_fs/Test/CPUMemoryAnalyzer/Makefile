CC= g++
CFLAGS= -c -std=c++11 -Wall
INCLUDE= -I.
LDFLAGS= -lprocps -lguestfs
HEADERS= \
    procssnapshot.h \
    logger.h \
    cpu_memory_analyzer_utils.h \
    testguestfs.h \
    timelogger.h \
    common.h \
    memsnapshot.h
SOURCES= \
    main.cpp \
    procssnapshot.cpp \
    logger.cpp \
    cpu_memory_analyzer_utils.cpp \
    testguestfs.cpp \
    timelogger.cpp \
    memusage.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=CpuMemoryAnalyzer

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o: $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@
