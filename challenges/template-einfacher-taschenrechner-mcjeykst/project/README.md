# 🔷 Challenge 5: Einfacher Taschenrechner

## 🎯 Ziel
Lerne switch-Statements und Grundrechenarten.

## 📝 Aufgabe
Implementiere eine Methode `Calculate(double a, double b, char operation)`:
- Unterstützt: +, -, *, /
- Behandelt Division durch Null
- Wirft Exception bei unbekannten Operatoren

## 🧪 Beispiele
```csharp
Calculate(10, 5, '+')   // → 15.0
Calculate(10, 3, '-')   // → 7.0
Calculate(4, 6, '*')    // → 24.0
Calculate(15, 3, '/')   // → 5.0
Calculate(10, 0, '/')   // → Exception
Calculate(5, 2, '%')    // → Exception
