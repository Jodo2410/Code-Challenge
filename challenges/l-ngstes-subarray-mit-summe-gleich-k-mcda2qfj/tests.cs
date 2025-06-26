using System;

public static class Tests {
    public static void RunAllTests() {
        Test(new int[] {1, -1, 5, -2, 3}, 3, 4, "Test 1");
        Test(new int[] {-2, -1, 2, 1}, 1, 2, "Test 2");
        Test(new int[] {1, 2, 3}, 6, 3, "Test 3");
        Test(new int[] {1, 2, 3}, 7, 0, "Test 4 - Keine Übereinstimmung");
        Test(new int[] {1, -1, 1, -1, 1, -1}, 0, 6, "Test 5 - Ganze Länge null");
    }

    private static void Test(int[] nums, int k, int expected, string testName) {
        int result = Solution.MaxSubArrayLen(nums, k);
        if (result == expected) {
            Console.WriteLine($"[✔] {testName} bestanden");
        } else {
            Console.WriteLine($"[✘] {testName} fehlgeschlagen: erwartet {expected}, erhalten {result}");
        }
    }
}