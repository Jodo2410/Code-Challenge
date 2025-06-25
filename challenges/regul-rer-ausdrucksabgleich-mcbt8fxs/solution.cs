/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public class RegexMatcher
{
    /// <summary>
    /// Prüft, ob ein String mit einem regulären Ausdruck übereinstimmt.
    /// </summary>
    /// <param name="s">Der zu prüfende String</param>
    /// <param name="p">Das reguläre Ausdrucksmuster</param>
    /// <returns>True, wenn der String mit dem Muster übereinstimmt, sonst False</returns>
    public static bool IsMatch(string s, string p)
    {
        // Behandle null-Eingaben
        if (s == null || p == null)
        {
            return false;
        }
        
        int m = s.Length;
        int n = p.Length;
        
        // DP-Tabelle: dp[i,j] bedeutet, ob s[0...i-1] und p[0...j-1] übereinstimmen
        bool[,] dp = new bool[m + 1, n + 1];
        
        // Basisfall: Leerer String passt zu leerem Muster
        dp[0, 0] = true;
        
        // Behandle Fälle, wo s leer ist, aber p '*' enthält
        // z.B. p = "a*" oder "a*b*" - diese können auf leeren String passen
        for (int j = 1; j <= n; j++)
        {
            // Wenn das aktuelle Zeichen ein '*' ist und das vorherige Zeichen+"*" zu leerem String passt
            if (p[j - 1] == '*' && j > 1)
            {
                dp[0, j] = dp[0, j - 2]; // Ignoriere das aktuelle Zeichen und das vorherige Zeichen
            }
        }
        
        // Hauptlogik für die DP-Tabelle
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                char currentChar = p[j - 1];
                
                if (currentChar == '*')
                {
                    // Wenn der aktuelle Char '*' ist, haben wir zwei Optionen:
                    // 1. Ignoriere das '*' und das vorherige Zeichen (j-2)
                    // 2. Wenn das vorherige Zeichen mit dem aktuellen String-Zeichen übereinstimmt, 
                    //    können wir entweder den aktuellen String-Zeichen verwenden oder nicht
                    
                    if (j > 1) // Stelle sicher, dass es ein vorheriges Zeichen gibt
                    {
                        // Option 1: Ignoriere das '*' und das vorherige Zeichen
                        dp[i, j] = dp[i, j - 2];
                        
                        // Option 2: Verwende das '*' mit dem vorherigen Zeichen
                        char prevChar = p[j - 2];
                        
                        // Wenn das vorherige Zeichen mit dem aktuellen String-Zeichen übereinstimmt
                        // oder das vorherige Zeichen ein '.' ist (passt zu allem)
                        if (prevChar == '.' || prevChar == s[i - 1])
                        {
                            // Entweder ignorieren oder den aktuellen String-Zeichen verwenden
                            dp[i, j] = dp[i, j] || dp[i - 1, j];
                        }
                    }
                }
                else if (currentChar == '.' || currentChar == s[i - 1])
                {
                    // Wenn der aktuelle Char '.' ist oder direkt mit dem String-Zeichen übereinstimmt,
                    // hängt die Übereinstimmung davon ab, ob die vorherigen Zeichen übereinstimmen
                    dp[i, j] = dp[i - 1, j - 1];
                }
                // Andernfalls bleibt dp[i, j] false
            }
        }
        
        return dp[m, n];
    }
}