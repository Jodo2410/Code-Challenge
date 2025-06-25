# Makefile for Überprüfe, ob ein String ein Palindrom ist (C++)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = test_runner
SOURCES = test_runner.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test
