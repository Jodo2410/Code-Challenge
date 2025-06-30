# 🔷 Challenge 7: Maximum Finder

## 🎯 Ziel
Lerne das Finden von Extremwerten in Arrays.

## 📝 Aufgabe
Implementiere eine Methode `FindMaximum(int[] numbers)`:
- Findet das größte Element im Array
- Behandelt leere und null Arrays
- Wirft Exception bei ungültigen Eingaben

## 🧪 Beispiele
```csharp
FindMaximum(new int[] { 1, 5, 3, 9, 2 })  // → 9
FindMaximum(new int[] { -5, -1, -10 })     // → -1
FindMaximum(new int[] { 42 })              // → 42
FindMaximum(new int[] { })                 // → Exception
FindMaximum(null)                          // → Exception