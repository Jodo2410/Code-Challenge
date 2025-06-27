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

public static class ArrayUtils
{
    public static int[] RemoveDuplicates(int[] input)
    {
        HashSet<int> seen = new HashSet<int>();
        List<int> result = new List<int>();

        foreach (int number in input)
        {
            if (!seen.Contains(number))
            {
                seen.Add(number);
                result.Add(number);
            }
        }

        return result.ToArray();
    }
}