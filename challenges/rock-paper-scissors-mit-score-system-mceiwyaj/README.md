# Rock-Paper-Scissors mit Score-System

    ## 📝 Beschreibung
    ### Einführung:
Erstelle ein kleines System für das klassische Spiel **Schere-Stein-Papier** (engl. Rock-Paper-Scissors), bei dem zwei Spieler gegeneinander antreten. Ziel ist es, ein Objektorientiertes System mit Punktezählung zu entwerfen.

### Regeln:
- **Rock** schlägt **Scissors**
- **Scissors** schlägt **Paper**
- **Paper** schlägt **Rock**

### Aufgabe:
1. Erstelle ein Enum `Move` mit den Werten `Rock`, `Paper`, `Scissors`
2. Implementiere eine Methode `DetermineWinner(Move player1, Move player2)`, die `1`, `2` oder `0` zurückgibt (Spieler 1 gewinnt, Spieler 2 gewinnt, Unentschieden).
3. Erstelle eine Klasse `Scoreboard`, die die Punkte beider Spieler verwaltet.
4. Schreibe Methoden, um Runden zu spielen und die Gesamtpunktzahl zu aktualisieren.

### Beispiele:
```csharp
DetermineWinner(Move.Rock, Move.Scissors) => 1
DetermineWinner(Move.Scissors, Move.Rock) => 2
DetermineWinner(Move.Paper, Move.Paper) => 0
```

    ## 🎯 Schwierigkeit
    ⭐⭐⭐ hard

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    csharp, oop, enum, spiel, logik, hard, score, methoden, testbar

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `RockpaperscissorsMitScoresystem` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `RockpaperscissorsMitScoresystem`
    - **Funktion:** `RockpaperscissorsMitScoresystem`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 27.6.2025
    - **Status:** approved

    
    