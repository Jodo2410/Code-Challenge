using System;
using System.Diagnostics;

public static class Tests {
    public static void RunAllTests() {
        Debug.Assert(Solution.CountEvenNumbers(new int[] { 1, 2, 3, 4, 5, 6 }) == 3);
        Debug.Assert(Solution.CountEvenNumbers(new int[] { 7, 9, 13 }) == 0);
        Debug.Assert(Solution.CountEvenNumbers(new int[] { 2, 4, 6, 8 }) == 4);
        Debug.Assert(Solution.CountEvenNumbers(new int[] { }) == 0);
        Debug.Assert(Solution.CountEvenNumbers(new int[] { 0, -2, -4 }) == 3);

        Console.WriteLine("Alle Tests erfolgreich bestanden.");
    }
}
