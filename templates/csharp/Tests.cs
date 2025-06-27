using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Starte Challenge Tests");
            Console.WriteLine(new string('=', 50));

            TestExample();
            TestEdgeCase();

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

        private static void TestExample()
        {
            Test("Beispiel Test", () => {
                var result = Challenge.Solve(new int[] { 1, 2 });
                AssertEqual(3, result, "1 + 2 sollte 3 ergeben");
            });
        }

        private static void TestEdgeCase()
        {
            Test("Edge Case Test", () => {
                var result = Challenge.Solve(new int[] { });
                AssertEqual(0, result, "Leeres Array sollte 0 ergeben");
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
