using System;

public static class Tests {
    public static void RunAllTests() {
        int passed = 0;
        int total = 4;

        if (Solution.FindMajorityElement(new int[] {3, 2, 3}) == 3) {
            Console.WriteLine("Test 1 bestanden ✓ (Erwartet: 3)");
            passed++;
        } else {
            Console.WriteLine("Test 1 fehlgeschlagen ✗");
        }

        if (Solution.FindMajorityElement(new int[] {2, 2, 1, 1, 1, 2, 2}) == 2) {
            Console.WriteLine("Test 2 bestanden ✓ (Erwartet: 2)");
            passed++;
        } else {
            Console.WriteLine("Test 2 fehlgeschlagen ✗");
        }

        if (Solution.FindMajorityElement(new int[] {1, 1, 1, 1, 2, 3, 4}) == 1) {
            Console.WriteLine("Test 3 bestanden ✓ (Erwartet: 1)");
            passed++;
        } else {
            Console.WriteLine("Test 3 fehlgeschlagen ✗");
        }

        if (Solution.FindMajorityElement(new int[] {9, 9, 9, 1, 2, 9, 9}) == 9) {
            Console.WriteLine("Test 4 bestanden ✓ (Erwartet: 9)");
            passed++;
        } else {
            Console.WriteLine("Test 4 fehlgeschlagen ✗");
        }

        Console.WriteLine($"{passed}/{total} Tests bestanden.");
    }
}