using System;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 4: String-Längen-Validator
    /// Validiere Strings nach Längen-Kriterien
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Prüft ob ein String die Längen-Anforderungen erfüllt
        /// </summary>
        /// <param name="text">Der zu prüfende String</param>
        /// <param name="minLength">Minimale Länge (inklusive)</param>
        /// <param name="maxLength">Maximale Länge (inklusive)</param>
        /// <returns>true wenn Länge zwischen min und max liegt</returns>
        public static bool IsValidLength(string text, int minLength, int maxLength)
        {
            
            // Berücksichtige null-Werte und empty strings
            // Implementierung hier
        }
        
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// Zählt gültige Strings in einem Array
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length == 0)
                return 0;
            
            // Für Template-Kompatibilität: erstelle Test-Strings
            string[] testStrings = new string[input.Length];
            for (int i = 0; i < input.Length; i++)
            {
                testStrings[i] = new string('A', Math.Abs(input[i]));
            }
            
            int validCount = 0;
            foreach (string str in testStrings)
            {
                if (IsValidLength(str, 3, 10))
                    validCount++;
            }
            return validCount;
        }
    }
}