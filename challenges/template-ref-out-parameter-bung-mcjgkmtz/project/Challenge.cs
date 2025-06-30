using System;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 12: Ref/Out Parameter-Übung
    /// Arbeite mit ref und out Parametern
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Tauscht die Werte von zwei Variablen
        /// </summary>
        /// <param name="a">Erste Variable (wird zu b)</param>
        /// <param name="b">Zweite Variable (wird zu a)</param>
        public static void SwapValues(ref int a, ref int b)
        {
            
            // Verwende eine temporäre Variable
            // Implementierung hier
        }
        
        /// <summary>
        /// Erweiterte Parse-Funktion mit zusätzlichen Informationen
        /// </summary>
        /// <param name="input">String zum Parsen</param>
        /// <param name="number">Geparste Zahl (0 bei Fehler)</param>
        /// <param name="isValid">true wenn erfolgreich geparst</param>
        /// <returns>true wenn parsing erfolgreich</returns>
        public static bool TryParseAdvanced(string input, out int number, out bool isValid)
        {
            
            // Setze number und isValid entsprechend
            // Verwende int.TryParse() als Basis
            // Implementierung hier
        }
        
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// Testet beide Funktionen und gibt Ergebnis zurück
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length < 2)
                return 0;
            
            // Test SwapValues
            int a = input[0];
            int b = input[1];
            int originalSum = a + b;
            
            SwapValues(ref a, ref b);
            int swappedSum = a + b; // Sollte gleich sein
            
            // Test TryParseAdvanced
            string testString = input[0].ToString();
            bool parseResult = TryParseAdvanced(testString, out int parsed, out bool valid);
            
            // Kombiniere Ergebnisse für Test
            return (originalSum == swappedSum && parseResult && valid) ? 1 : 0;
        }
    }
}