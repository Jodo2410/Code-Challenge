using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 6: Array-Summierer Tests");
            Console.WriteLine(new string('=', 50));

            TestSimpleSum();
            TestMixedNumbers();
            TestSingleElement();
            TestEmptyArray();
            TestNullArray();
            TestNegativeNumbers();
            TestZeros();
            TestLargeNumbers();

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

        private static void TestSimpleSum()
        {
            Test("Einfache Summe", () => {
                var result = Challenge.SumArray(new int[] { 1, 2, 3, 4, 5 });
                AssertEqual(15, result, "1+2+3+4+5 sollte 15 sein");
            });
        }

        private static void TestMixedNumbers()
        {
            Test("Gemischte Zahlen", () => {
                var result = Challenge.SumArray(new int[] { -1, 1, -2, 2 });
                AssertEqual(0, result, "-1+1-2+2 sollte 0 sein");
            });
        }

        private static void TestSingleElement()
        {
            Test("Einzelnes Element", () => {
                var result = Challenge.SumArray(new int[] { 42 });
                AssertEqual(42, result, "Array mit einem Element sollte das Element zurückgeben");
            });
        }

        private static void TestEmptyArray()
        {
            Test("Leeres Array", () => {
                var result = Challenge.SumArray(new int[] { });
                AssertEqual(0, result, "Leeres Array sollte 0 ergeben");
            });
        }

        private static void TestNullArray()
        {
            Test("Null Array", () => {
                var result = Challenge.SumArray(null);
                AssertEqual(0, result, "Null Array sollte 0 ergeben");
            });
        }

        private static void TestNegativeNumbers()
        {
            Test("Nur negative Zahlen", () => {
                var result = Challenge.SumArray(new int[] { -1, -2, -3 });
                AssertEqual(-6, result, "-1-2-3 sollte -6 sein");
            });
        }

        private static void TestZeros()
        {
            Test("Array mit Nullen", () => {
                var result = Challenge.SumArray(new int[] { 0, 0, 0, 5 });
                AssertEqual(5, result, "0+0+0+5 sollte 5 sein");
            });
        }

        private static void TestLargeNumbers()
        {
            Test("Große Zahlen", () => {
                var result = Challenge.SumArray(new int[] { 1000, 2000, 3000 });
                AssertEqual(6000, result, "1000+2000+3000 sollte 6000 sein");
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