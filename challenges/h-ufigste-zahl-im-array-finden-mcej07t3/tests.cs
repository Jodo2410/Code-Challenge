using System;

public static class Tests
{
    public static void RunAllTests()
    {
        int passed = 0;
        int total = 3;
        Console.WriteLine("=== Häufigste Zahl Tests ===\n");

        if (Test("Test 1", FrequencyAnalyzer.MostFrequent(new int[] {1, 2, 2, 3, 1, 2}), 2)) passed++;
        if (Test("Test 2", FrequencyAnalyzer.MostFrequent(new int[] {4, 4, 5, 5, 5, 6}), 5)) passed++;
        if (Test("Test 3", FrequencyAnalyzer.MostFrequent(new int[] {9}), 9)) passed++;

        Console.WriteLine($"\n{passed}/{total} Tests bestanden");
    }

    private static bool Test(string name, int actual, int expected)
    {
        bool success = actual == expected;
        Console.WriteLine(success ? $"[PASS] {name}" : $"[FAIL] {name} | Erwartet: {expected}, Erhalten: {actual}");
        return success;
    }
}
