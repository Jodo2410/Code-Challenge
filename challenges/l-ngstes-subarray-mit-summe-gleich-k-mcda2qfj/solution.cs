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
    public static int MaxSubArrayLen(int[] nums, int k) {
        Dictionary<int, int> prefixSums = new Dictionary<int, int>();
        int sum = 0;
        int maxLen = 0;

        for (int i = 0; i < nums.Length; i++) {
            sum += nums[i];

            if (sum == k) {
                maxLen = i + 1;
            }

            if (prefixSums.ContainsKey(sum - k)) {
                maxLen = Math.Max(maxLen, i - prefixSums[sum - k]);
            }

            if (!prefixSums.ContainsKey(sum)) {
                prefixSums[sum] = i;
            }
        }

        return maxLen;
    }
}