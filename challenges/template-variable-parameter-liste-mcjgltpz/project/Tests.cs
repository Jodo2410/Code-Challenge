using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 13: Variable Parameter-Liste Tests");
            Console.WriteLine(new string('=', 50));

            TestAverageMultipleValues();
            TestAverageSingleValue();
            TestAverageNoValues();
            TestAverageAsArray();
            TestAverageDecimals();
            TestAverageNegatives();
            TestFindMaxMultiple();
            TestFindMaxSingle();
            TestFindMaxEmpty();
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

        private static void TestAverageMultipleValues()
        {
            Test("Durchschnitt mehrere Werte", () => {
                var result = Challenge.CalculateAverage(1.0, 2.0, 3.0, 4.0);
                AssertEqual(2.5, result, "Durchschnitt von 1,2,3,4 sollte 2.5 sein");
            });
        }

        private static void TestAverageSingleValue()
        {
            Test("Durchschnitt einzelner Wert", () => {
                var result = Challenge.CalculateAverage(5.0);
                AssertEqual(5.0, result, "Durchschnitt von 5 sollte 5 sein");
            });
        }

        private static void TestAverageNoValues()
        {
            Test("Durchschnitt keine Werte", () => {
                var result = Challenge.CalculateAverage();
                AssertEqual(0.0, result, "Durchschnitt ohne Werte sollte 0 sein");
            });
        }

        private static void TestAverageAsArray()
        {
            Test("Durchschnitt als Array", () => {
                double[] values = { 2.0, 4.0, 6.0, 8.0 };
                var result = Challenge.CalculateAverage(values);
                AssertEqual(5.0, result, "Durchschnitt von Array [2,4,6,8] sollte 5 sein");
            });
        }

        private static void TestAverageDecimals()
        {
            Test("Durchschnitt Dezimalzahlen", () => {
                var result = Challenge.CalculateAverage(1.5, 2.5, 3.5);
                AssertEqual(2.5, result, "Durchschnitt von 1.5,2.5,3.5 sollte 2.5 sein");
            });
        }

        private static void TestAverageNegatives()
        {
            Test("Durchschnitt negative Zahlen", () => {
                var result = Challenge.CalculateAverage(-1.0, -2.0, -3.0);
                AssertEqual(-2.0, result, "Durchschnitt von -1,-2,-3 sollte -2 sein");
            });
        }

        private static void TestFindMaxMultiple()
        {
            Test("FindMax mehrere Werte", () => {
                var result = Challenge.FindMax(1, 5, 3, 9, 2);
                AssertEqual(9, result, "Maximum von 1,5,3,9,2 sollte 9 sein");
            });
        }

        private static void TestFindMaxSingle()
        {
            Test("FindMax einzelner Wert", () => {
                var result = Challenge.FindMax(42);
                AssertEqual(42, result, "Maximum von 42 sollte 42 sein");
            });
        }

        private static void TestFindMaxEmpty()
        {
            Test("FindMax leer wirft Exception", () => {
                try
                {
                    Challenge.FindMax();
                    throw new Exception("Expected exception was not thrown");
                }
                catch (ArgumentException)
                {
                    // Erwartet
                }
                catch (InvalidOperationException)
                {
                    // Auch akzeptabel
                }
            });
        }

        private static void TestSolveMethod()
        {
            Test("Solve Methode", () => {
                var result = Challenge.Solve(new int[] { 1, 2, 3, 4 });
                AssertEqual(3, result, "Gerundeter Durchschnitt von 1,2,3,4 sollte 3 sein");
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