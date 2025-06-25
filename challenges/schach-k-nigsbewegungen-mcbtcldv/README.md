# Schach - Königsbewegungen

    ## 📝 Beschreibung
    In einem Schachspiel möchtest du wissen, ob ein König von seiner aktuellen Position zu einer Zielposition gelangen kann, ohne von gegnerischen Figuren geschlagen zu werden.

Implementiere eine Funktion, die überprüft, ob der König sicher von Start zu Ziel navigieren kann. Der König kann sich in jede angrenzende Position bewegen (horizontal, vertikal oder diagonal), aber er darf sich nicht auf Felder bewegen, die von gegnerischen Figuren bedroht sind.

Das Schachbrett wird als 8x8-Grid dargestellt:
- 'K' ist die Position des Königs (Start)
- 'T' ist die Zielposition
- 'X' sind Felder, die von gegnerischen Figuren bedroht werden
- '.' sind freie, sichere Felder

Beispiel:
```
Schachbrett:
[
  ['.', '.', '.', '.', '.', '.', '.', '.'],
  ['.', '.', 'X', 'X', '.', '.', '.', '.'],
  ['.', '.', '.', '.', '.', 'X', '.', '.'],
  ['.', 'X', 'K', '.', '.', '.', '.', '.'],
  ['.', '.', '.', '.', '.', 'X', '.', '.'],
  ['.', '.', '.', '.', '.', '.', '.', '.'],
  ['.', '.', '.', 'T', '.', '.', '.', '.'],
  ['.', '.', '.', '.', '.', '.', '.', '.']
]

Ausgabe: true
```

Erklärung: Der König kann sicher von (3,2) nach (6,3) gelangen, indem er einen Pfad wählt, der alle bedrohten Felder vermeidet.

    ## 🎯 Schwierigkeit
    ⭐ easy

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    games, chess, pathfinding, bfs, grid, 2d-array, csharp, easy

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `SchachKnigsbewegungen` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `SchachKnigsbewegungen`
    - **Funktion:** `SchachKnigsbewegungen`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 25.6.2025
    - **Status:** approved

    
    