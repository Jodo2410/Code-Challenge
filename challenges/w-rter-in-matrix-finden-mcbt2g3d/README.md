# Wörter in Matrix finden

    ## 📝 Beschreibung
    Implementiere eine Funktion, die prüft, ob ein gegebenes Wort in einer 2D-Matrix (Grid) von Buchstaben gefunden werden kann. Das Wort kann gebildet werden, indem benachbarte Zellen in der Matrix verknüpft werden. Benachbarte Zellen sind horizontal oder vertikal miteinander verbunden (nicht diagonal). Die gleiche Zelle darf nicht mehrfach verwendet werden.

Beispiel:

```
Matrix:
['A', 'B', 'C', 'E']
['S', 'F', 'C', 'S']
['A', 'D', 'E', 'E']

Wort: "ABCCED" -> true
Wort: "SEE" -> true
Wort: "ABCB" -> false
```

Erklärung:
- "ABCCED" kann gefunden werden, beginnend mit 'A' in der oberen linken Ecke, dann nach rechts zu 'B' und 'C', dann nach unten zu 'C', dann diagonal nach unten links zu 'E' und schließlich zu 'D'.
- "SEE" kann gefunden werden, beginnend mit 'S' in der oberen rechten Ecke, dann nach unten zu 'E' und nach rechts zu 'E'.
- "ABCB" kann nicht gefunden werden, weil nach dem Pfad 'A'->'B'->'C' würde es wieder zurück zu 'B' gehen müssen, aber die Zelle mit 'B' wurde bereits verwendet.

    ## 🎯 Schwierigkeit
    ⭐⭐ medium

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    graph, dfs, backtracking, matrix, recursion, algorithms, csharp, medium

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `WrterInMatrixFinden` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `WrterInMatrixFinden`
    - **Funktion:** `WrterInMatrixFinden`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 25.6.2025
    - **Status:** approved

    
    