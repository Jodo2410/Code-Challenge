# Regulärer Ausdrucksabgleich

    ## 📝 Beschreibung
    Implementiere einen regulären Ausdrucksabgleich mit Unterstützung für '.' und '*'.

- '.' entspricht einem beliebigen einzelnen Zeichen.
- '*' entspricht null oder mehr des vorherigen Elements.

Der Abgleich sollte den gesamten String s abdecken, nicht nur einen Teil davon.

Beispiele:

```
s = "aa", p = "a" → false (Ein einzelnes 'a' passt nicht zu "aa")
s = "aa", p = "a*" → true ('*' bedeutet, dass 'a' beliebig oft vorkommen kann, also auch zweimal)
s = "ab", p = ".*" → true ('.' bedeutet beliebiges Zeichen und '*' bedeutet beliebig oft, also passt ".*" zu beliebigen Strings)
s = "aab", p = "c*a*b" → true ('c*' entspricht 0 Vorkommen von 'c', 'a*' entspricht 2 Vorkommen von 'a', und 'b' entspricht 'b')
s = "mississippi", p = "mis*is*p*." → false (passt nicht zum gesamten String)
```

Die Lösung sollte effizient sein und sowohl Zeit- als auch Speicherkomplexität berücksichtigen.

    ## 🎯 Schwierigkeit
    ⭐⭐⭐ hard

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    dynamic-programming, string-matching, regex, algorithms, csharp, hard

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `RegulrerAusdrucksabgleich` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `RegulrerAusdrucksabgleich`
    - **Funktion:** `RegulrerAusdrucksabgleich`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 25.6.2025
    - **Status:** approved

    
    