# C++ Template Challenge Makefile (mit build/ Ordner)
# Template-Version: 2.0
# Kompatibel mit: C++17

# Compiler und Flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I.
DEBUG_FLAGS = -g -DDEBUG -O0
RELEASE_FLAGS = -O3 -DNDEBUG -march=native
PROFILE_FLAGS = -pg -O2

# Verzeichnisse
BUILD_DIR = build
SRC_DIR = .
HEADERS_DIR = .

# Dateien
TARGET = test_runner
SOURCES = test_runner.cpp Challenge.cpp Tests.cpp
HEADERS = Challenge.h Tests.h

# Build-Artefakte in build/ Ordner
BUILD_OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
BUILD_DEPS = $(BUILD_OBJECTS:%.o=%.d)
BUILD_TARGET = $(BUILD_DIR)/$(TARGET)

# Farben für Output
GREEN = \033[0;32m
YELLOW = \033[1;33m
RED = \033[0;31m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
NC = \033[0m # No Color

# Standard-Target
all: $(BUILD_TARGET)
	@echo "$(GREEN)✅ C++ Template-Challenge erfolgreich kompiliert!$(NC)"
	@echo "$(YELLOW)📁 Build-Artefakte in $(BUILD_DIR)/$(NC)"
	@echo "$(YELLOW)📁 Verwende 'make test' zum Ausführen der Tests$(NC)"

# Build-Verzeichnis erstellen
$(BUILD_DIR):
	@echo "$(CYAN)📁 Erstelle Build-Verzeichnis: $(BUILD_DIR)$(NC)"
	@mkdir -p $(BUILD_DIR)

# Test Runner kompilieren (im build/ Ordner)
$(BUILD_TARGET): $(BUILD_OBJECTS) | $(BUILD_DIR)
	@echo "$(YELLOW)🔗 Linke Executable im Build-Ordner...$(NC)"
	$(CXX) $(CXXFLAGS) -o $(BUILD_TARGET) $(BUILD_OBJECTS)
	@echo "$(GREEN)✅ Linking erfolgreich: $(BUILD_TARGET)$(NC)"

# Object-Dateien kompilieren (in build/ Ordner)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	@echo "$(CYAN)🔧 Kompiliere $< → $@$(NC)"
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Dependencies einbinden
-include $(BUILD_DEPS)

# Tests ausführen
test: $(BUILD_TARGET)
	@echo "$(YELLOW)🧪 Führe C++ Template Tests aus...$(NC)"
	@echo "$(YELLOW)================================================$(NC)"
	$(BUILD_TARGET)
	@echo "$(YELLOW)================================================$(NC)"
	@echo "$(GREEN)🎉 Tests abgeschlossen!$(NC)"

# Tests ausführen und Executable nach Root kopieren (für Kompatibilität)
test-local: $(BUILD_TARGET)
	@echo "$(CYAN)📋 Kopiere Executable für lokale Ausführung...$(NC)"
	@cp $(BUILD_TARGET) $(TARGET)
	@echo "$(YELLOW)🧪 Führe C++ Template Tests aus...$(NC)"
	@echo "$(YELLOW)================================================$(NC)"
	./$(TARGET)
	@echo "$(YELLOW)================================================$(NC)"
	@echo "$(GREEN)🎉 Tests abgeschlossen!$(NC)"
	@echo "$(YELLOW)💡 Executable auch verfügbar als: $(TARGET)$(NC)"

# Debug-Version kompilieren
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean $(BUILD_TARGET)
	@echo "$(PURPLE)🐛 Debug-Version kompiliert in $(BUILD_DIR)/$(NC)"
	@echo "$(YELLOW)💡 Verwende 'make debug-run' oder 'gdb $(BUILD_TARGET)'$(NC)"

# Debug-Version ausführen
debug-run: debug
	@echo "$(PURPLE)🐛 Führe Debug-Version aus...$(NC)"
	$(BUILD_TARGET)

# Release-Version kompilieren
release: CXXFLAGS += $(RELEASE_FLAGS)
release: clean $(BUILD_TARGET)
	@echo "$(GREEN)🚀 Release-Version kompiliert (optimiert) in $(BUILD_DIR)/$(NC)"

# Release-Version ausführen
release-run: release
	@echo "$(GREEN)🚀 Führe Release-Version aus...$(NC)"
	$(BUILD_TARGET)

# Profiling-Version kompilieren
profile: CXXFLAGS += $(PROFILE_FLAGS)
profile: clean $(BUILD_TARGET)
	@echo "$(BLUE)📊 Profiling-Version kompiliert in $(BUILD_DIR)/$(NC)"
	@echo "$(YELLOW)💡 Führe 'make profile-run' aus und nutze 'gprof $(BUILD_TARGET)' zur Analyse$(NC)"

# Profiling-Version ausführen
profile-run: profile
	@echo "$(BLUE)📊 Führe Profiling-Version aus...$(NC)"
	$(BUILD_TARGET)
	@if [ -f gmon.out ]; then \
		echo "$(BLUE)📊 Profiling-Daten in gmon.out generiert$(NC)"; \
		echo "$(YELLOW)💡 Analyse mit: gprof $(BUILD_TARGET) gmon.out$(NC)"; \
	fi

# Aufräumen - nur Build-Artefakte
clean:
	@echo "$(YELLOW)🧹 Räume Build-Artefakte auf...$(NC)"
	@if [ -d "$(BUILD_DIR)" ]; then \
		rm -rf $(BUILD_DIR); \
		echo "$(GREEN)✅ $(BUILD_DIR)/ Ordner entfernt$(NC)"; \
	fi
	@rm -f $(TARGET)
	@rm -f *.exe
	@rm -f core
	@rm -f gmon.out
	@echo "$(GREEN)✅ Aufgeräumt!$(NC)"

# Deep Clean (auch temporäre Editor-Dateien)
distclean: clean
	@echo "$(YELLOW)🗑️  Deep Clean...$(NC)"
	@rm -f *~
	@rm -f *.bak
	@rm -f *.tmp
	@rm -f .*.swp
	@rm -f .*.swo
	@rm -f *.s
	@rm -f *.i
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
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $(BUILD_TARGET); \
		echo "$(GREEN)✅ Memory-Check abgeschlossen$(NC)"; \
	else \
		echo "$(YELLOW)⚠️  Valgrind nicht gefunden - übersprungen$(NC)"; \
	fi

# Benchmark ausführen
benchmark: release
	@echo "$(BLUE)📊 Führe Performance-Benchmark aus...$(NC)"
	@for i in 1 2 3 4 5; do \
		echo "$(CYAN)Run $$i:$(NC)"; \
		time $(BUILD_TARGET); \
		echo ""; \
	done
	@echo "$(GREEN)✅ Benchmark abgeschlossen$(NC)"

# Template-Info anzeigen
info:
	@echo "$(YELLOW)📋 C++ Template Challenge Info:$(NC)"
	@echo "   Template-Version: 2.0"
	@echo "   C++ Standard: C++17"
	@echo "   Datei-Struktur: Separate .h/.cpp Files"
	@echo "   Build-Ordner: $(BUILD_DIR)/"
	@echo "   Compiler: $(CXX)"
	@echo "   Flags: $(CXXFLAGS)"
	@echo "   Target: $(BUILD_TARGET)"
	@echo ""
	@echo "$(YELLOW)📁 Projekt-Struktur:$(NC)"
	@echo "   Source Files: $(SOURCES)"
	@echo "   Header Files: $(HEADERS)"
	@echo "   Build Objects: $(BUILD_OBJECTS)"
	@echo "   Build Target: $(BUILD_TARGET)"
	@echo ""
	@echo "$(YELLOW)📂 Verfügbare Dateien:$(NC)"
	@ls -la *.h *.cpp *.json Makefile 2>/dev/null || echo "   Dateien werden erstellt..."
	@if [ -d "$(BUILD_DIR)" ]; then \
		echo "$(YELLOW)📂 Build-Artefakte:$(NC)"; \
		ls -la $(BUILD_DIR)/ 2>/dev/null || echo "   Build-Ordner leer"; \
	else \
		echo "$(YELLOW)📂 Build-Ordner: Noch nicht erstellt$(NC)"; \
	fi
	@echo ""
	@echo "$(YELLOW)🎯 Verfügbare Make-Targets:$(NC)"
	@echo "   make           - Standard kompilierung in $(BUILD_DIR)/"
	@echo "   make test      - Kompilieren + Tests ausführen"
	@echo "   make test-local- Tests mit lokalem Executable"
	@echo "   make debug     - Debug-Version (-g -O0)"
	@echo "   make debug-run - Debug kompilieren + ausführen"
	@echo "   make release   - Release-Version (-O3 optimiert)"
	@echo "   make release-run- Release kompilieren + ausführen"
	@echo "   make profile   - Profiling-Version (-pg)"
	@echo "   make profile-run- Profiling kompilieren + ausführen"
	@echo "   make clean     - Build-Ordner aufräumen"
	@echo "   make distclean - Deep Clean"
	@echo "   make validate  - Template validieren"
	@echo "   make check     - Syntax prüfen"
	@echo "   make deps      - Dependencies anzeigen"
	@echo "   make format    - Code formatieren (clang-format)"
	@echo "   make lint      - Code-Analyse (cppcheck)"
	@echo "   make memcheck  - Memory-Check (valgrind)"
	@echo "   make benchmark - Performance-Test"
	@echo "   make info      - Diese Info anzeigen"

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

# Assembly-Output generieren (im build/ Ordner)
asm: | $(BUILD_DIR)
	@echo "$(CYAN)🔧 Generiere Assembly-Output...$(NC)"
	$(CXX) $(CXXFLAGS) -S -fverbose-asm -o $(BUILD_DIR)/Challenge.s Challenge.cpp
	@echo "$(GREEN)✅ Assembly in $(BUILD_DIR)/Challenge.s generiert$(NC)"

# Preprocessor-Output generieren (im build/ Ordner)
preprocess: | $(BUILD_DIR)
	@echo "$(CYAN)🔧 Generiere Preprocessor-Output...$(NC)"
	$(CXX) $(CXXFLAGS) -E -o $(BUILD_DIR)/Challenge.i Challenge.cpp
	@echo "$(GREEN)✅ Preprocessor-Output in $(BUILD_DIR)/Challenge.i generiert$(NC)"

# Object-File-Info anzeigen
objdump: $(BUILD_TARGET)
	@echo "$(CYAN)🔍 Object-File Informationen:$(NC)"
	@for obj in $(BUILD_OBJECTS); do \
		if [ -f "$$obj" ]; then \
			echo "$(YELLOW)--- $$obj ---$(NC)"; \
			objdump -t $$obj | head -20; \
			echo ""; \
		fi; \
	done

# Größe der generierten Dateien anzeigen
size: $(BUILD_TARGET)
	@echo "$(CYAN)📏 Datei-Größen:$(NC)"
	@echo "$(YELLOW)Executable:$(NC)"
	@ls -lh $(BUILD_TARGET)
	@echo "$(YELLOW)Object Files:$(NC)"
	@ls -lh $(BUILD_OBJECTS) 2>/dev/null || echo "   Keine Object-Files gefunden"
	@echo "$(YELLOW)Größen-Details:$(NC)"
	@size $(BUILD_TARGET)

# Build-Struktur anzeigen
tree:
	@echo "$(CYAN)🌳 Projekt-Struktur:$(NC)"
	@echo "."
	@echo "├── Source Files"
	@for src in $(SOURCES); do echo "│   ├── $$src"; done
	@echo "├── Header Files"
	@for hdr in $(HEADERS); do echo "│   ├── $$hdr"; done
	@echo "├── Config Files"
	@echo "│   ├── Makefile"
	@echo "│   └── challenge-info.json"
	@if [ -d "$(BUILD_DIR)" ]; then \
		echo "└── $(BUILD_DIR)/ (Build Artifacts)"; \
		for obj in $(BUILD_OBJECTS); do \
			if [ -f "$$obj" ]; then echo "    ├── $$(basename $$obj)"; fi; \
		done; \
		if [ -f "$(BUILD_TARGET)" ]; then echo "    └── $(TARGET) (executable)"; fi; \
	else \
		echo "└── $(BUILD_DIR)/ (not created yet)"; \
	fi

# Workspace aufräumen und neu bauen
rebuild: distclean all
	@echo "$(GREEN)🔄 Rebuild abgeschlossen!$(NC)"

# Schneller rebuild ohne Deep Clean
fast-rebuild: clean all
	@echo "$(GREEN)⚡ Fast Rebuild abgeschlossen!$(NC)"

# Watch-Mode (erfordert inotify-tools)
watch:
	@if command -v inotifywait >/dev/null 2>&1; then \
		echo "$(CYAN)👁️  Watch-Mode aktiv - Änderungen an .cpp/.h Dateien werden überwacht$(NC)"; \
		echo "$(YELLOW)💡 Beenden mit Ctrl+C$(NC)"; \
		while true; do \
			inotifywait -e modify *.cpp *.h 2>/dev/null; \
			echo "$(YELLOW)🔄 Datei geändert - Rebuilding...$(NC)"; \
			make --no-print-directory fast-rebuild; \
			echo "$(GREEN)✅ Rebuild abgeschlossen$(NC)"; \
		done; \
	else \
		echo "$(RED)❌ inotifywait nicht gefunden$(NC)"; \
		echo "$(YELLOW)💡 Installation: sudo apt install inotify-tools$(NC)"; \
	fi

# Phony targets (keine echten Dateien)
.PHONY: all test test-local debug debug-run release release-run profile profile-run clean distclean validate check deps format lint memcheck benchmark info help install-deps asm preprocess objdump size tree rebuild fast-rebuild watch

# Default-Target falls nichts angegeben
.DEFAULT_GOAL := all

# Automatische Abhängigkeiten für bessere Builds
.PRECIOUS: %.o $(BUILD_DIR)/%.o

# Parallelisierung aktivieren falls unterstützt
MAKEFLAGS += -j$(shell nproc 2>/dev/null || echo 1)

# Verbose Output bei V=1
ifeq ($(V),1)
	Q :=
else
	Q := @
endif

# Build-Ordner als Order-Only Prerequisite für bessere Performance
$(BUILD_OBJECTS): | $(BUILD_DIR)
