CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=-pthread -lstdc++
SOURCES=src/battery.cpp src/cell.cpp src/simulator.cpp src/cparser.cpp src/batsim.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=batsim
all: clean build

build: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -fr ./*/*.o $(EXECUTABLE)
