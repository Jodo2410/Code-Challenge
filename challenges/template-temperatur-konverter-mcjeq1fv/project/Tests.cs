using System;

namespace ChallengeTemplate
{
    public class Tests
    {
        private static int testsPassed = 0;
        private static int totalTests = 0;

        public static void RunAllTests()
        {
            Console.WriteLine("🧪 Challenge 3: Temperatur-Konverter Tests");
            Console.WriteLine(new string('=', 50));

            TestCelsiusToFahrenheitFreezing();
            TestCelsiusToFahrenheitBoiling();
            TestFahrenheitToCelsiusFreezing();
            TestFahrenheitToCelsiusBoiling();
            TestCelsiusToFahrenheitZero();
            TestNegativeTemperatures();
            TestRoomTemperature();
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

        private static void TestCelsiusToFahrenheitFreezing()
        {
            Test("Celsius zu Fahrenheit (Gefrierpunkt)", () => {
                var result = Challenge.CelsiusToFahrenheit(0);
                AssertEqual(32.0, result, "0°C sollte 32°F sein");
            });
        }

        private static void TestCelsiusToFahrenheitBoiling()
        {
            Test("Celsius zu Fahrenheit (Siedepunkt)", () => {
                var result = Challenge.CelsiusToFahrenheit(100);
                AssertEqual(212.0, result, "100°C sollte 212°F sein");
            });
        }

        private static void TestFahrenheitToCelsiusFreezing()
        {
            Test("Fahrenheit zu Celsius (Gefrierpunkt)", () => {
                var result = Challenge.FahrenheitToCelsius(32);
                AssertEqual(0.0, result, "32°F sollte 0°C sein");
            });
        }

        private static void TestFahrenheitToCelsiusBoiling()
        {
            Test("Fahrenheit zu Celsius (Siedepunkt)", () => {
                var result = Challenge.FahrenheitToCelsius(212);
                AssertEqual(100.0, result, "212°F sollte 100°C sein");
            });
        }

        private static void TestCelsiusToFahrenheitZero()
        {
            Test("Absolute Zero Celsius", () => {
                var result = Challenge.CelsiusToFahrenheit(-273.15);
                AssertEqual(-459.67, Math.Round(result, 2), "-273.15°C sollte -459.67°F sein");
            });
        }

        private static void TestNegativeTemperatures()
        {
            Test("Negative Temperaturen", () => {
                var result = Challenge.CelsiusToFahrenheit(-10);
                AssertEqual(14.0, result, "-10°C sollte 14°F sein");
            });
        }

        private static void TestRoomTemperature()
        {
            Test("Raumtemperatur", () => {
                var celsius = Challenge.FahrenheitToCelsius(68);
                AssertEqual(20.0, Math.Round(celsius, 1), "68°F sollte ca. 20°C sein");
            });
        }

        private static void TestSolveMethod()
        {
            Test("Solve Methode", () => {
                var result = Challenge.Solve(new int[] { 0, 100 });
                AssertEqual(244, result, "0°C + 100°C zu Fahrenheit sollte 244 ergeben (32 + 212)");
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