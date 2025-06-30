# 🔷 Challenge 13: Variable Parameter-Liste

## 🎯 Ziel
Lerne das params-Schlüsselwort für variable Argumentanzahl.

## 📝 Aufgabe
Implementiere eine Methode `CalculateAverage(params double[] values)`:
- Akzeptiert beliebig viele double-Werte
- Berechnet den Durchschnitt aller Werte
- Behandelt den Fall mit 0 Argumenten
- Kann als Array oder einzelne Parameter aufgerufen werden

## 🧪 Beispiele
```csharp
CalculateAverage(1.0, 2.0, 3.0)           // → 2.0
CalculateAverage(10.0, 20.0)              // → 15.0
CalculateAverage(5.0)                     // → 5.0
CalculateAverage()                        // → 0.0
CalculateAverage(new double[] {1,2,3,4})  // → 2.5