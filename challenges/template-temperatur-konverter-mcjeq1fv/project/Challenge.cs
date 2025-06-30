using System;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 3: Temperatur-Konverter
    /// Konvertiere zwischen Celsius und Fahrenheit
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Konvertiert Celsius zu Fahrenheit
        /// </summary>
        /// <param name="celsius">Temperatur in Celsius</param>
        /// <returns>Temperatur in Fahrenheit</returns>
        public static double CelsiusToFahrenheit(double celsius)
        {
            
            // Formel: F = C × 9/5 + 32
            // Implementierung hier
        }
        
        /// <summary>
        /// Konvertiert Fahrenheit zu Celsius
        /// </summary>
        /// <param name="fahrenheit">Temperatur in Fahrenheit</param>
        /// <returns>Temperatur in Celsius</returns>
        public static double FahrenheitToCelsius(double fahrenheit)
        {
            
            // Formel: C = (F - 32) × 5/9
            // Implementierung hier
        }
        
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// Konvertiert Array von Celsius-Werten zu Fahrenheit und gibt Summe zurück
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length == 0)
                return 0;
            
            double sum = 0;
            foreach (int celsius in input)
            {
                double fahrenheit = CelsiusToFahrenheit(celsius);
                sum += fahrenheit;
            }
            return (int)Math.Round(sum);
        }
    }
}