# LRU Cache Implementierung

    ## 📝 Beschreibung
    Implementiere eine Least Recently Used (LRU) Cache-Datenstruktur. Der Cache sollte eine festgelegte Kapazität haben und folgende Operationen unterstützen:

1. `Get(key)`: Gibt den Wert des Schlüssels zurück, wenn er im Cache vorhanden ist, sonst -1.
2. `Put(key, value)`: Fügt einen Schlüssel-Wert-Paar in den Cache ein. Wenn der Schlüssel bereits existiert, aktualisiere seinen Wert. Wenn die Anzahl der Schlüssel die Kapazität überschreitet, entferne den am längsten nicht verwendeten Schlüssel.

Beide Operationen sollten in einer durchschnittlichen Zeitkomplexität von O(1) ausgeführt werden.

Beispiel:
```
LRUCache cache = new LRUCache(2); // Kapazität = 2

cache.Put(1, 1);
cache.Put(2, 2);
cache.Get(1);       // Gibt 1 zurück
cache.Put(3, 3);    // Entfernt Schlüssel 2, da 1 zuletzt verwendet wurde
cache.Get(2);       // Gibt -1 zurück (nicht gefunden)
cache.Put(4, 4);    // Entfernt Schlüssel 1
cache.Get(1);       // Gibt -1 zurück (nicht gefunden)
cache.Get(3);       // Gibt 3 zurück
cache.Get(4);       // Gibt 4 zurück
```

    ## 🎯 Schwierigkeit
    ⭐⭐ medium

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    data-structures, dictionary, linked-list, algorithms, caching, csharp, medium

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `LruCacheImplementierung` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `LruCacheImplementierung`
    - **Funktion:** `LruCacheImplementierung`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 25.6.2025
    - **Status:** approved

    
    