# Makefile for Überprüfe, ob ein String ein Palindrom ist (C++)
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I.
TARGET = test_runner
HEADERS = BerprfeObEinStringEinPalindromIst.h Tests.h
MAIN_SOURCE = test_runner.cpp

# ✅ Nur test_runner.cpp kompilieren - enthält bereits alle Headers
all: $(TARGET)

$(TARGET): $(MAIN_SOURCE) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_SOURCE)

clean:
	rm -f $(TARGET) *.o

test: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

.PHONY: all clean test debug

# ✅ Informationen für Entwickler
help:
	@echo "Verfügbare Targets:"
	@echo "  all     - Kompiliert das Projekt"
	@echo "  test    - Kompiliert und führt Tests aus"
	@echo "  clean   - Löscht alle erzeugten Dateien"
	@echo "  debug   - Kompiliert mit Debug-Informationen"
	@echo "  help    - Zeigt diese Hilfe"

.PHONY: help
