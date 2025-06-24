using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Array Sum Calculator Tests");
        Console.WriteLine("=" + new string('=', 40));

        // TotalSum Tests
        TestTotalSum();
        
        // SumEvenNumbers Tests
        TestSumEvenNumbers();
        
        // SumOddNumbers Tests
        TestSumOddNumbers();
        
        // SumInRange Tests
        TestSumInRange();

        // Ergebnis anzeigen
        Console.WriteLine("\n" + new string('=', 50));
        Console.WriteLine($"Ergebnis: {testsPassed}/{totalTests} Tests bestanden");
        
        if (testsPassed == totalTests)
        {
            Console.WriteLine("🎉 Alle Tests bestanden! Challenge erfolgreich gelöst!");
        }
        else
        {
            Console.WriteLine("❌ Einige Tests sind fehlgeschlagen. Überprüfe deine Implementierung.");
        }
    }

    private static void TestTotalSum()
    {
        Console.WriteLine("\n📊 TotalSum Tests:");
        
        // Test 1: Normale Arrays
        Test("TotalSum([1,2,3,4,5]) = 15", () => {
            int result = ArraySumCalculator.TotalSum(new int[] {1, 2, 3, 4, 5});
            AssertEqual(15, result);
        });
        
        // Test 2: Negative Zahlen
        Test("TotalSum([-1,-2,3,4]) = 4", () => {
            int result = ArraySumCalculator.TotalSum(new int[] {-1, -2, 3, 4});
            AssertEqual(4, result);
        });
        
        // Test 3: Leeres Array
        Test("TotalSum([]) = 0", () => {
            int result = ArraySumCalculator.TotalSum(new int[] {});
            AssertEqual(0, result);
        });
        
        // Test 4: Einzelnes Element
        Test("TotalSum([42]) = 42", () => {
            int result = ArraySumCalculator.TotalSum(new int[] {42});
            AssertEqual(42, result);
        });
    }

    private static void TestSumEvenNumbers()
    {
        Console.WriteLine("\n🔢 SumEvenNumbers Tests:");
        
        // Test 1: Gemischte gerade/ungerade Zahlen
        Test("SumEvenNumbers([1,2,3,4,5,6]) = 12", () => {
            int result = ArraySumCalculator.SumEvenNumbers(new int[] {1, 2, 3, 4, 5, 6});
            AssertEqual(12, result);
        });
        
        // Test 2: Nur ungerade Zahlen
        Test("SumEvenNumbers([1,3,5]) = 0", () => {
            int result = ArraySumCalculator.SumEvenNumbers(new int[] {1, 3, 5});
            AssertEqual(0, result);
        });
        
        // Test 3: Nur gerade Zahlen
        Test("SumEvenNumbers([2,4,6,8]) = 20", () => {
            int result = ArraySumCalculator.SumEvenNumbers(new int[] {2, 4, 6, 8});
            AssertEqual(20, result);
        });
        
        // Test 4: Negative gerade Zahlen
        Test("SumEvenNumbers([-2,1,-4,3]) = -6", () => {
            int result = ArraySumCalculator.SumEvenNumbers(new int[] {-2, 1, -4, 3});
            AssertEqual(-6, result);
        });
    }

    private static void TestSumOddNumbers()
    {
        Console.WriteLine("\n🔢 SumOddNumbers Tests:");
        
        // Test 1: Gemischte gerade/ungerade Zahlen
        Test("SumOddNumbers([1,2,3,4,5,6]) = 9", () => {
            int result = ArraySumCalculator.SumOddNumbers(new int[] {1, 2, 3, 4, 5, 6});
            AssertEqual(9, result);
        });
        
        // Test 2: Nur gerade Zahlen
        Test("SumOddNumbers([2,4,6]) = 0", () => {
            int result = ArraySumCalculator.SumOddNumbers(new int[] {2, 4, 6});
            AssertEqual(0, result);
        });
        
        // Test 3: Nur ungerade Zahlen
        Test("SumOddNumbers([1,3,5,7]) = 16", () => {
            int result = ArraySumCalculator.SumOddNumbers(new int[] {1, 3, 5, 7});
            AssertEqual(16, result);
        });
        
        // Test 4: Negative ungerade Zahlen
        Test("SumOddNumbers([-1,2,-3,4]) = -4", () => {
            int result = ArraySumCalculator.SumOddNumbers(new int[] {-1, 2, -3, 4});
            AssertEqual(-4, result);
        });
    }

    private static void TestSumInRange()
    {
        Console.WriteLine("\n📏 SumInRange Tests:");
        
        // Test 1: Normaler Bereich
        Test("SumInRange([1,2,3,4,5,6], 2, 5) = 14", () => {
            int result = ArraySumCalculator.SumInRange(new int[] {1, 2, 3, 4, 5, 6}, 2, 5);
            AssertEqual(14, result);
        });
        
        // Test 2: Keine Zahlen im Bereich
        Test("SumInRange([1,2,3], 10, 20) = 0", () => {
            int result = ArraySumCalculator.SumInRange(new int[] {1, 2, 3}, 10, 20);
            AssertEqual(0, result);
        });
        
        // Test 3: Alle Zahlen im Bereich
        Test("SumInRange([3,4,5], 1, 10) = 12", () => {
            int result = ArraySumCalculator.SumInRange(new int[] {3, 4, 5}, 1, 10);
            AssertEqual(12, result);
        });
        
        // Test 4: Negative Bereiche
        Test("SumInRange([-5,-2,0,3,8], -3, 2) = 1", () => {
            int result = ArraySumCalculator.SumInRange(new int[] {-5, -2, 0, 3, 8}, -3, 2);
            AssertEqual(1, result); // -2 + 0 + 3 = 1 (3 ist nicht im Bereich -3 bis 2)
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
            Console.WriteLine($"  ❌ {description}");
            Console.WriteLine($"     Fehler: {ex.Message}");
        }
    }

    private static void AssertEqual(int expected, int actual)
    {
        if (expected != actual)
        {
            throw new Exception($"Erwartet: {expected}, Erhalten: {actual}");
        }
    }
}