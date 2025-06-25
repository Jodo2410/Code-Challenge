using System;

public static class Tests {
    public static void RunTests() {
        int passed = 0;
        int total = 5;

        if (Solution.HasPairWithSum(new int[] { 2, 7, 11, 15 }, 9)) {
            Console.WriteLine("Test 1 bestanden ✓");
            passed++;
        } else {
            Console.WriteLine("Test 1 fehlgeschlagen ✗");
        }

        if (!Solution.HasPairWithSum(new int[] { 1, 2, 3, 4 }, 8)) {
            Console.WriteLine("Test 2 bestanden ✓");
            passed++;
        } else {
            Console.WriteLine("Test 2 fehlgeschlagen ✗");
        }

        if (Solution.HasPairWithSum(new int[] { -1, 5, 3, 6 }, 4)) {
            Console.WriteLine("Test 3 bestanden ✓");
            passed++;
        } else {
            Console.WriteLine("Test 3 fehlgeschlagen ✗");
        }

        if (Solution.HasPairWithSum(new int[] { 0, 4, 3, 0 }, 0)) {
            Console.WriteLine("Test 4 bestanden ✓");
            passed++;
        } else {
            Console.WriteLine("Test 4 fehlgeschlagen ✗");
        }

        if (!Solution.HasPairWithSum(new int[] { 1 }, 1)) {
            Console.WriteLine("Test 5 bestanden ✓");
            passed++;
        } else {
            Console.WriteLine("Test 5 fehlgeschlagen ✗");
        }

        Console.WriteLine($"{passed}/{total} Tests bestanden");
    }
}