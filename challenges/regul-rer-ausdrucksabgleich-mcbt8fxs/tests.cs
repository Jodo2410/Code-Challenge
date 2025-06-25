using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Regulärer Ausdrucksabgleich");
        Console.WriteLine(new string('=', 50));

        TestBasicMatching();
        TestDotMatching();
        TestStarMatching();
        TestComplexPatterns();
        TestEdgeCases();
        TestLargeInputs();

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

    private static void TestBasicMatching()
    {
        Test("Einfache Übereinstimmungen", () => {
            AssertEqual(true, RegexMatcher.IsMatch("abc", "abc"), "Exakte Übereinstimmung sollte funktionieren");
            AssertEqual(false, RegexMatcher.IsMatch("abc", "abcd"), "Unterschiedliche Längen ohne Sonderzeichen sollten nicht übereinstimmen");
            AssertEqual(false, RegexMatcher.IsMatch("abc", "abd"), "Unterschiedliche Zeichen sollten nicht übereinstimmen");
        });
    }

    private static void TestDotMatching()
    {
        Test("Punkt-Übereinstimmungen", () => {
            AssertEqual(true, RegexMatcher.IsMatch("abc", "a.c"), "'.' sollte mit einem beliebigen Zeichen übereinstimmen");
            AssertEqual(true, RegexMatcher.IsMatch("abc", "..."), "Mehrere '.' sollten mit mehreren beliebigen Zeichen übereinstimmen");
            AssertEqual(false, RegexMatcher.IsMatch("abc", "...."), "Zu viele '.' sollten nicht mit einem kürzeren String übereinstimmen");
        });
    }

    private static void TestStarMatching()
    {
        Test("Stern-Übereinstimmungen", () => {
            AssertEqual(true, RegexMatcher.IsMatch("aaa", "a*"), "'a*' sollte mit beliebig vielen 'a' übereinstimmen");
            AssertEqual(true, RegexMatcher.IsMatch("", "a*"), "'a*' sollte auch mit einem leeren String übereinstimmen");
            AssertEqual(true, RegexMatcher.IsMatch("aaa", "a*a"), "'a*a' sollte mit 'aaa' übereinstimmen");
            AssertEqual(false, RegexMatcher.IsMatch("aab", "a*c"), "'a*c' sollte nicht mit 'aab' übereinstimmen");
        });
    }

    private static void TestComplexPatterns()
    {
        Test("Komplexe Muster", () => {
            AssertEqual(true, RegexMatcher.IsMatch("aab", "c*a*b"), "'c*a*b' sollte mit 'aab' übereinstimmen");
            AssertEqual(true, RegexMatcher.IsMatch("ab", ".*"), "'.*' sollte mit jedem String übereinstimmen");
            AssertEqual(false, RegexMatcher.IsMatch("mississippi", "mis*is*p*."), "'mis*is*p*.' sollte nicht mit 'mississippi' übereinstimmen");
            AssertEqual(true, RegexMatcher.IsMatch("mississippi", "mis*is*ip*i"), "'mis*is*ip*i' sollte mit 'mississippi' übereinstimmen");
        });
    }

    private static void TestEdgeCases()
    {
        Test("Randfälle", () => {
            AssertEqual(true, RegexMatcher.IsMatch("", ""), "Leerer String sollte mit leerem Muster übereinstimmen");
            AssertEqual(false, RegexMatcher.IsMatch("a", ""), "Nicht-leerer String sollte nicht mit leerem Muster übereinstimmen");
            AssertEqual(true, RegexMatcher.IsMatch("", "a*"), "Leerer String sollte mit 'a*' übereinstimmen");
            AssertEqual(true, RegexMatcher.IsMatch("", ".*"), "Leerer String sollte mit '.*' übereinstimmen");
            AssertEqual(false, RegexMatcher.IsMatch("", "."), "Leerer String sollte nicht mit '.' übereinstimmen");
            AssertEqual(false, RegexMatcher.IsMatch(null, "a*"), "Null-String sollte nicht mit 'a*' übereinstimmen");
            AssertEqual(false, RegexMatcher.IsMatch("a", null), "String sollte nicht mit null-Muster übereinstimmen");
        });
    }

    private static void TestLargeInputs()
    {
        Test("Große Eingaben", () => {
            // Teste mit einem längeren String und Muster
            string longString = new string('a', 100);
            string longPattern = "a*" + new string('.', 50) + "a*";
            AssertEqual(true, RegexMatcher.IsMatch(longString, longPattern), "Lange Eingabe sollte korrekt verarbeitet werden");
            
            // Teste mit einem komplexen wiederholenden Muster
            string complexString = "ababababab";
            string complexPattern = "(a*b*)*";
            AssertEqual(true, RegexMatcher.IsMatch(complexString, "a*b*a*b*a*b*a*b*a*b*"), "Komplexes Muster sollte korrekt verarbeitet werden");
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