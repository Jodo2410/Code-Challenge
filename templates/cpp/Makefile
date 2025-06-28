# C++ Template Challenge Makefile (Separate .h/.cpp Files)
# Template-Version: {{TEMPLATE_VERSION}}
# Kompatibel mit: C++17

# Compiler und Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I.
DEBUG_FLAGS = -g -DDEBUG -O0
RELEASE_FLAGS = -O3 -DNDEBUG -march=native
PROFILE_FLAGS = -pg -O2

# Dateien
TARGET = test_runner
SOURCES = test_runner.cpp Challenge.cpp Tests.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = Challenge.h Tests.h

# Dependencies
DEPS = $(OBJECTS:.o=.d)

# Farben für Output
GREEN = \033[0;32m
YELLOW = \033[1;33m
RED = \033[0;31m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
NC = \033[0m # No Color

# Standard-Target
all: $(TARGET)
	@echo "$(GREEN)✅ C++ Template-Challenge erfolgreich kompiliert!$(NC)"
	@echo "$(YELLOW)📁 Verwende 'make test' zum Ausführen der Tests$(NC)"

# Test Runner kompilieren (mit separaten Object-Dateien)
$(TARGET): $(OBJECTS)
	@echo "$(YELLOW)🔗 Linke Executable...$(NC)"
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "$(GREEN)✅ Linking erfolgreich: $(TARGET)$(NC)"

# Object-Dateien kompilieren
%.o: %.cpp $(HEADERS)
	@echo "$(CYAN)🔧 Kompiliere $<...$(NC)"
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Dependencies einbinden
-include $(DEPS)

# Tests ausführen
test: $(TARGET)
	@echo "$(YELLOW)🧪 Führe C++ Template Tests aus...$(NC)"
	@echo "$(YELLOW)================================================$(NC)"
	./$(TARGET)
	@echo "$(YELLOW)================================================$(NC)"
	@echo "$(GREEN)🎉 Tests abgeschlossen!$(NC)"

# Debug-Version kompilieren
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)
	@echo "$(PURPLE)🐛 Debug-Version kompiliert$(NC)"
	@echo "$(YELLOW)💡 Verwende 'gdb ./$(TARGET)' zum Debuggen$(NC)"

# Release-Version kompilieren
release: CXXFLAGS += $(RELEASE_FLAGS)
release: clean $(TARGET)
	@echo "$(GREEN)🚀 Release-Version kompiliert (optimiert)$(NC)"

# Profiling-Version kompilieren
profile: CXXFLAGS += $(PROFILE_FLAGS)
profile: clean $(TARGET)
	@echo "$(BLUE)📊 Profiling-Version kompiliert$(NC)"
	@echo "$(YELLOW)💡 Führe aus und nutze 'gprof ./$(TARGET)' zur Analyse$(NC)"

# Aufräumen
clean:
	@echo "$(YELLOW)🧹 Räume auf...$(NC)"
	rm -f $(TARGET)
	rm -f $(OBJECTS)
	rm -f $(DEPS)
	rm -f *.exe
	rm -f core
	rm -f gmon.out
	@echo "$(GREEN)✅ Aufgeräumt!$(NC)"

# Deep Clean (auch temporäre Editor-Dateien)
distclean: clean
	@echo "$(YELLOW)🗑️  Deep Clean...$(NC)"
	rm -f *~
	rm -f *.bak
	rm -f *.tmp
	rm -f .*.swp
	rm -f .*.swo
	@echo "$(GREEN)✅ Deep Clean abgeschlossen!$(NC)"

# Template-Validierung
validate:
	@echo "$(YELLOW)🔍 Validiere Template-Struktur...$(NC)"
	@if [ ! -f "Challenge.h" ]; then \
		echo "$(RED)❌ Challenge.h fehlt$(NC)"; \
		exit 1; \
	fi
	@if [ ! -f "Challenge.cpp" ]; then \
		echo "$(RED)❌ Challenge.cpp fehlt$(NC)"; \
		exit 1; \
	fi
	@if [ ! -f "Tests.h" ]; then \
		echo "$(RED)❌ Tests.h fehlt$(NC)"; \
		exit 1; \
	fi
	@if [ ! -f "Tests.cpp" ]; then \
		echo "$(RED)❌ Tests.cpp fehlt$(NC)"; \
		exit 1; \
	fi
	@if [ ! -f "test_runner.cpp" ]; then \
		echo "$(RED)❌ test_runner.cpp fehlt$(NC)"; \
		exit 1; \
	fi
	@if [ ! -f "challenge-info.json" ]; then \
		echo "$(RED)❌ challenge-info.json fehlt$(NC)"; \
		exit 1; \
	fi
	@echo "$(GREEN)✅ Template-Struktur vollständig$(NC)"

# Syntax-Check ohne Ausführung
check:
	@echo "$(YELLOW)🔍 Syntax-Check...$(NC)"
	$(CXX) $(CXXFLAGS) -fsyntax-only $(SOURCES)
	@echo "$(GREEN)✅ Syntax korrekt$(NC)"

# Header-Dependencies prüfen
deps:
	@echo "$(YELLOW)🔍 Analysiere Dependencies...$(NC)"
	$(CXX) $(CXXFLAGS) -MM $(SOURCES)

# Code-Formatting (falls clang-format verfügbar)
format:
	@if command -v clang-format >/dev/null 2>&1; then \
		echo "$(YELLOW)🎨 Formatiere Code...$(NC)"; \
		clang-format -i *.cpp *.h; \
		echo "$(GREEN)✅ Code formatiert$(NC)"; \
	else \
		echo "$(YELLOW)⚠️  clang-format nicht gefunden - übersprungen$(NC)"; \
	fi

# Code-Linting (falls cppcheck verfügbar)
lint:
	@if command -v cppcheck >/dev/null 2>&1; then \
		echo "$(YELLOW)🔍 Code-Analyse mit cppcheck...$(NC)"; \
		cppcheck --enable=all --std=c++17 --suppress=missingIncludeSystem *.cpp *.h; \
		echo "$(GREEN)✅ Code-Analyse abgeschlossen$(NC)"; \
	else \
		echo "$(YELLOW)⚠️  cppcheck nicht gefunden - übersprungen$(NC)"; \
	fi

# Memory-Check mit Valgrind (falls verfügbar)
memcheck: debug
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "$(PURPLE)🔍 Memory-Check mit Valgrind...$(NC)"; \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET); \
		echo "$(GREEN)✅ Memory-Check abgeschlossen$(NC)"; \
	else \
		echo "$(YELLOW)⚠️  Valgrind nicht gefunden - übersprungen$(NC)"; \
	fi

# Benchmark ausführen
benchmark: release
	@echo "$(BLUE)📊 Führe Performance-Benchmark aus...$(NC)"
	@for i in 1 2 3 4 5; do \
		echo "$(CYAN)Run $$i:$(NC)"; \
		time ./$(TARGET); \
		echo ""; \
	done
	@echo "$(GREEN)✅ Benchmark abgeschlossen$(NC)"

# Template-Info anzeigen
info:
	@echo "$(YELLOW)📋 C++ Template Challenge Info:$(NC)"
	@echo "   Template-Version: {{TEMPLATE_VERSION}}"
	@echo "   C++ Standard: C++17"
	@echo "   Datei-Struktur: Separate .h/.cpp Files"
	@echo "   Compiler: $(CXX)"
	@echo "   Flags: $(CXXFLAGS)"
	@echo "   Target: $(TARGET)"
	@echo ""
	@echo "$(YELLOW)📁 Projekt-Dateien:$(NC)"
	@echo "   Headers: $(HEADERS)"
	@echo "   Sources: $(SOURCES)"
	@echo "   Objects: $(OBJECTS)"
	@echo ""
	@echo "$(YELLOW)📂 Verfügbare Dateien:$(NC)"
	@ls -la *.h *.cpp *.json Makefile 2>/dev/null || echo "   Dateien werden erstellt..."
	@echo ""
	@echo "$(YELLOW)🎯 Verfügbare Make-Targets:$(NC)"
	@echo "   make        - Standard kompilierung"
	@echo "   make test   - Kompilieren + Tests ausführen"
	@echo "   make debug  - Debug-Version (-g -O0)"
	@echo "   make release- Release-Version (-O3 optimiert)"
	@echo "   make profile- Profiling-Version (-pg)"
	@echo "   make clean  - Aufräumen"
	@echo "   make distclean - Deep Clean"
	@echo "   make validate- Template validieren"
	@echo "   make check  - Syntax prüfen"
	@echo "   make deps   - Dependencies anzeigen"
	@echo "   make format - Code formatieren (clang-format)"
	@echo "   make lint   - Code-Analyse (cppcheck)"
	@echo "   make memcheck- Memory-Check (valgrind)"
	@echo "   make benchmark- Performance-Test"
	@echo "   make info   - Diese Info anzeigen"

# Installation von Dependencies (falls benötigt)
install-deps:
	@echo "$(YELLOW)📦 Prüfe C++ Dependencies...$(NC)"
	@if ! command -v g++ >/dev/null 2>&1; then \
		echo "$(RED)❌ g++ nicht gefunden!$(NC)"; \
		echo "$(YELLOW)💡 Installation:$(NC)"; \
		echo "   Ubuntu/Debian: sudo apt install build-essential"; \
		echo "   CentOS/RHEL:   sudo yum groupinstall 'Development Tools'"; \
		echo "   macOS:         xcode-select --install"; \
		echo "   Windows:       Install MSYS2 or MinGW"; \
		exit 1; \
	fi
	@echo "$(GREEN)✅ g++ verfügbar: $$(g++ --version | head -n1)$(NC)"
	@echo "$(YELLOW)📦 Optionale Tools prüfen...$(NC)"
	@command -v clang-format >/dev/null 2>&1 && echo "$(GREEN)✅ clang-format verfügbar$(NC)" || echo "$(YELLOW)⚠️  clang-format nicht verfügbar$(NC)"
	@command -v cppcheck >/dev/null 2>&1 && echo "$(GREEN)✅ cppcheck verfügbar$(NC)" || echo "$(YELLOW)⚠️  cppcheck nicht verfügbar$(NC)"
	@command -v valgrind >/dev/null 2>&1 && echo "$(GREEN)✅ valgrind verfügbar$(NC)" || echo "$(YELLOW)⚠️  valgrind nicht verfügbar$(NC)"
	@command -v gdb >/dev/null 2>&1 && echo "$(GREEN)✅ gdb verfügbar$(NC)" || echo "$(YELLOW)⚠️  gdb nicht verfügbar$(NC)"

# Help-Target
help: info

# Assembly-Output generieren (für Optimierungs-Analyse)
asm:
	@echo "$(CYAN)🔧 Generiere Assembly-Output...$(NC)"
	$(CXX) $(CXXFLAGS) -S -fverbose-asm -o Challenge.s Challenge.cpp
	@echo "$(GREEN)✅ Assembly in Challenge.s generiert$(NC)"

# Preprocessor-Output generieren
preprocess:
	@echo "$(CYAN)🔧 Generiere Preprocessor-Output...$(NC)"
	$(CXX) $(CXXFLAGS) -E -o Challenge.i Challenge.cpp
	@echo "$(GREEN)✅ Preprocessor-Output in Challenge.i generiert$(NC)"

# Object-File-Info anzeigen
objdump: $(OBJECTS)
	@echo "$(CYAN)🔍 Object-File Informationen:$(NC)"
	@for obj in $(OBJECTS); do \
		echo "$(YELLOW)--- $$obj ---$(NC)"; \
		objdump -t $$obj | head -20; \
		echo ""; \
	done

# Größe der generierten Dateien anzeigen
size: $(TARGET)
	@echo "$(CYAN)📏 Datei-Größen:$(NC)"
	@echo "$(YELLOW)Executable:$(NC)"
	@ls -lh $(TARGET)
	@echo "$(YELLOW)Object Files:$(NC)"
	@ls -lh $(OBJECTS)
	@echo "$(YELLOW)Größen-Details:$(NC)"
	@size $(TARGET)

# Phony targets (keine echten Dateien)
.PHONY: all test debug release profile clean distclean validate check deps format lint memcheck benchmark info help install-deps asm preprocess objdump size

# Default-Target falls nichts angegeben
.DEFAULT_GOAL := all

# Automatische Abhängigkeiten für bessere Builds
.PRECIOUS: %.o

# Parallelisierung aktivieren falls unterstützt
MAKEFLAGS += -j$(shell nproc 2>/dev/null || echo 1)

# Verbose Output bei V=1
ifeq ($(V),1)
	Q :=
else
	Q := @
endif
