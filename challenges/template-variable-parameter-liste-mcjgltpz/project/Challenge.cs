using System;
using System.Linq;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 13: Variable Parameter-Liste
    /// Nutze params für variable Argumentanzahl
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Berechnet den Durchschnitt von beliebig vielen Werten
        /// </summary>
        /// <param name="values">Variable Anzahl von double-Werten</param>
        /// <returns>Durchschnitt der Werte, 0.0 wenn keine Werte</returns>
        public static double CalculateAverage(params double[] values)
        {
            
            // Behandle den Fall mit 0 Argumenten
            // Verwende params für variable Argumentanzahl
            // Implementierung hier
        }
        
        /// <summary>
        /// Bonus: Findet das Maximum von beliebig vielen ganzen Zahlen
        /// </summary>
        /// <param name="numbers">Variable Anzahl von int-Werten</param>
        /// <returns>Maximum der Werte</returns>
        public static int FindMax(params int[] numbers)
        {
            
            // Wirf Exception bei leerem Array
            // Implementierung hier
        }
        
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// Berechnet Durchschnitt des Input-Arrays
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length == 0)
                return 0;
            
            // Konvertiere int[] zu double[] und berechne Durchschnitt
            double[] doubleValues = input.Select(x => (double)x).ToArray();
            double average = CalculateAverage(doubleValues);
            return (int)Math.Round(average);
        }
    }
}