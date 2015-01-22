CXX = g++
CXXFLAGS = -c -std=c++11 -Wall -W
LDFLAGS = 
INCPATH = -I.
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = gen_small

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	rm -f *.o gen_small
