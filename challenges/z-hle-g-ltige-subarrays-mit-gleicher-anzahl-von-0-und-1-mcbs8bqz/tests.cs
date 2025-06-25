using System;

public static class Tests {
    public static void RunAllTests() {
        Test(new int[] {0, 1}, 1, "Test 1");
        Test(new int[] {0, 1, 0}, 2, "Test 2");
        Test(new int[] {0, 0, 1, 1, 0}, 4, "Test 3");
        Test(new int[] {0, 1, 1, 0, 1, 0, 0}, 9, "Test 4");
        Test(new int[] {}, 0, "Test 5 - Leeres Array");
        Test(new int[] {1, 1, 1, 0, 0, 0, 1}, 6, "Test 6");
    }

    private static void Test(int[] input, int expected, string testName) {
        int result = Solution.CountBalancedSubarrays(input);
        if (result == expected) {
            Console.WriteLine($"[✔] {testName} bestanden");
        } else {
            Console.WriteLine($"[✘] {testName} fehlgeschlagen: erwartet {expected}, erhalten {result}");
        }
    }
}