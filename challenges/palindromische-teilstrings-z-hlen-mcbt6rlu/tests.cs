using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Palindromische Teilstrings zählen");
        Console.WriteLine(new string('=', 50));

        TestEmptyString();
        TestSingleCharacter();
        TestAllSameCharacters();
        TestNoRepeatingCharacters();
        TestMixedCharacters();
        TestLongerPalindromes();
        TestLargeInput();

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

    private static void TestEmptyString()
    {
        Test("Leerer String", () => {
            int result = PalindromeCounter.CountPalindromicSubstrings("");
            AssertEqual(0, result, "Ein leerer String sollte 0 palindromische Teilstrings haben");
            
            result = PalindromeCounter.CountPalindromicSubstrings(null);
            AssertEqual(0, result, "Ein null-String sollte 0 palindromische Teilstrings haben");
        });
    }

    private static void TestSingleCharacter()
    {
        Test("Einzelnes Zeichen", () => {
            int result = PalindromeCounter.CountPalindromicSubstrings("a");
            AssertEqual(1, result, "Ein einzelnes Zeichen sollte 1 palindromischen Teilstring haben");
        });
    }

    private static void TestAllSameCharacters()
    {
        Test("Alle gleichen Zeichen", () => {
            int result = PalindromeCounter.CountPalindromicSubstrings("aaa");
            AssertEqual(6, result, "'aaa' sollte 6 palindromische Teilstrings haben");
            
            result = PalindromeCounter.CountPalindromicSubstrings("aaaa");
            AssertEqual(10, result, "'aaaa' sollte 10 palindromische Teilstrings haben");
        });
    }

    private static void TestNoRepeatingCharacters()
    {
        Test("Keine wiederholenden Zeichen", () => {
            int result = PalindromeCounter.CountPalindromicSubstrings("abc");
            AssertEqual(3, result, "'abc' sollte 3 palindromische Teilstrings haben (a, b, c)");
            
            result = PalindromeCounter.CountPalindromicSubstrings("abcdef");
            AssertEqual(6, result, "'abcdef' sollte 6 palindromische Teilstrings haben (a, b, c, d, e, f)");
        });
    }

    private static void TestMixedCharacters()
    {
        Test("Gemischte Zeichen", () => {
            int result = PalindromeCounter.CountPalindromicSubstrings("aabaa");
            AssertEqual(9, result, "'aabaa' sollte 9 palindromische Teilstrings haben");
            
            result = PalindromeCounter.CountPalindromicSubstrings("aabb");
            AssertEqual(6, result, "'aabb' sollte 6 palindromische Teilstrings haben (a, a, b, b, aa, bb)");
        });
    }

    private static void TestLongerPalindromes()
    {
        Test("Längere Palindrome", () => {
            int result = PalindromeCounter.CountPalindromicSubstrings("racecar");
            AssertEqual(10, result, "'racecar' sollte 10 palindromische Teilstrings haben");
            
            result = PalindromeCounter.CountPalindromicSubstrings("abccba");
            AssertEqual(9, result, "'abccba' sollte 9 palindromische Teilstrings haben");
        });
    }

    private static void TestLargeInput()
    {
        Test("Große Eingabe", () => {
            // Erstelle einen String mit 1000 'a's
            string largeString = new string('a', 1000);
            int result = PalindromeCounter.CountPalindromicSubstrings(largeString);
            
            // Für n 'a's haben wir n*(n+1)/2 Palindrome (Summe von 1 bis n)
            int expected = (1000 * 1001) / 2;
            AssertEqual(expected, result, $"Ein String mit 1000 gleichen Zeichen sollte {expected} palindromische Teilstrings haben");
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