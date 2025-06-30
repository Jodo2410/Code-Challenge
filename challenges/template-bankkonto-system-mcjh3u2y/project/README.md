# 🔷 Challenge 17: Bankkonto-System

## 🎯 Ziel
Lerne Objektorientierung, Kapselung und Geschäftslogik-Implementierung.

## 📝 Aufgabe
Implementiere eine `BankAccount` Klasse mit folgenden Features:
- Private Felder für Kontonummer und Saldo
- Properties für sicheren Zugriff
- Methoden: `Deposit()`, `Withdraw()`, `GetBalance()`
- Validierung (keine negativen Beträge, ausreichend Saldo)
- Transaktionshistorie (optional)

## 🧪 Beispiele
```csharp
var account = new BankAccount("12345", 1000.0m);
account.Deposit(500.0m);        // Saldo: 1500
account.Withdraw(200.0m);       // Saldo: 1300
account.GetBalance();           // → 1300.0m
account.Withdraw(2000.0m);      // Exception: Unzureichender Saldo