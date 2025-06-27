using System;

public static class HtmlCleanerTests
{
    public static void RunAllTests()
    {
        int passed = 0;
        int total = 4;
        Console.WriteLine("=== HTML-Tag-Remover Tests ===\n");

        if (Test("Test 1", HtmlCleaner.RemoveHtmlTags("<p>Hello</p>"), "Hello")) passed++;
        if (Test("Test 2", HtmlCleaner.RemoveHtmlTags("<div><b>Text</b> mit <i>Tags</i></div>"), "Text mit Tags")) passed++;
        if (Test("Test 3", HtmlCleaner.RemoveHtmlTags("<a href='link'>Klick mich</a>"), "Klick mich")) passed++;
        if (Test("Empty", HtmlCleaner.RemoveHtmlTags(""), "")) passed++;

        Console.WriteLine($"\n{passed}/{total} Tests bestanden");
    }

    private static bool Test(string name, string actual, string expected)
    {
        bool success = actual == expected;
        Console.WriteLine(success ? $"[PASS] {name}" : $"[FAIL] {name} | Erwartet: '{expected}', Erhalten: '{actual}'");
        return success;
    }
}