using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Anagramm-Prüfer");
        Console.WriteLine(new string('=', 50));

        TestBasicAnagrams();
        TestNonAnagrams();
        TestCaseSensitivity();
        TestIgnoreNonLetters();
        TestEmptyAndNullStrings();
        TestUnicodeCharacters();
        TestLongStrings();

        Console.WriteLine($"\n{testsPassed}/{totalTests} Tests bestanden");
        
        if (testsPassed == totalTests)
        {
            Console.WriteLine("🎉 Alle Tests bestanden!");
        }
        else
        {
            Console.WriteLine("❌ Einige Tests fehlgeschlagen.");
            Environment.Exit(1);
        }
    }

    private static void TestBasicAnagrams()
    {
        Test("Einfache Anagramme", () => {
            AssertEqual(true, AnagramChecker.IsAnagram("listen", "silent"), "'listen' und 'silent' sollten Anagramme sein");
            AssertEqual(true, AnagramChecker.IsAnagram("anagram", "nagaram"), "'anagram' und 'nagaram' sollten Anagramme sein");
            AssertEqual(true, AnagramChecker.IsAnagram("triangle", "integral"), "'triangle' und 'integral' sollten Anagramme sein");
        });
    }

    private static void TestNonAnagrams()
    {
        Test("Keine Anagramme", () => {
            AssertEqual(false, AnagramChecker.IsAnagram("hello", "world"), "'hello' und 'world' sollten keine Anagramme sein");
            AssertEqual(false, AnagramChecker.IsAnagram("rat", "car"), "'rat' und 'car' sollten keine Anagramme sein");
            AssertEqual(false, AnagramChecker.IsAnagram("abc", "abcd"), "'abc' und 'abcd' sollten keine Anagramme sein (unterschiedliche Länge)");
        });
    }

    private static void TestCaseSensitivity()
    {
        Test("Groß- und Kleinschreibung ignorieren", () => {
            AssertEqual(true, AnagramChecker.IsAnagram("Listen", "Silent"), "'Listen' und 'Silent' sollten Anagramme sein (Groß-/Kleinschreibung ignorieren)");
            AssertEqual(true, AnagramChecker.IsAnagram("ANAGRAM", "nagaram"), "'ANAGRAM' und 'nagaram' sollten Anagramme sein (Groß-/Kleinschreibung ignorieren)");
        });
    }

    private static void TestIgnoreNonLetters()
    {
        Test("Leerzeichen und Sonderzeichen ignorieren", () => {
            AssertEqual(true, AnagramChecker.IsAnagram("Debit Card", "Bad Credit"), "'Debit Card' und 'Bad Credit' sollten Anagramme sein (Leerzeichen ignorieren)");
            AssertEqual(true, AnagramChecker.IsAnagram("astronomer", "moon starer"), "'astronomer' und 'moon starer' sollten Anagramme sein (Leerzeichen ignorieren)");
            AssertEqual(true, AnagramChecker.IsAnagram("The Morse Code", "Here comes dots"), "'The Morse Code' und 'Here comes dots' sollten Anagramme sein");
            AssertEqual(true, AnagramChecker.IsAnagram("Dormitory", "Dirty Room!"), "'Dormitory' und 'Dirty Room!' sollten Anagramme sein (Sonderzeichen ignorieren)");
        });
    }

    private static void TestEmptyAndNullStrings()
    {
        Test("Leere und null Strings", () => {
            AssertEqual(true, AnagramChecker.IsAnagram("", ""), "Zwei leere Strings sollten Anagramme sein");
            AssertEqual(true, AnagramChecker.IsAnagram(null, null), "Zwei null-Strings sollten Anagramme sein");
            AssertEqual(false, AnagramChecker.IsAnagram("", "a"), "Ein leerer String und ein nicht-leerer String sollten keine Anagramme sein");
            AssertEqual(false, AnagramChecker.IsAnagram(null, "a"), "Ein null-String und ein nicht-null-String sollten keine Anagramme sein");
        });
    }

    private static void TestUnicodeCharacters()
    {
        Test("Unicode-Zeichen", () => {
            // Nur Buchstaben werden berücksichtigt, daher sollten folgende Beispiele Anagramme sein
            AssertEqual(true, AnagramChecker.IsAnagram("abc123", "cba456"), "'abc123' und 'cba456' sollten Anagramme sein (Zahlen ignorieren)");
            AssertEqual(true, AnagramChecker.IsAnagram("abc!@#", "cba$%^"), "'abc!@#' und 'cba$%^' sollten Anagramme sein (Sonderzeichen ignorieren)");
        });
    }

    private static void TestLongStrings()
    {
        Test("Lange Strings", () => {
            string longString1 = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
            string longString2 = "zyxwvutsrqponmlkjihgfedcbazyxwvutsrqponmlkjihgfedcba";
            AssertEqual(true, AnagramChecker.IsAnagram(longString1, longString2), "Lange Strings sollten korrekt als Anagramme erkannt werden");
        });
    }

    private static void Test(string description, Action testAction)
    {
        totalTests++;
        try
        {
            testAction();
            Console.WriteLine($"  ✅ {description}");
            testsPassed++;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"  ❌ {description}: {ex.Message}");
        }
    }

    private static void AssertEqual<T>(T expected, T actual, string message = null)
    {
        if (!Equals(expected, actual))
        {
            throw new Exception($"{message ?? ""}\nErwartet: {expected}, Erhalten: {actual}");
        }
    }
}