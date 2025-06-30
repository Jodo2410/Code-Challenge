using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 12: Ref/Out Parameter-Übung Tests");
            Console.WriteLine(new string('=', 50));

            TestSwapPositiveNumbers();
            TestSwapNegativeNumbers();
            TestSwapZeros();
            TestSwapSameNumbers();
            TestTryParseValidNumber();
            TestTryParseInvalidNumber();
            TestTryParseEmptyString();
            TestTryParseNullString();
            TestSolveMethod();

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

        private static void TestSwapPositiveNumbers()
        {
            Test("Swap positive Zahlen", () => {
                int a = 5, b = 10;
                Challenge.SwapValues(ref a, ref b);
                AssertEqual(10, a, "a sollte 10 sein nach Swap");
                AssertEqual(5, b, "b sollte 5 sein nach Swap");
            });
        }

        private static void TestSwapNegativeNumbers()
        {
            Test("Swap negative Zahlen", () => {
                int a = -3, b = -7;
                Challenge.SwapValues(ref a, ref b);
                AssertEqual(-7, a, "a sollte -7 sein nach Swap");
                AssertEqual(-3, b, "b sollte -3 sein nach Swap");
            });
        }

        private static void TestSwapZeros()
        {
            Test("Swap mit Null", () => {
                int a = 0, b = 42;
                Challenge.SwapValues(ref a, ref b);
                AssertEqual(42, a, "a sollte 42 sein nach Swap");
                AssertEqual(0, b, "b sollte 0 sein nach Swap");
            });
        }

        private static void TestSwapSameNumbers()
        {
            Test("Swap gleiche Zahlen", () => {
                int a = 7, b = 7;
                Challenge.SwapValues(ref a, ref b);
                AssertEqual(7, a, "a sollte 7 bleiben");
                AssertEqual(7, b, "b sollte 7 bleiben");
            });
        }

        private static void TestTryParseValidNumber()
        {
            Test("TryParse gültige Zahl", () => {
                bool result = Challenge.TryParseAdvanced("123", out int number, out bool isValid);
                AssertEqual(true, result, "Parsing sollte erfolgreich sein");
                AssertEqual(123, number, "Geparste Zahl sollte 123 sein");
                AssertEqual(true, isValid, "isValid sollte true sein");
            });
        }

        private static void TestTryParseInvalidNumber()
        {
            Test("TryParse ungültige Zahl", () => {
                bool result = Challenge.TryParseAdvanced("abc", out int number, out bool isValid);
                AssertEqual(false, result, "Parsing sollte fehlschlagen");
                AssertEqual(0, number, "number sollte 0 sein bei Fehler");
                AssertEqual(false, isValid, "isValid sollte false sein");
            });
        }

        private static void TestTryParseEmptyString()
        {
            Test("TryParse leerer String", () => {
                bool result = Challenge.TryParseAdvanced("", out int number, out bool isValid);
                AssertEqual(false, result, "Leerer String sollte fehlschlagen");
                AssertEqual(0, number, "number sollte 0 sein");
                AssertEqual(false, isValid, "isValid sollte false sein");
            });
        }

        private static void TestTryParseNullString()
        {
            Test("TryParse null String", () => {
                bool result = Challenge.TryParseAdvanced(null, out int number, out bool isValid);
                AssertEqual(false, result, "null sollte fehlschlagen");
                AssertEqual(0, number, "number sollte 0 sein");
                AssertEqual(false, isValid, "isValid sollte false sein");
            });
        }

        private static void TestSolveMethod()
        {
            Test("Solve Methode", () => {
                var result = Challenge.Solve(new int[] { 5, 10 });
                AssertEqual(1, result, "Solve sollte 1 zurückgeben wenn alle Tests bestehen");
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
    }
}