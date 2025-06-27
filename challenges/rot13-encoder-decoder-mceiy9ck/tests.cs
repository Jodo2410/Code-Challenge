using System;

public static class Rot13Tests
{
    public static void RunAllTests()
    {
        int passed = 0;
        int total = 4;
        Console.WriteLine("=== ROT13 Tests ===\n");

        if (Test("Test 1", Rot13Cipher.Rot13("Hello"), "Uryyb")) passed++;
        if (Test("Test 2", Rot13Cipher.Rot13("Uryyb"), "Hello")) passed++;
        if (Test("Test 3", Rot13Cipher.Rot13("1234"), "1234")) passed++;
        if (Test("Test 4 - Double Encoding", Rot13Cipher.Rot13(Rot13Cipher.Rot13("OpenAI")), "OpenAI")) passed++;

        Console.WriteLine($"\n{passed}/{total} Tests bestanden");
    }

    private static bool Test(string name, string actual, string expected)
    {
        bool success = actual == expected;
        Console.WriteLine(success
            ? $"[PASS] {name}"
            : $"[FAIL] {name} | Erwartet: '{expected}', Erhalten: '{actual}'");
        return success;
    }
}