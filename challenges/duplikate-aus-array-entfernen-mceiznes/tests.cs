using System;

public static class ArrayUtilsTests
{
    public static void RunAllTests()
    {
        int passed = 0;
        int total = 3;
        Console.WriteLine("=== Duplikate Entfernen Tests ===\n");

        if (Test("Test 1", ArrayUtils.RemoveDuplicates(new int[] { 1, 2, 2, 3, 1 }), new int[] { 1, 2, 3 })) passed++;
        if (Test("Test 2", ArrayUtils.RemoveDuplicates(new int[] { 4, 5, 4, 5, 4 }), new int[] { 4, 5 })) passed++;
        if (Test("Test 3", ArrayUtils.RemoveDuplicates(new int[] { 10 }), new int[] { 10 })) passed++;

        Console.WriteLine($"\n{passed}/{total} Tests bestanden");
    }

    private static bool Test(string name, int[] actual, int[] expected)
    {
        bool success = actual.Length == expected.Length;
        for (int i = 0; i < actual.Length && success; i++)
        {
            if (actual[i] != expected[i]) success = false;
        }

        Console.WriteLine(success ? $"[PASS] {name}" : $"[FAIL] {name} | Erwartet: [{string.Join(", ", expected)}], Erhalten: [{string.Join(", ", actual)}]");
        return success;
    }
}