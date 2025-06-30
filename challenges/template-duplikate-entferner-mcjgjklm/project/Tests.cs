using System;
using System.Linq;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 10: Duplikate-Entferner Tests");
            Console.WriteLine(new string('=', 50));

            TestSimpleDuplicates();
            TestAllDuplicates();
            TestNoDuplicates();
            TestSingleElement();
            TestEmptyArray();
            TestNullArray();
            TestOrderPreservation();
            TestMixedNumbers();
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

        private static void TestSimpleDuplicates()
        {
            Test("Einfache Duplikate", () => {
                var result = Challenge.RemoveDuplicates(new int[] { 1, 2, 2, 3, 1, 4 });
                var expected = new int[] { 1, 2, 3, 4 };
                AssertArrayEqual(expected, result, "Duplikate sollten entfernt werden");
            });
        }

        private static void TestAllDuplicates()
        {
            Test("Alle gleich", () => {
                var result = Challenge.RemoveDuplicates(new int[] { 5, 5, 5, 5 });
                var expected = new int[] { 5 };
                AssertArrayEqual(expected, result, "Sollte nur ein Element übrig bleiben");
            });
        }

        private static void TestNoDuplicates()
        {
            Test("Keine Duplikate", () => {
                var result = Challenge.RemoveDuplicates(new int[] { 1, 2, 3, 4 });
                var expected = new int[] { 1, 2, 3, 4 };
                AssertArrayEqual(expected, result, "Array sollte unverändert bleiben");
            });
        }

        private static void TestSingleElement()
        {
            Test("Einzelnes Element", () => {
                var result = Challenge.RemoveDuplicates(new int[] { 42 });
                var expected = new int[] { 42 };
                AssertArrayEqual(expected, result, "Einzelnes Element sollte beibehalten werden");
            });
        }

        private static void TestEmptyArray()
        {
            Test("Leeres Array", () => {
                var result = Challenge.RemoveDuplicates(new int[] { });
                var expected = new int[] { };
                AssertArrayEqual(expected, result, "Leeres Array sollte leer bleiben");
            });
        }

        private static void TestNullArray()
        {
            Test("Null Array", () => {
                var result = Challenge.RemoveDuplicates(null);
                var expected = new int[] { };
                AssertArrayEqual(expected, result, "Null sollte leeres Array zurückgeben");
            });
        }

        private static void TestOrderPreservation()
        {
            Test("Reihenfolge beibehalten", () => {
                var result = Challenge.RemoveDuplicates(new int[] { 3, 1, 4, 1, 5, 9, 2, 6, 5 });
                var expected = new int[] { 3, 1, 4, 5, 9, 2, 6 };
                AssertArrayEqual(expected, result, "Reihenfolge des ersten Vorkommens sollte beibehalten werden");
            });
        }

        private static void TestMixedNumbers()
        {
            Test("Gemischte Zahlen", () => {
                var result = Challenge.RemoveDuplicates(new int[] { -1, 0, -1, 1, 0, 1 });
                var expected = new int[] { -1, 0, 1 };
                AssertArrayEqual(expected, result, "Negative und positive Zahlen korrekt behandeln");
            });
        }

        private static void TestSolveMethod()
        {
            Test("Solve Methode", () => {
                var result = Challenge.Solve(new int[] { 1, 2, 2, 3, 1, 4 });
                AssertEqual(4, result, "Sollte 4 eindeutige Elemente zählen");
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

        private static void AssertArrayEqual(int[] expected, int[] actual, string message = "")
        {
            if (expected == null && actual == null) return;
            if (expected == null || actual == null)
                throw new Exception($"{message}. Ein Array ist null");
            if (expected.Length != actual.Length)
                throw new Exception($"{message}. Längen unterschiedlich: {expected.Length} vs {actual.Length}");
            
            for (int i = 0; i < expected.Length; i++)
            {
                if (expected[i] != actual[i])
                    throw new Exception($"{message}. Index {i}: Erwartet {expected[i]}, Erhalten {actual[i]}");
            }
        }
    }
}