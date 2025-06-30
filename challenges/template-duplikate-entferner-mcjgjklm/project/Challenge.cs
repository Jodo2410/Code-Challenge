using System;
using System.Collections.Generic;
using System.Linq;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 10: Duplikate-Entferner
    /// Entferne Duplikate aus einem Array
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Entfernt alle Duplikate aus einem Array und behält die Reihenfolge bei
        /// </summary>
        /// <param name="numbers">Das Array mit möglichen Duplikaten</param>
        /// <returns>Neues Array ohne Duplikate</returns>
        public static int[] RemoveDuplicates(int[] numbers)
        {
            
            // Verwende HashSet und List für die Lösung
            // Behandle null und leere Arrays
            // Implementierung hier
        }
        
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// Gibt die Anzahl eindeutiger Elemente zurück
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length == 0)
                return 0;
            
            int[] unique = RemoveDuplicates(input);
            return unique.Length;
        }
    }
}