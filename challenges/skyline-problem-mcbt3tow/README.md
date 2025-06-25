# Skyline Problem

    ## 📝 Beschreibung
    Eine Stadt besteht aus mehreren Gebäuden unterschiedlicher Höhe. Die Skyline der Stadt ist die äußere Kontur, die von allen Gebäuden zusammen gebildet wird, wenn man sie von der Seite betrachtet.

Gegeben ist eine Liste von Gebäuden in der Form `[[Li, Ri, Hi]]`, wobei:
- `Li`: die x-Koordinate der linken Seite des i-ten Gebäudes
- `Ri`: die x-Koordinate der rechten Seite des i-ten Gebäudes
- `Hi`: die Höhe des i-ten Gebäudes

Implementiere eine Funktion, die die Skyline in Form einer Liste von "Schlüsselpunkten" zurückgibt, sortiert nach x-Koordinate. Ein Schlüsselpunkt ist definiert als eine x-Koordinate, an der sich die Höhe der Skyline ändert. Das Format der Ausgabe ist eine Liste von Paaren `[x, y]`, wobei `x` die x-Koordinate ist und `y` die Höhe der Skyline an dieser Position.

Beispiel:
Eingabe: `[[2, 9, 10], [3, 7, 15], [5, 12, 12], [15, 20, 10], [19, 24, 8]]`
Ausgabe: `[[2, 10], [3, 15], [7, 12], [12, 0], [15, 10], [20, 8], [24, 0]]`

Erklärung:
- Bei x=2 beginnt das erste Gebäude mit Höhe 10
- Bei x=3 beginnt das zweite Gebäude mit Höhe 15 (überschreibt das erste)
- Bei x=7 endet das zweite Gebäude, das dritte Gebäude ist sichtbar mit Höhe 12
- Bei x=12 endet das dritte Gebäude, die Höhe fällt auf 0
- Bei x=15 beginnt das vierte Gebäude mit Höhe 10
- Bei x=20 endet das vierte Gebäude, das fünfte Gebäude ist sichtbar mit Höhe 8
- Bei x=24 endet das fünfte Gebäude, die Höhe fällt auf 0

![Skyline Visualisierung](https://assets.leetcode.com/uploads/2020/12/01/merged.jpg)

    ## 🎯 Schwierigkeit
    ⭐⭐⭐ hard

    ## 💻 Sprache
    csharp

    ## 🏷️ Tags
    algorithms, divide-and-conquer, heap, priority-queue, line-sweep, csharp, hard, geometry

    ## 🚀 Anweisungen
    1. Öffne `starter.cs`
    2. Implementiere deine Lösung in der `SkylineProblem` Klasse
    3. Führe Tests aus: `dotnet run`
    4. Alle Tests müssen bestehen! ✅

    ## 📖 Referenz
    - `solution.cs` - Referenz-Lösung zum Vergleichen

    ## 🔧 Template-Info
    - **Test-Klasse:** `Tests`
    - **Main-Klasse:** `SkylineProblem`
    - **Funktion:** `SkylineProblem`

    ## 📊 Details
    - **Erstellt von:** Jodo2410
    - **Erstellt am:** 25.6.2025
    - **Status:** approved

    
    