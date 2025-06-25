/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;
using System.Collections.Generic;
using System.Text.RegularExpressions;

public class AnagramChecker
{
    /// <summary>
    /// Prüft, ob zwei Strings Anagramme voneinander sind.
    /// Ein Anagramm ist ein Wort, das durch Umstellen der Buchstaben eines anderen Wortes entsteht.
    /// Die Funktion ignoriert Groß- und Kleinschreibung, Leerzeichen und Sonderzeichen.
    /// </summary>
    /// <param name="s">Der erste String</param>
    /// <param name="t">Der zweite String</param>
    /// <returns>True, wenn die Strings Anagramme sind, sonst False</returns>
    public static bool IsAnagram(string s, string t)
    {
        // Sonderfall: Beide Strings sind null oder leer
        if (string.IsNullOrEmpty(s) && string.IsNullOrEmpty(t))
        {
            return true;
        }
        
        // Sonderfall: Nur einer der Strings ist null oder leer
        if (string.IsNullOrEmpty(s) || string.IsNullOrEmpty(t))
        {
            return false;
        }
        
        // Entferne alle Nicht-Buchstaben und konvertiere zu Kleinbuchstaben
        string cleanS = Regex.Replace(s.ToLower(), "[^a-z]", "");
        string cleanT = Regex.Replace(t.ToLower(), "[^a-z]", "");
        
        // Wenn die bereinigten Strings unterschiedliche Längen haben, können sie keine Anagramme sein
        if (cleanS.Length != cleanT.Length)
        {
            return false;
        }
        
        // Zähle die Häufigkeit jedes Buchstabens in beiden Strings
        Dictionary<char, int> charCount = new Dictionary<char, int>();
        
        // Zähle Buchstaben im ersten String
        foreach (char c in cleanS)
        {
            if (charCount.ContainsKey(c))
            {
                charCount[c]++;
            }
            else
            {
                charCount[c] = 1;
            }
        }
        
        // Subtrahiere Buchstaben im zweiten String
        foreach (char c in cleanT)
        {
            if (!charCount.ContainsKey(c))
            {
                // Wenn ein Buchstabe im zweiten String vorkommt, der nicht im ersten ist
                return false;
            }
            
            charCount[c]--;
            
            if (charCount[c] < 0)
            {
                // Wenn ein Buchstabe im zweiten String öfter vorkommt als im ersten
                return false;
            }
        }
        
        // Prüfe, ob alle Buchstaben vollständig abgeglichen wurden
        foreach (var count in charCount.Values)
        {
            if (count != 0)
            {
                return false;
            }
        }
        
        return true;
    }
}