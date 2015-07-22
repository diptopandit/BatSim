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
<<<<<<< HEAD
	$(CC) $(CFLAGS) $< -o $@
=======
	$(CC) $(CFLAGS) $< -o $@ $(LINKFLAGS)
>>>>>>> 7ccaeb82ca3d8943f5564b6cec4bd43c611f7a69

clean:
	rm -fr ./*/*.o $(EXECUTABLE)
