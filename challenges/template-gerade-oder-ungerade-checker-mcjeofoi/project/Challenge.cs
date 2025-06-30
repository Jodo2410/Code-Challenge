using System;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 2: Gerade oder Ungerade Checker
    /// Bestimme ob eine Zahl gerade oder ungerade ist
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Prüft ob eine Zahl gerade ist
        /// </summary>
        /// <param name="number">Die zu prüfende Zahl</param>
        /// <returns>true wenn gerade, false wenn ungerade</returns>
        public static bool IsEven(int number)
        {
            
            // Implementierung hier
        }
        
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// Zählt die Anzahl gerader Zahlen im Array
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length == 0)
                return 0;
            
            int evenCount = 0;
            foreach (int number in input)
            {
                if (IsEven(number))
                    evenCount++;
            }
            return evenCount;
        }
    }
}