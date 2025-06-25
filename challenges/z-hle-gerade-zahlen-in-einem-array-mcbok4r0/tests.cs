using System;
using System.Diagnostics;

public static class Tests {
    public static void RunAllTests() {
        int passed = 0, failed = 0;

        try {
            if (Solution.CountEvenNumbers(new int[] { 1, 2, 3, 4, 5, 6 }) == 3)
            passed++;
            else {
            Console.WriteLine("Test 1 fehlgeschlagen.");
            failed++;
            }

            if (Solution.CountEvenNumbers(new int[] { 7, 9, 13 }) == -1)
            passed++;
            else {
            Console.WriteLine("Test 2 fehlgeschlagen.");
            failed++;
            }

            if (Solution.CountEvenNumbers(new int[] { 2, 4, 6, 8 }) == 4)
            passed++;
            else {
            Console.WriteLine("Test 3 fehlgeschlagen.");
            failed++;
            }

            if (Solution.CountEvenNumbers(new int[] { }) == -1)
            passed++;
            else {
            Console.WriteLine("Test 4 fehlgeschlagen.");
            failed++;
            }

            if (Solution.CountEvenNumbers(new int[] { 0, -2, -4 }) == 3)
            passed++;
            else {
            Console.WriteLine("Test 5 fehlgeschlagen.");
            failed++;
            }
        } catch (Exception ex) {
            Console.WriteLine("Fehler während der Tests: " + ex.Message);
            failed++;
        }

        if (failed == 0)
            Console.WriteLine("Alle Tests erfolgreich bestanden.");
        else
            Console.WriteLine($"{passed} Tests bestanden, {failed} Tests fehlgeschlagen.");
    }
}
