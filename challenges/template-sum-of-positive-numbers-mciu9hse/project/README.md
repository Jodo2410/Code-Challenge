# Sum of Positive Numbers - C++ Challenge

## 📋 Challenge Beschreibung

**Titel:** Sum of Positive Numbers  
**Schwierigkeit:** Leicht  
**Sprache:** C++17  
**Kategorie:** Arrays, Basic Math

### Problem
Schreibe eine Funktion, die die Summe aller positiven Zahlen in einem gegebenen Vektor berechnet. Negative Zahlen und Null sollen ignoriert werden.

### Beispiele
```cpp
Input:  [1, 2, 3, 4, 5]
Output: 15

Input:  [1, -4, 7, 12]
Output: 20

Input:  [-1, -2, -3]
Output: 0

Input:  []
Output: 0
```

### Anforderungen
- Summiere nur positive Zahlen (> 0)
- Ignoriere negative Zahlen und Null
- Leere Vektoren ergeben 0
- Verwende effiziente Algorithmen

## 🚀 Getting Started

### Projekt-Struktur
```
├── Challenge.h         # Header-Datei mit Klassen-Deklaration
├── Challenge.cpp       # Implementierung der solve() Funktion
├── Tests.h            # Test-Framework Header
├── Tests.cpp          # Test-Implementierung
├── test_runner.cpp    # Test-Ausführung und Main-Funktion
├── Challenge-info.json # Challenge-Metadaten
├── Makefile           # Build-Konfiguration
└── README.md          # Diese Datei
```

### Kompilieren und Ausführen

#### Mit Makefile (empfohlen)
```bash
# Kompilieren
make

# Tests ausführen
make test

# Aufräumen
make clean

# Debug-Build
make debug
```

#### Manuell kompilieren
```bash
# Kompilieren
g++ -std=c++17 -Wall -Wextra -O2 -o challenge test_runner.cpp Challenge.cpp Tests.cpp

# Ausführen
./challenge
```

## 🎯 Implementierung

### 1. Challenge.h verstehen
Die Header-Datei definiert die `Challenge` Klasse mit der `solve()` Methode und Hilfsfunktionen.

### 2. Challenge.cpp implementieren
Implementiere die `solve()` Funktion in `Challenge.cpp`:

```cpp
int Challenge::solve(const std::vector<int>& input) {
    
    // Durchlaufe den Vektor und summiere nur positive Zahlen
}
```

### 3. Tests verstehen
Die Tests prüfen verschiedene Szenarien:
- Grundlegende Funktionalität
- Edge Cases (leere Vektoren, nur negative Zahlen)
- Performance mit großen Eingaben
- Spezielle Fälle

## 📊 Template-Funktionen

### Test-Kategorien
- **Basic Functionality:** Standard-Eingaben
- **Edge Cases:** Grenzfälle und spezielle Eingaben
- **Performance:** Tests mit großen Datenmengen
- **Invalid Input:** Fehlerbehandlung
- **Large Input:** Stress-Tests
- **Special Cases:** Besondere Szenarien

### Performance-Ziele
- ✅ < 100ms: Excellent
- ✅ < 500ms: Good
- ✅ < 1000ms: Acceptable
- ⚠️ > 1000ms: Needs optimization

## 💡 Lösungsansatz

### Algorithmus
1. Durchlaufe alle Elemente im Vektor
2. Prüfe für jedes Element: `element > 0`
3. Wenn ja, addiere zur Summe
4. Gib die finale Summe zurück

### Komplexität
- **Zeit:** O(n) - Einmaliger Durchlauf
- **Speicher:** O(1) - Konstanter Speicher

### Pseudocode
```
sum = 0
for each number in input:
    if number > 0:
        sum += number
return sum
```

## 🧪 Testing

Das Template bietet umfangreiche Tests:

```bash
# Alle Tests ausführen
make test

# Einzelne Kategorien testen (in Tests.cpp anpassen)
# - testBasicFunctionality()
# - testEdgeCases()
# - testPerformance()
```

## 🏗️ Build-System

### Makefile Targets
- `make` oder `make all`: Standard-Build
- `make test`: Build und Test-Ausführung
- `make debug`: Debug-Build mit Symbolen
- `make clean`: Aufräumen
- `make help`: Zeige verfügbare Targets

### Compiler-Einstellungen
- **Standard:** C++17
- **Optimierung:** -O2 (Release), -O0 (Debug)
- **Warnings:** -Wall -Wextra
- **Debug:** -g -DDEBUG

## 📝 Challenge-Info

Die `Challenge-info.json` enthält Metadaten:
```json
{
  "title": "Sum of Positive Numbers",
  "description": "Berechne die Summe aller positiven Zahlen in einem Vektor",
  "difficulty": "easy",
  "language": "cpp",
  "tags": ["arrays", "math", "basic"],
  "author": "Template-Generator",
  "version": "2.0"
}
```

## 🎉 Erfolgskriterien

- ✅ Alle Tests bestehen
- ✅ Code kompiliert ohne Warnings
- ✅ Performance unter 1 Sekunde
- ✅ Template-Struktur vollständig
- ✅ Challenge-Info ausgefüllt

## 🔧 Troubleshooting

### Häufige Probleme

**Kompilierungsfehler:**
```bash
# Prüfe C++17 Support
g++ --version

# Alternative Kompilierung
g++ -std=c++17 *.cpp -o challenge
```

**Linker-Fehler:**
- Stelle sicher, dass alle .cpp Dateien kompiliert werden
- Prüfe, dass alle Funktionen implementiert sind

**Test-Fehler:**
- Überprüfe deine `solve()` Implementierung
- Debug mit `make debug` und `gdb`

### Support
- Template-Version: 2.0
- C++ Standard: C++17
- Getestet mit: GCC 7+, Clang 5+

---

**Viel Erfolg bei der Challenge! 🚀**