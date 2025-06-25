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
    public static int CountEvenNumbers(int[] numbers) {
        int count = 0;
        foreach (int num in numbers) {
            if (num % 2 == 0) {
                count++;
            }
        }
        return count;
    }

    public static void Main() {
        int[] test1 = { 1, 2, 3, 4, 5, 6 };
        int[] test2 = { 7, 9, 13 };
        int[] test3 = { 2, 4, 6, 8 };

        Console.WriteLine(CountEvenNumbers(test1)); // Ausgabe: 3
        Console.WriteLine(CountEvenNumbers(test2)); // Ausgabe: 0
        Console.WriteLine(CountEvenNumbers(test3)); // Ausgabe: 4
    }
}