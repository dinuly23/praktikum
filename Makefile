CC=g++
CFLAGS=-c -Wall -std=c++14
SOURCES=main.cpp factory.cpp objects.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main


all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean : 
	rm main.o factory.o main
