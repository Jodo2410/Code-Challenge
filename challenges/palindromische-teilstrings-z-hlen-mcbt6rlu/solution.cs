/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public class PalindromeCounter
{
    /// <summary>
    /// Zählt die Anzahl der palindromischen Teilstrings in einem gegebenen String.
    /// </summary>
    /// <param name="s">Der zu untersuchende String</param>
    /// <returns>Die Anzahl der palindromischen Teilstrings</returns>
    public static int CountPalindromicSubstrings(string s)
    {
        // Sonderfall: Leerer String
        if (string.IsNullOrEmpty(s))
        {
            return 0;
        }
        
        int count = 0;
        int n = s.Length;
        
        // Für jeden Zeichen-Index im String
        for (int i = 0; i < n; i++)
        {
            // Überprüfe ungerade Länge Palindrome (mit Zentrum i)
            count += ExpandAroundCenter(s, i, i);
            
            // Überprüfe gerade Länge Palindrome (mit Zentrum zwischen i und i+1)
            if (i < n - 1)
            {
                count += ExpandAroundCenter(s, i, i + 1);
            }
        }
        
        return count;
    }
    
    /// <summary>
    /// Erweitert die Suche von einem Zentrum aus und zählt Palindrome.
    /// </summary>
    /// <param name="s">Der zu untersuchende String</param>
    /// <param name="left">Der linke Index des Zentrums</param>
    /// <param name="right">Der rechte Index des Zentrums</param>
    /// <returns>Die Anzahl der gefundenen Palindrome</returns>
    private static int ExpandAroundCenter(string s, int left, int right)
    {
        int count = 0;
        
        // Erweitern, solange wir innerhalb der Grenzen bleiben und es ein Palindrom ist
        while (left >= 0 && right < s.Length && s[left] == s[right])
        {
            count++; // Ein weiteres Palindrom gefunden
            left--; // Nach links erweitern
            right++; // Nach rechts erweitern
        }
        
        return count;
    }
}