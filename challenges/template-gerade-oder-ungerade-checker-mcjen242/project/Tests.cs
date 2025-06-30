using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 2: Gerade oder Ungerade Tests");
            Console.WriteLine(new string('=', 50));

            TestEvenPositive();
            TestOddPositive();
            TestEvenNegative();
            TestOddNegative();
            TestZero();
            TestMixedArray();
            TestEmptyArray();
            TestOnlyEven();
            TestOnlyOdd();

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

        private static void TestEvenPositive()
        {
            Test("Positive gerade Zahl", () => {
                var result = Challenge.IsEven(4);
                AssertEqual(true, result, "4 sollte gerade sein");
            });
        }

        private static void TestOddPositive()
        {
            Test("Positive ungerade Zahl", () => {
                var result = Challenge.IsEven(7);
                AssertEqual(false, result, "7 sollte ungerade sein");
            });
        }

        private static void TestEvenNegative()
        {
            Test("Negative gerade Zahl", () => {
                var result = Challenge.IsEven(-2);
                AssertEqual(true, result, "-2 sollte gerade sein");
            });
        }

        private static void TestOddNegative()
        {
            Test("Negative ungerade Zahl", () => {
                var result = Challenge.IsEven(-3);
                AssertEqual(false, result, "-3 sollte ungerade sein");
            });
        }

        private static void TestZero()
        {
            Test("Null ist gerade", () => {
                var result = Challenge.IsEven(0);
                AssertEqual(true, result, "0 sollte gerade sein");
            });
        }

        private static void TestMixedArray()
        {
            Test("Gemischtes Array", () => {
                var result = Challenge.Solve(new int[] { 1, 2, 3, 4, 5, 6 });
                AssertEqual(3, result, "Array sollte 3 gerade Zahlen haben (2, 4, 6)");
            });
        }

        private static void TestEmptyArray()
        {
            Test("Leeres Array", () => {
                var result = Challenge.Solve(new int[] { });
                AssertEqual(0, result, "Leeres Array sollte 0 gerade Zahlen haben");
            });
        }

        private static void TestOnlyEven()
        {
            Test("Nur gerade Zahlen", () => {
                var result = Challenge.Solve(new int[] { 2, 4, 6, 8 });
                AssertEqual(4, result, "Array sollte 4 gerade Zahlen haben");
            });
        }

        private static void TestOnlyOdd()
        {
            Test("Nur ungerade Zahlen", () => {
                var result = Challenge.Solve(new int[] { 1, 3, 5, 7 });
                AssertEqual(0, result, "Array sollte 0 gerade Zahlen haben");
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