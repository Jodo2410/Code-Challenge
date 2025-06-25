using System;

public static class Tests {
    public static void RunAllTests() {
        int passed = 0;
        int total = 5;

        if (Solution.LengthOfLongestSubstring("abcabcbb") == 3) {
            Console.WriteLine("Test 1 bestanden ✓ ("abc")");
            passed++;
        } else {
            Console.WriteLine("Test 1 fehlgeschlagen ✗");
        }

        if (Solution.LengthOfLongestSubstring("bbbbb") == 1) {
            Console.WriteLine("Test 2 bestanden ✓ ("b")");
            passed++;
        } else {
            Console.WriteLine("Test 2 fehlgeschlagen ✗");
        }

        if (Solution.LengthOfLongestSubstring("pwwkew") == 3) {
            Console.WriteLine("Test 3 bestanden ✓ ("wke")");
            passed++;
        } else {
            Console.WriteLine("Test 3 fehlgeschlagen ✗");
        }

        if (Solution.LengthOfLongestSubstring("") == 0) {
            Console.WriteLine("Test 4 bestanden ✓ (leer)");
            passed++;
        } else {
            Console.WriteLine("Test 4 fehlgeschlagen ✗");
        }

        if (Solution.LengthOfLongestSubstring("dvdf") == 3) {
            Console.WriteLine("Test 5 bestanden ✓ ("vdf")");
            passed++;
        } else {
            Console.WriteLine("Test 5 fehlgeschlagen ✗");
        }

        Console.WriteLine($"{passed}/{total} Tests bestanden.");
    }
}