using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Minimale Editierdistanz");
        Console.WriteLine(new string('=', 50));

        TestBasicCases();
        TestEdgeCases();
        TestEmptyStrings();
        TestSameStrings();
        TestLargeStrings();
        TestNullInput();

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

    private static void TestBasicCases()
    {
        Test("Beispiele aus der Aufgabenstellung", () => {
            int distance = EditDistance.MinDistance("horse", "ros");
            AssertEqual(3, distance, "Die Editierdistanz zwischen 'horse' und 'ros' sollte 3 sein");
            
            distance = EditDistance.MinDistance("intention", "execution");
            AssertEqual(5, distance, "Die Editierdistanz zwischen 'intention' und 'execution' sollte 5 sein");
        });
    }

    private static void TestEdgeCases()
    {
        Test("Einfache Operationen", () => {
            // Einzelne Einfügung
            int distance = EditDistance.MinDistance("abc", "abcd");
            AssertEqual(1, distance, "Eine Einfügung sollte die Distanz 1 haben");
            
            // Einzelne Löschung
            distance = EditDistance.MinDistance("abcd", "abc");
            AssertEqual(1, distance, "Eine Löschung sollte die Distanz 1 haben");
            
            // Einzelne Ersetzung
            distance = EditDistance.MinDistance("abc", "abd");
            AssertEqual(1, distance, "Eine Ersetzung sollte die Distanz 1 haben");
        });
    }

    private static void TestEmptyStrings()
    {
        Test("Leere Strings", () => {
            // Leerer String zu nicht-leerem String
            int distance = EditDistance.MinDistance("", "abc");
            AssertEqual(3, distance, "Die Distanz von leerem String zu 'abc' sollte 3 sein (3 Einfügungen)");
            
            // Nicht-leerer String zu leerem String
            distance = EditDistance.MinDistance("abc", "");
            AssertEqual(3, distance, "Die Distanz von 'abc' zu leerem String sollte 3 sein (3 Löschungen)");
            
            // Beide Strings leer
            distance = EditDistance.MinDistance("", "");
            AssertEqual(0, distance, "Die Distanz zwischen zwei leeren Strings sollte 0 sein");
        });
    }

    private static void TestSameStrings()
    {
        Test("Identische Strings", () => {
            int distance = EditDistance.MinDistance("abc", "abc");
            AssertEqual(0, distance, "Die Distanz zwischen identischen Strings sollte 0 sein");
            
            distance = EditDistance.MinDistance("abcdefghij", "abcdefghij");
            AssertEqual(0, distance, "Die Distanz zwischen identischen längeren Strings sollte 0 sein");
        });
    }

    private static void TestLargeStrings()
    {
        Test("Größere Strings", () => {
            // Lange Strings mit Unterschieden am Anfang, in der Mitte und am Ende
            int distance = EditDistance.MinDistance("abcdefghijklmno", "pqcdefghijklz");
            AssertEqual(5, distance, "Die Distanz sollte korrekt für längere Strings berechnet werden");
            
            // Völlig unterschiedliche Strings
            distance = EditDistance.MinDistance("abcdefg", "hijklmn");
            AssertEqual(7, distance, "Die Distanz sollte korrekt für völlig unterschiedliche Strings berechnet werden");
        });
    }

    private static void TestNullInput()
    {
        Test("Null-Eingaben", () => {
            int distance = EditDistance.MinDistance(null, "abc");
            AssertEqual(3, distance, "Die Distanz von null zu 'abc' sollte 3 sein (null wird als leerer String behandelt)");
            
            distance = EditDistance.MinDistance("abc", null);
            AssertEqual(3, distance, "Die Distanz von 'abc' zu null sollte 3 sein (null wird als leerer String behandelt)");
            
            distance = EditDistance.MinDistance(null, null);
            AssertEqual(0, distance, "Die Distanz zwischen zwei null-Strings sollte 0 sein (beide werden als leere Strings behandelt)");
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