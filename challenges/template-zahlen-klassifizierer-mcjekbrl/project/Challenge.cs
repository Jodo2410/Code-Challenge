using System;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 1: Zahlen-Klassifizierer
    /// Klassifiziere eine Zahl als positiv, negativ oder null
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Klassifiziert eine Zahl als "positive", "negative" oder "zero"
        /// </summary>
        /// <param name="number">Die zu klassifizierende Zahl</param>
        /// <returns>Klassifikation als String</returns>
        public static string ClassifyNumber(int number)
        {
            
            // Implementierung hier
        }
        
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length == 0)
                return 0;
            
            int result = 0;
            foreach (int number in input)
            {
                string classification = ClassifyNumber(number);
                switch (classification)
                {
                    case "positive": result += 1; break;
                    case "negative": result -= 1; break;
                    case "zero": result += 0; break;
                }
            }
            return result;
        }
    }
}