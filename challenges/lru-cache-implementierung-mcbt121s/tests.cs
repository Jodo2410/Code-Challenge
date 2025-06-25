using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: LRU Cache Implementierung");
        Console.WriteLine(new string('=', 50));

        TestBasicOperations();
        TestCapacityLimit();
        TestUpdateExistingKey();
        TestLRUEvictionPolicy();
        TestEmptyCache();
        TestSingleItemCache();
        TestEdgeCases();

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

    private static void TestBasicOperations()
    {
        Test("Grundlegende Operationen", () => {
            LRUCache cache = new LRUCache(2);
            
            cache.Put(1, 1);
            cache.Put(2, 2);
            
            int value1 = cache.Get(1);
            AssertEqual(1, value1, "Wert für Schlüssel 1 sollte 1 sein");
            
            cache.Put(3, 3);    // Entfernt Schlüssel 2
            
            int value2 = cache.Get(2);
            AssertEqual(-1, value2, "Schlüssel 2 sollte nicht mehr im Cache sein");
            
            int value3 = cache.Get(3);
            AssertEqual(3, value3, "Wert für Schlüssel 3 sollte 3 sein");
        });
    }

    private static void TestCapacityLimit()
    {
        Test("Kapazitätsgrenze", () => {
            LRUCache cache = new LRUCache(3);
            
            cache.Put(1, 1);
            cache.Put(2, 2);
            cache.Put(3, 3);
            cache.Put(4, 4);    // Entfernt Schlüssel 1
            
            int value1 = cache.Get(1);
            AssertEqual(-1, value1, "Schlüssel 1 sollte nicht mehr im Cache sein");
            
            int value2 = cache.Get(2);
            AssertEqual(2, value2, "Wert für Schlüssel 2 sollte 2 sein");
        });
    }

    private static void TestUpdateExistingKey()
    {
        Test("Aktualisierung vorhandener Schlüssel", () => {
            LRUCache cache = new LRUCache(2);
            
            cache.Put(1, 1);
            cache.Put(1, 10);  // Aktualisiere Wert für Schlüssel 1
            
            int value = cache.Get(1);
            AssertEqual(10, value, "Wert für Schlüssel 1 sollte auf 10 aktualisiert sein");
        });
    }

    private static void TestLRUEvictionPolicy()
    {
        Test("LRU Verdrängungsrichtlinie", () => {
            LRUCache cache = new LRUCache(3);
            
            cache.Put(1, 1);
            cache.Put(2, 2);
            cache.Put(3, 3);
            
            // Zugriff auf Schlüssel 1, dadurch wird er als kürzlich verwendet markiert
            cache.Get(1);
            
            // Füge einen neuen Schlüssel hinzu, der die Kapazität überschreitet
            cache.Put(4, 4);    // Sollte Schlüssel 2 entfernen, nicht 1
            
            int value1 = cache.Get(1);
            AssertEqual(1, value1, "Schlüssel 1 sollte noch im Cache sein");
            
            int value2 = cache.Get(2);
            AssertEqual(-1, value2, "Schlüssel 2 sollte nicht mehr im Cache sein");
            
            int value3 = cache.Get(3);
            AssertEqual(3, value3, "Schlüssel 3 sollte noch im Cache sein");
            
            int value4 = cache.Get(4);
            AssertEqual(4, value4, "Schlüssel 4 sollte im Cache sein");
        });
    }

    private static void TestEmptyCache()
    {
        Test("Leerer Cache", () => {
            LRUCache cache = new LRUCache(0);
            
            cache.Put(1, 1);
            int value = cache.Get(1);
            
            AssertEqual(-1, value, "Schlüssel 1 sollte nicht im Cache sein, da die Kapazität 0 ist");
        });
    }

    private static void TestSingleItemCache()
    {
        Test("Cache mit einem Element", () => {
            LRUCache cache = new LRUCache(1);
            
            cache.Put(1, 1);
            int value1 = cache.Get(1);
            AssertEqual(1, value1, "Wert für Schlüssel 1 sollte 1 sein");
            
            cache.Put(2, 2);    // Entfernt Schlüssel 1
            int value2 = cache.Get(1);
            AssertEqual(-1, value2, "Schlüssel 1 sollte nicht mehr im Cache sein");
            
            int value3 = cache.Get(2);
            AssertEqual(2, value3, "Wert für Schlüssel 2 sollte 2 sein");
        });
    }

    private static void TestEdgeCases()
    {
        Test("Grenzfälle", () => {
            LRUCache cache = new LRUCache(5);
            
            // Füge doppelte Schlüssel hinzu
            cache.Put(1, 1);
            cache.Put(1, 2);    // Aktualisiere Schlüssel 1
            int value1 = cache.Get(1);
            AssertEqual(2, value1, "Wert für Schlüssel 1 sollte 2 sein");
            
            // Zugriff auf nicht existierenden Schlüssel
            int value2 = cache.Get(999);
            AssertEqual(-1, value2, "Nicht existierender Schlüssel sollte -1 zurückgeben");
            
            // Abrufen eines Schlüssels ändert die LRU-Reihenfolge
            cache.Put(2, 2);
            cache.Put(3, 3);
            cache.Put(4, 4);
            cache.Put(5, 5);
            
            // Jetzt ist der Cache voll: [5, 4, 3, 2, 1]
            // Zugriff auf Schlüssel 2 ändert die Reihenfolge zu: [2, 5, 4, 3, 1]
            cache.Get(2);
            
            // Hinzufügen eines neuen Schlüssels sollte Schlüssel 1 entfernen
            cache.Put(6, 6);
            int value3 = cache.Get(1);
            AssertEqual(-1, value3, "Schlüssel 1 sollte nicht mehr im Cache sein");
            
            // Alle anderen Schlüssel sollten noch vorhanden sein
            AssertEqual(2, cache.Get(2), "Schlüssel 2 sollte noch im Cache sein");
            AssertEqual(3, cache.Get(3), "Schlüssel 3 sollte noch im Cache sein");
            AssertEqual(4, cache.Get(4), "Schlüssel 4 sollte noch im Cache sein");
            AssertEqual(5, cache.Get(5), "Schlüssel 5 sollte noch im Cache sein");
            AssertEqual(6, cache.Get(6), "Schlüssel 6 sollte im Cache sein");
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