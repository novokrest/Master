CXX= g++
CXXFLAGS= -c -std=c++11 -Wall -W
LDFLAGS= -L/usr/local/lib -lguestfs 	\
	-ltirpc \
	-lintl \
	-lxml2 -lz -llzma -liconv -lm \
	-lpcre -lpcrecpp
INCPATH= -I.
SOURCES = \
	main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=simple-test

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o: 
	$(CXX) $(CXXFLAGS) $(INCPATH) $< -o $@

clean:
	rm -f *.o 
