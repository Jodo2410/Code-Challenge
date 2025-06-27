using System;

public static class Tests
{
    public static void RunAllTests()
    {
        int passed = 0;
        int total = 3;
        Console.WriteLine("=== Morsecode Tests ===\n");

        if (Test("ToMorse 'HELLO'", MorseCodeConverter.ToMorse("HELLO"), ".... . .-.. .-.. ---")) passed++;
        if (Test("ToMorse 'HELLO WORLD'", MorseCodeConverter.ToMorse("HELLO WORLD"), ".... . .-.. .-.. --- / .-- --- .-. .-.. -..")) passed++;
        if (Test("FromMorse back to 'HELLO'", MorseCodeConverter.FromMorse(".... . .-.. .-.. ---"), "HELLO")) passed++;

        Console.WriteLine($"\n{passed}/{total} Tests bestanden");
    }

    private static bool Test(string name, string actual, string expected)
    {
        bool success = actual == expected;
        Console.WriteLine(success ? $"[PASS] {name}" : $"[FAIL] {name} | Erwartet: '{expected}', Erhalten: '{actual}'");
        return success;
    }
}
