/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public static class Solution {
    public static int CountVowels(string input) {
        int count = 0;
        string vowels = "aeiou";
        foreach (char c in input.ToLower()) {
            if (vowels.Contains(c)) {
                count++;
            }
        }
        return count;
    }

    public static void Main() {
        Console.WriteLine(CountVowels("Hallo Welt")); // Ausgabe: 3
        Console.WriteLine(CountVowels("Programmieren macht Spaß")); // Ausgabe: 6
        Console.WriteLine(CountVowels("XYZ")); // Ausgabe: 0
    }
}