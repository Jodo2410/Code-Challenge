# Geheimer Nachrichten-Decoder

    ## 📝 Beschreibung
    ### Einführung:
Ein Geheimbund hat eine Nachricht verschlüsselt. Die Verschlüsselung basiert auf einer einfachen **Caesar-Verschiebung**, bei der jeder Buchstabe im Alphabet um eine bestimmte Anzahl an Positionen nach links oder rechts verschoben wurde.

Beispiel:
- Eingabe: "D", Verschiebung: -3 → Ausgabe: "A"
- Eingabe: "E", Verschiebung: -1 → Ausgabe: "D"

**Nicht-Buchstaben** (z. B. Leerzeichen, Satzzeichen, Zahlen) sollen **unverändert** bleiben.

### Aufgabe:
Schreibe eine Methode `DecodeMessage(string input, int shift)`, die eine verschlüsselte Nachricht dekodiert, indem sie jeden Buchstaben mit dem übergebenen Verschiebungswert zurückverschiebt. 

Die Groß-/Kleinschreibung soll erhalten bleiben.

### Beispiele:
```csharp
DecodeMessage("Khoor Zruog", -3) => "Hello World"
DecodeMessage("Fdhvdu Flskhu", -3) => "Caesar Cipher"
DecodeMessage("Ebiil Tloia", 23) => "Hello World"
```

    ## 🎯 Schwierigkeit
    ⭐ easy

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    csharp, strings, caesar, decoder, easy, verschlüsselung, algorithmen, einsteiger

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `GeheimerNachrichtendecoder` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `GeheimerNachrichtendecoder`
    - **Funktion:** `GeheimerNachrichtendecoder`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 27.6.2025
    - **Status:** approved

    
    