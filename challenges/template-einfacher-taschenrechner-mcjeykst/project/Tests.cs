using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 5: Einfacher Taschenrechner Tests");
            Console.WriteLine(new string('=', 50));

            TestAddition();
            TestSubtraction();
            TestMultiplication();
            TestDivision();
            TestDivisionByZero();
            TestInvalidOperation();
            TestNegativeNumbers();
            TestDecimals();
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

        private static void TestAddition()
        {
            Test("Addition", () => {
                var result = Challenge.Calculate(10, 5, '+');
                AssertEqual(15.0, result, "10 + 5 sollte 15 sein");
            });
        }

        private static void TestSubtraction()
        {
            Test("Subtraktion", () => {
                var result = Challenge.Calculate(10, 3, '-');
                AssertEqual(7.0, result, "10 - 3 sollte 7 sein");
            });
        }

        private static void TestMultiplication()
        {
            Test("Multiplikation", () => {
                var result = Challenge.Calculate(4, 6, '*');
                AssertEqual(24.0, result, "4 * 6 sollte 24 sein");
            });
        }

        private static void TestDivision()
        {
            Test("Division", () => {
                var result = Challenge.Calculate(15, 3, '/');
                AssertEqual(5.0, result, "15 / 3 sollte 5 sein");
            });
        }

        private static void TestDivisionByZero()
        {
            Test("Division durch Null", () => {
                try
                {
                    Challenge.Calculate(10, 0, '/');
                    throw new Exception("Expected exception was not thrown");
                }
                catch (DivideByZeroException)
                {
                    // Erwartet
                }
                catch (ArgumentException)
                {
                    // Auch akzeptabel
                }
            });
        }

        private static void TestInvalidOperation()
        {
            Test("Ungültiger Operator", () => {
                try
                {
                    Challenge.Calculate(5, 2, '%');
                    throw new Exception("Expected exception was not thrown");
                }
                catch (ArgumentException)
                {
                    // Erwartet
                }
            });
        }

        private static void TestNegativeNumbers()
        {
            Test("Negative Zahlen", () => {
                var result = Challenge.Calculate(-5, 3, '+');
                AssertEqual(-2.0, result, "-5 + 3 sollte -2 sein");
            });
        }

        private static void TestDecimals()
        {
            Test("Dezimalzahlen", () => {
                var result = Challenge.Calculate(7.5, 2.5, '*');
                AssertEqual(18.75, result, "7.5 * 2.5 sollte 18.75 sein");
            });
        }

        private static void TestSolveMethod()
        {
            Test("Solve Methode", () => {
                var result = Challenge.Solve(new int[] { 10, 5, 0 }); // 0 = Addition
                AssertEqual(15, result, "10 + 5 sollte 15 ergeben");
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
