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
    public static bool HasPairWithSum(int[] nums, int target) {
        HashSet<int> seen = new HashSet<int>();
        foreach (int num in nums) {
            int complement = target - num;
            if (seen.Contains(complement)) {
                return true;
            }
            seen.Add(num);
        }
        return false;
    }
}