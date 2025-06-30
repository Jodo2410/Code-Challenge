using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 1: Zahlen-Klassifizierer Tests");
            Console.WriteLine(new string('=', 50));

            TestPositiveNumber();
            TestNegativeNumber();
            TestZero();
            TestMixedArray();
            TestEmptyArray();
            TestSinglePositive();
            TestSingleNegative();

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

        private static void TestPositiveNumber()
        {
            Test("Positive Zahl", () => {
                var result = Challenge.ClassifyNumber(5);
                AssertEqual("positive", result, "5 sollte als positive klassifiziert werden");
            });
        }

        private static void TestNegativeNumber()
        {
            Test("Negative Zahl", () => {
                var result = Challenge.ClassifyNumber(-3);
                AssertEqual("negative", result, "-3 sollte als negative klassifiziert werden");
            });
        }

        private static void TestZero()
        {
            Test("Null", () => {
                var result = Challenge.ClassifyNumber(0);
                AssertEqual("zero", result, "0 sollte als zero klassifiziert werden");
            });
        }

        private static void TestMixedArray()
        {
            Test("Gemischtes Array", () => {
                var result = Challenge.Solve(new int[] { 1, -1, 2, 0, -2 });
                AssertEqual(0, result, "Gemischtes Array sollte 0 ergeben (1-1+1+0-1=0)");
            });
        }

        private static void TestEmptyArray()
        {
            Test("Leeres Array", () => {
                var result = Challenge.Solve(new int[] { });
                AssertEqual(0, result, "Leeres Array sollte 0 ergeben");
            });
        }

        private static void TestSinglePositive()
        {
            Test("Einzelne positive Zahl", () => {
                var result = Challenge.Solve(new int[] { 42 });
                AssertEqual(1, result, "Array mit einer positiven Zahl sollte 1 ergeben");
            });
        }

        private static void TestSingleNegative()
        {
            Test("Einzelne negative Zahl", () => {
                var result = Challenge.Solve(new int[] { -7 });
                AssertEqual(-1, result, "Array mit einer negativen Zahl sollte -1 ergeben");
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