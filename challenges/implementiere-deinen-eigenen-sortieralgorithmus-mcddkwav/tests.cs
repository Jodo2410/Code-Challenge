using System;
using System.Diagnostics;

public static class Tests {
    public static void RunAllTests() {
        Test(new int[] {5, 3, 8, 1, 2}, new int[] {1, 2, 3, 5, 8}, "Test 1 - Kleine Liste");
        Test(new int[] {10, -3, 0, 4, 7}, new int[] {-3, 0, 4, 7, 10}, "Test 2 - Mit negativen Zahlen");
        TestPerformance(100000, 200, "Performance Test - 100.000 Elemente unter 200ms");
    }

    private static void Test(int[] input, int[] expected, string testName) {
        int[] copy = (int[])input.Clone();
        Solution.CustomSort(copy);
        bool success = copy.Length == expected.Length;

        for (int i = 0; i < expected.Length && success; i++) {
            if (copy[i] != expected[i]) success = false;
        }

        Console.WriteLine(success
            ? $"[✔] {testName} bestanden"
            : $"[✘] {testName} fehlgeschlagen: Ergebnis stimmt nicht.");
    }

    private static void TestPerformance(int size, int maxMilliseconds, string testName) {
        int[] input = new int[size];
        Random rnd = new Random();
        for (int i = 0; i < size; i++) input[i] = rnd.Next();

        Stopwatch sw = Stopwatch.StartNew();
        Solution.CustomSort((int[])input.Clone());
        sw.Stop();

        if (sw.ElapsedMilliseconds <= maxMilliseconds) {
            Console.WriteLine($"[✔] {testName} in {sw.ElapsedMilliseconds}ms");
        } else {
            Console.WriteLine($"[✘] {testName} zu langsam: {sw.ElapsedMilliseconds}ms > {maxMilliseconds}ms");
        }
    }
}