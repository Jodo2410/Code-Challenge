# HTML-Tag-Remover

    ## 📝 Beschreibung
    ### Einführung:
HTML-Tags werden von Webbrowsern interpretiert, um Inhalte darzustellen. Zum Beispiel `<p>Hello</p>` zeigt einfach "Hello" an. Für manche Anwendungen (z. B. Textanalyse) müssen HTML-Tags entfernt werden.

### Aufgabe:
Schreibe eine Methode `RemoveHtmlTags(string html)`, die alle HTML-Tags aus dem Text entfernt und nur den reinen Inhalt zurückgibt.

### Beispiele:
```csharp
RemoveHtmlTags("<p>Hello World</p>") => "Hello World"
RemoveHtmlTags("<div><b>Text</b> mit <i>Tags</i></div>") => "Text mit Tags"
RemoveHtmlTags("<a href='link'>Klick mich</a>") => "Klick mich"
```

    ## 🎯 Schwierigkeit
    ⭐ easy

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    csharp, strings, html, regex, textverarbeitung, einsteiger

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `Htmltagremover` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `Htmltagremover`
    - **Funktion:** `Htmltagremover`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 27.6.2025
    - **Status:** approved

    
    