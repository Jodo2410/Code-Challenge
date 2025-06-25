# Minimale Editierdistanz

    ## 📝 Beschreibung
    Gegeben sind zwei Strings word1 und word2. Berechne die minimale Anzahl von Operationen, die benötigt werden, um word1 in word2 umzuwandeln.

Du darfst die folgenden drei Operationen auf einem String ausführen:
- Einfügen eines Zeichens
- Löschen eines Zeichens
- Ersetzen eines Zeichens

Beispiele:

```
word1 = "horse", word2 = "ros"
Ausgabe: 3
Erklärung: 
horse -> rorse (ersetze 'h' durch 'r')
rorse -> rose (lösche 'r')
rose -> ros (lösche 'e')

word1 = "intention", word2 = "execution"
Ausgabe: 5
Erklärung: 
intention -> inention (lösche 't')
inention -> enention (ersetze 'i' durch 'e')
enention -> exention (ersetze 'n' durch 'x')
exention -> exection (ersetze 'n' durch 'c')
exection -> execution (füge 'u' ein)
```

Dieses Problem ist auch bekannt als Levenshtein-Distanz und hat wichtige Anwendungen in der Textverarbeitung, Spracherkennung und der DNA-Sequenzanalyse.

    ## 🎯 Schwierigkeit
    ⭐⭐⭐ hard

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    dynamic-programming, strings, algorithms, levenshtein-distance, csharp, hard

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `MinimaleEditierdistanz` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `MinimaleEditierdistanz`
    - **Funktion:** `MinimaleEditierdistanz`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 25.6.2025
    - **Status:** approved

    
    