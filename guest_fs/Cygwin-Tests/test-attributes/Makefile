CXX= g++
CXXFLAGS= -c -std=c++11 -Wall -W -Wattributes
LDFLAGS=
INCPATH= -I.
SOURCES = \
	main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test-attr-constructor

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o: 
	$(CXX) $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	rm -f *.o 
