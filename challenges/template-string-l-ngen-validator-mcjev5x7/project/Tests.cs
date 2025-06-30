using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 4: String-Längen-Validator Tests");
            Console.WriteLine(new string('=', 50));

            TestValidLength();
            TestTooShort();
            TestTooLong();
            TestExactBoundaries();
            TestNullString();
            TestEmptyString();
            TestWhitespaceString();
            TestSingleCharacter();
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

        private static void TestValidLength()
        {
            Test("Gültige Länge", () => {
                var result = Challenge.IsValidLength("Hello", 3, 10);
                AssertEqual(true, result, "Hello (5 Zeichen) sollte zwischen 3-10 gültig sein");
            });
        }

        private static void TestTooShort()
        {
            Test("Zu kurz", () => {
                var result = Challenge.IsValidLength("Hi", 3, 10);
                AssertEqual(false, result, "Hi (2 Zeichen) sollte für min=3 zu kurz sein");
            });
        }

        private static void TestTooLong()
        {
            Test("Zu lang", () => {
                var result = Challenge.IsValidLength("VeryLongTextHere", 3, 10);
                AssertEqual(false, result, "VeryLongTextHere sollte für max=10 zu lang sein");
            });
        }

        private static void TestExactBoundaries()
        {
            Test("Exakte Grenzen (min)", () => {
                var result = Challenge.IsValidLength("ABC", 3, 10);
                AssertEqual(true, result, "ABC (genau 3 Zeichen) sollte gültig sein");
            });
            
            Test("Exakte Grenzen (max)", () => {
                var result = Challenge.IsValidLength("1234567890", 3, 10);
                AssertEqual(true, result, "10 Zeichen sollten für max=10 gültig sein");
            });
        }

        private static void TestNullString()
        {
            Test("Null String", () => {
                var result = Challenge.IsValidLength(null, 1, 5);
                AssertEqual(false, result, "null sollte ungültig sein");
            });
        }

        private static void TestEmptyString()
        {
            Test("Leerer String (erlaubt)", () => {
                var result = Challenge.IsValidLength("", 0, 5);
                AssertEqual(true, result, "Leerer String sollte für min=0 gültig sein");
            });
            
            Test("Leerer String (nicht erlaubt)", () => {
                var result = Challenge.IsValidLength("", 1, 5);
                AssertEqual(false, result, "Leerer String sollte für min=1 ungültig sein");
            });
        }

        private static void TestWhitespaceString()
        {
            Test("Whitespace String", () => {
                var result = Challenge.IsValidLength("   ", 1, 5);
                AssertEqual(true, result, "3 Leerzeichen sollten als 3 Zeichen zählen");
            });
        }

        private static void TestSingleCharacter()
        {
            Test("Einzelnes Zeichen", () => {
                var result = Challenge.IsValidLength("X", 1, 1);
                AssertEqual(true, result, "Einzelnes Zeichen sollte für min=max=1 gültig sein");
            });
        }

        private static void TestSolveMethod()
        {
            Test("Solve Methode", () => {
                var result = Challenge.Solve(new int[] { 5, 2, 15, 8 });
                AssertEqual(2, result, "Von 4 Test-Strings sollten 2 gültig sein (5 und 8 Zeichen)");
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