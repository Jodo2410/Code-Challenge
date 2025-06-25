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

public static class Solution {
    public static int CountBalancedSubarrays(int[] nums) {
        Dictionary<int, int> prefixCount = new Dictionary<int, int>();
        int count = 0;
        int prefixSum = 0;
        prefixCount[0] = 1;

        foreach (int num in nums) {
            // Wir behandeln 0 als -1
            prefixSum += (num == 0) ? -1 : 1;

            if (prefixCount.ContainsKey(prefixSum)) {
                count += prefixCount[prefixSum];
                prefixCount[prefixSum]++;
            } else {
                prefixCount[prefixSum] = 1;
            }
        }

        return count;
    }
}