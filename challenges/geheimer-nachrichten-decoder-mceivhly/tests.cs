using System;

public static class CaesarDecoderTests
{
    public static void RunAllTests()
    {
        int passed = 0;
        int total = 4;

        Console.WriteLine("=== Caesar Decoder Tests ===\n");

        if (Test("Test 1", CaesarDecoder.DecodeMessage("Khoor Zruog", -3), "Hello World")) passed++;
        if (Test("Test 2", CaesarDecoder.DecodeMessage("Fdhvdu Flskhu", -3), "Caesar Cipher")) passed++;
        if (Test("Test 3", CaesarDecoder.DecodeMessage("Ebiil Tloia", 23), "Hello World")) passed++;
        if (Test("Test 4 (No Shift)", CaesarDecoder.DecodeMessage("Unverändert", 0), "Unverändert")) passed++;

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