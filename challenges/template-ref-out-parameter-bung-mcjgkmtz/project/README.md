# 🔷 Challenge 12: Ref/Out Parameter-Übung

## 🎯 Ziel
Lerne ref und out Parameter für erweiterte Methodenrückgaben.

## 📝 Aufgabe
Implementiere zwei Methoden mit speziellen Parametern:

1. `SwapValues(ref int a, ref int b)` - Tauscht zwei Werte
2. `TryParseAdvanced(string input, out int number, out bool isValid)` - Erweiterte Parse-Funktion

## 🧪 Beispiele
```csharp
int x = 5, y = 10;
SwapValues(ref x, ref y);  // x=10, y=5

TryParseAdvanced("123", out int num, out bool valid);  // num=123, valid=true
TryParseAdvanced("abc", out int num2, out bool valid2); // num2=0, valid2=false