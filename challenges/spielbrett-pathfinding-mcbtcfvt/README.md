# Spielbrett-Pathfinding

    ## 📝 Beschreibung
    Du entwickelst ein Brettspiel, in dem ein Spieler von einem Startpunkt zu einem Zielpunkt navigieren muss. Das Spielbrett ist ein Gitter aus Feldern, wobei einige Felder Hindernisse enthalten, die nicht betreten werden können.

Implementiere eine Funktion, die den kürzesten Weg vom Start zum Ziel findet, oder -1 zurückgibt, wenn kein Weg existiert.

Das Spielbrett wird als 2D-Array dargestellt, wobei:
- 0 für ein freies Feld steht
- 1 für ein Hindernis steht
- 2 für den Startpunkt steht
- 3 für den Zielpunkt steht

Der Spieler kann sich horizontal und vertikal (nicht diagonal) bewegen.

Beispiel:
```
Spielfeld:
[
  [0, 0, 0, 0],
  [0, 1, 1, 0],
  [2, 0, 0, 0],
  [1, 1, 0, 3]
]

Ausgabe: 5
```

Erklärung: Der kürzeste Weg vom Start (2,0) zum Ziel (3,3) hat 5 Schritte:
(2,0) -> (2,1) -> (2,2) -> (2,3) -> (3,3)

Beachte, dass die Koordinaten im Format (Zeile, Spalte) angegeben sind.

    ## 🎯 Schwierigkeit
    ⭐⭐ medium

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    games, pathfinding, bfs, 2d-array, algorithms, grid, csharp, medium

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `Spielbrettpathfinding` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `Spielbrettpathfinding`
    - **Funktion:** `Spielbrettpathfinding`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 25.6.2025
    - **Status:** approved

    
    