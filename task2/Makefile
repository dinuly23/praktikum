CC=g++
CFLAGS=-c -Wall -std=c++14 -isystem $(GTEST_DIR)/include
LDFLAGS=-L/usr/lib -pthread -lgtest -lgmock
SOURCES=main.cpp observer.cpp observer.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main


all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean : 
	rm main.o observer.o main

test:
	$(CXX) -o run_tests $(CPPFLAGS) ./main_test.cpp ./observer.cpp ./test.cpp $(LDFLAGS)

test_clean:
	rm run_tests
