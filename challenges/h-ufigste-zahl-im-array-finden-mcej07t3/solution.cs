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

public static class FrequencyAnalyzer
{
    public static int MostFrequent(int[] input)
    {
        Dictionary<int, int> freq = new Dictionary<int, int>();
        int maxCount = 0;
        int result = input[0];

        foreach (int num in input)
        {
            if (!freq.ContainsKey(num)) freq[num] = 0;
            freq[num]++;

            if (freq[num] > maxCount || (freq[num] == maxCount && Array.IndexOf(input, num) < Array.IndexOf(input, result)))
            {
                maxCount = freq[num];
                result = num;
            }
        }

        return result;
    }
}