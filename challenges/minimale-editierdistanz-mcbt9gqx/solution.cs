/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public class EditDistance
{
    /// <summary>
    /// Berechnet die minimale Anzahl von Operationen, die benötigt werden, um word1 in word2 umzuwandeln.
    /// </summary>
    /// <param name="word1">Das erste Wort</param>
    /// <param name="word2">Das zweite Wort</param>
    /// <returns>Die minimale Editierdistanz</returns>
    public static int MinDistance(string word1, string word2)
    {
        // Behandle null-Eingaben
        if (word1 == null) word1 = "";
        if (word2 == null) word2 = "";
        
        int m = word1.Length;
        int n = word2.Length;
        
        // Wenn eines der Wörter leer ist, ist die Distanz gleich der Länge des anderen Wortes
        if (m == 0) return n;
        if (n == 0) return m;
        
        // Erstelle eine DP-Tabelle
        // dp[i,j] = minimale Operationen, um word1[0..i-1] in word2[0..j-1] umzuwandeln
        int[,] dp = new int[m + 1, n + 1];
        
        // Initialisiere die erste Zeile und Spalte
        // Dies repräsentiert das Umwandeln in oder von einem leeren String
        for (int i = 0; i <= m; i++)
        {
            dp[i, 0] = i; // Es kostet i Löschungen, um word1[0..i-1] in einen leeren String umzuwandeln
        }
        
        for (int j = 0; j <= n; j++)
        {
            dp[0, j] = j; // Es kostet j Einfügungen, um einen leeren String in word2[0..j-1] umzuwandeln
        }
        
        // Fülle die DP-Tabelle aus
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                // Wenn die aktuellen Zeichen gleich sind, keine Operation notwendig
                if (word1[i - 1] == word2[j - 1])
                {
                    dp[i, j] = dp[i - 1, j - 1];
                }
                else
                {
                    // Wähle die beste Operation: Einfügen, Löschen oder Ersetzen
                    dp[i, j] = 1 + Math.Min(
                        Math.Min(
                            dp[i - 1, j],   // Löschen
                            dp[i, j - 1]),  // Einfügen
                        dp[i - 1, j - 1]    // Ersetzen
                    );
                }
            }
        }
        
        // Das Ergebnis ist in der unteren rechten Ecke der Tabelle
        return dp[m, n];
    }
}