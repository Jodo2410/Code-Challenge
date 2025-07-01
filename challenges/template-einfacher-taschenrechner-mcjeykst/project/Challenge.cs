### 💻 **Challenge.cs** (Leere Vorlage)
```csharp
using System;

namespace ChallengeTemplate
{
    /// <summary>
    /// Challenge 5: Einfacher Taschenrechner
    /// Implementiere Grundrechenarten
    /// </summary>
    public class Challenge
    {
        /// <summary>
        /// Führt eine mathematische Operation aus
        /// </summary>
        /// <param name="a">Erste Zahl</param>
        /// <param name="b">Zweite Zahl</param>
        /// <param name="operation">Operator (+, -, *, /)</param>
        /// <returns>Ergebnis der Berechnung</returns>
        public static double Calculate(double a, double b, char operation)
        {
            // TODO: Implementiere den Taschenrechner hier
            // Unterstütze: +, -, *, /
            // Behandle Division durch Null
            // Wirf Exception bei unbekannten Operatoren
            throw new NotImplementedException("Implementiere Calculate");
        }
        
        /// <summary>
        /// Template-kompatible Solve-Methode
        /// Führt Berechnungen basierend auf Array-Indizes aus
        /// </summary>
        public static int Solve(int[] input)
        {
            if (input == null || input.Length < 3)
                return 0;
            
            // Verwende die ersten 3 Elemente: a, b, operation-index
            double a = input[0];
            double b = input[1];
            char[] operations = { '+', '-', '*', '/' };
            int opIndex = Math.Abs(input[2]) % operations.Length;
            
            try
            {
                double result = Calculate(a, b, operations[opIndex]);
                return (int)Math.Round(result);
            }
            catch
            {
                return -1; // Fehler-Indikator
            }
        }
    }
}
