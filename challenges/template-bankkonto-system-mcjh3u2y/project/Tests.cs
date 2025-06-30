using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 17: Bankkonto-System Tests");
            Console.WriteLine(new string('=', 50));

            TestAccountCreation();
            TestInitialBalance();
            TestDeposit();
            TestWithdrawSuccess();
            TestWithdrawInsufficientFunds();
            TestDepositNegativeAmount();
            TestWithdrawNegativeAmount();
            TestMultipleTransactions();
            TestAccountMethods();
            TestToString();
            TestSolveMethod();
            TestEmptyAccountNumber();
            TestLargeAmounts();
            TestDecimalPrecision();

            Console.WriteLine($"\n{testsPassed}/{totalTests} Tests bestanden");
            
            if (testsPassed == totalTests)
            {
                Console.WriteLine("🎉 Alle Tests bestanden!");
            }
            else
            {
                Console.WriteLine("❌ Tests fehlgeschlagen.");
                Environment.Exit(1);
            }
        }

        private static void TestAccountCreation()
        {
            Test("Konto-Erstellung ohne Anfangssaldo", () => {
                var account = new BankAccount("12345");
                AssertEqual("12345", account.GetAccountNumber(), "Kontonummer sollte korrekt gesetzt sein");
                AssertEqual(0m, account.GetBalance(), "Anfangssaldo sollte 0 sein");
            });
        }

        private static void TestInitialBalance()
        {
            Test("Konto mit Anfangssaldo", () => {
                var account = new BankAccount("67890", 1000m);
                AssertEqual("67890", account.GetAccountNumber(), "Kontonummer sollte korrekt sein");
                AssertEqual(1000m, account.GetBalance(), "Anfangssaldo sollte 1000 sein");
            });
        }

        private static void TestDeposit()
        {
            Test("Geld einzahlen", () => {
                var account = new BankAccount("TEST1", 100m);
                account.Deposit(50m);
                AssertEqual(150m, account.GetBalance(), "Saldo sollte 150 nach Einzahlung sein");
            });
        }

        private static void TestWithdrawSuccess()
        {
            Test("Erfolgreiches Abheben", () => {
                var account = new BankAccount("TEST2", 100m);
                bool success = account.Withdraw(30m);
                AssertEqual(true, success, "Abhebung sollte erfolgreich sein");
                AssertEqual(70m, account.GetBalance(), "Saldo sollte 70 nach Abhebung sein");
            });
        }

        private static void TestWithdrawInsufficientFunds()
        {
            Test("Abhebung bei unzureichendem Saldo", () => {
                var account = new BankAccount("TEST3", 50m);
                bool success = account.Withdraw(100m);
                AssertEqual(false, success, "Abhebung sollte fehlschlagen");
                AssertEqual(50m, account.GetBalance(), "Saldo sollte unverändert bei 50 bleiben");
            });
        }

        private static void TestDepositNegativeAmount()
        {
            Test("Negative Einzahlung", () => {
                var account = new BankAccount("TEST4", 100m);
                try
                {
                    account.Deposit(-50m);
                    throw new Exception("Expected exception was not thrown for negative deposit");
                }
                catch (ArgumentException)
                {
                    // Erwartet
                    AssertEqual(100m, account.GetBalance(), "Saldo sollte unverändert bei 100 bleiben");
                }
                catch (InvalidOperationException)
                {
                    // Auch akzeptabel
                    AssertEqual(100m, account.GetBalance(), "Saldo sollte unverändert bei 100 bleiben");
                }
            });
        }

        private static void TestWithdrawNegativeAmount()
        {
            Test("Negative Abhebung", () => {
                var account = new BankAccount("TEST5", 100m);
                try
                {
                    account.Withdraw(-30m);
                    throw new Exception("Expected exception was not thrown for negative withdrawal");
                }
                catch (ArgumentException)
                {
                    // Erwartet
                    AssertEqual(100m, account.GetBalance(), "Saldo sollte unverändert bei 100 bleiben");
                }
                catch (InvalidOperationException)
                {
                    // Auch akzeptabel
                    AssertEqual(100m, account.GetBalance(), "Saldo sollte unverändert bei 100 bleiben");
                }
            });
        }

        private static void TestMultipleTransactions()
        {
            Test("Mehrere Transaktionen", () => {
                var account = new BankAccount("TEST6", 1000m);
                
                account.Deposit(500m);   // 1000 + 500 = 1500
                AssertEqual(1500m, account.GetBalance(), "Nach 1. Einzahlung: 1500");
                
                bool withdraw1 = account.Withdraw(200m);  // 1500 - 200 = 1300
                AssertEqual(true, withdraw1, "1. Abhebung sollte erfolgreich sein");
                AssertEqual(1300m, account.GetBalance(), "Nach 1. Abhebung: 1300");
                
                account.Deposit(100m);   // 1300 + 100 = 1400
                AssertEqual(1400m, account.GetBalance(), "Nach 2. Einzahlung: 1400");
                
                bool withdraw2 = account.Withdraw(50m);   // 1400 - 50 = 1350
                AssertEqual(true, withdraw2, "2. Abhebung sollte erfolgreich sein");
                AssertEqual(1350m, account.GetBalance(), "Finaler Saldo sollte 1350 sein");
            });
        }

        private static void TestAccountMethods()
        {
            Test("Konto-Methoden sind korrekt", () => {
                var account = new BankAccount("METH123", 777m);
                
                // Methoden sollten korrekte Werte zurückgeben
                AssertEqual("METH123", account.GetAccountNumber(), "GetAccountNumber() sollte korrekt sein");
                AssertEqual(777m, account.GetBalance(), "GetBalance() sollte korrekt sein");
                
                // Nach einer Transaktion sollte sich GetBalance() aktualisieren
                account.Deposit(223m); // 777 + 223 = 1000
                AssertEqual(1000m, account.GetBalance(), "GetBalance() sollte nach Transaktion aktualisiert sein");
            });
        }

        private static void TestToString()
        {
            Test("ToString Methode", () => {
                var account = new BankAccount("STR123", 1234.56m);
                string result = account.ToString();
                
                // Sollte nicht null oder leer sein
                AssertNotNullOrEmpty(result, "ToString sollte nicht null oder leer sein");
                
                // Sollte Kontonummer enthalten
                AssertTrue(result.Contains("STR123"), "ToString sollte Kontonummer 'STR123' enthalten");
                
                // Sollte Saldo-Information enthalten (irgendeine Form von 1234)
                AssertTrue(result.Contains("1234"), "ToString sollte Saldo-Information enthalten");
            });
        }

        private static void TestSolveMethod()
        {
            Test("Solve Methode", () => {
                // Test: Anfangssaldo 1000, Einzahlung 500, Abhebung 200
                // Ergebnis: 1000 + 500 - 200 = 1300
                var result = Challenge.Solve(new int[] { 1000, 500, 200 });
                AssertEqual(1300, result, "Solve sollte korrekten Endsaldo 1300 zurückgeben");
            });
        }

        private static void TestEmptyAccountNumber()
        {
            Test("Leere Kontonummer", () => {
                try
                {
                    var account = new BankAccount("", 100m);
                    // Wenn keine Exception geworfen wird, sollte zumindest die Kontonummer gesetzt sein
                    AssertEqual("", account.GetAccountNumber(), "Leere Kontonummer sollte akzeptiert werden");
                }
                catch (ArgumentException)
                {
                    // Auch akzeptabel wenn leere Kontonummer nicht erlaubt ist
                }
            });
        }

        private static void TestLargeAmounts()
        {
            Test("Große Beträge", () => {
                var account = new BankAccount("LARGE", 1000000m);
                account.Deposit(500000m);
                AssertEqual(1500000m, account.GetBalance(), "Große Beträge sollten korrekt verarbeitet werden");
                
                bool success = account.Withdraw(750000m);
                AssertEqual(true, success, "Abhebung großer Beträge sollte funktionieren");
                AssertEqual(750000m, account.GetBalance(), "Saldo nach großer Abhebung sollte korrekt sein");
            });
        }

        private static void TestDecimalPrecision()
        {
            Test("Dezimal-Präzision", () => {
                var account = new BankAccount("DECIMAL", 100.50m);
                account.Deposit(25.25m);
                AssertEqual(125.75m, account.GetBalance(), "Dezimalstellen sollten korrekt verarbeitet werden");
                
                bool success = account.Withdraw(50.33m);
                AssertEqual(true, success, "Abhebung mit Dezimalstellen sollte funktionieren");
                AssertEqual(75.42m, account.GetBalance(), "Saldo mit Dezimalstellen sollte präzise sein");
            });
        }

        private static void Test(string description, Action testAction)
        {
            totalTests++;
            try
            {
                testAction();
                Console.WriteLine($"  ✅ {description}");
                testsPassed++;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"  ❌ {description}: {ex.Message}");
            }
        }

        private static void AssertEqual<T>(T expected, T actual, string message = "")
        {
            if (!Equals(expected, actual))
            {
                throw new Exception($"{message}. Erwartet: {expected}, Erhalten: {actual}");
            }
        }

        private static void AssertTrue(bool condition, string message = "")
        {
            if (!condition)
            {
                throw new Exception($"{message}. Bedingung sollte true sein");
            }
        }

        private static void AssertNotNullOrEmpty(string value, string message = "")
        {
            if (string.IsNullOrEmpty(value))
            {
                throw new Exception($"{message}. Wert sollte nicht null oder leer sein");
            }
        }
    }
}