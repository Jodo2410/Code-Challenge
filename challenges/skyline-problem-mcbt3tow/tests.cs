using System;
using System.Collections.Generic;
using System.Linq;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Skyline Problem");
        Console.WriteLine(new string('=', 50));

        TestEmptyInput();
        TestSingleBuilding();
        TestNonOverlappingBuildings();
        TestOverlappingBuildings();
        TestExampleCase();
        TestComplexCase();
        TestSameHeightBuildings();
        TestBuildingsWithSameStartOrEnd();

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

    private static void TestEmptyInput()
    {
        Test("Leere Eingabe", () => {
            int[][] buildings = new int[0][];
            List<int[]> result = SkylineSolver.GetSkyline(buildings);
            AssertEmptyList(result, "Bei leerer Eingabe sollte eine leere Liste zurückgegeben werden");
        });
    }

    private static void TestSingleBuilding()
    {
        Test("Einzelnes Gebäude", () => {
            int[][] buildings = new int[][] { new int[] { 1, 5, 10 } };
            List<int[]> result = SkylineSolver.GetSkyline(buildings);
            List<int[]> expected = new List<int[]> { new int[] { 1, 10 }, new int[] { 5, 0 } };
            AssertListEquals(expected, result, "Skyline für ein einzelnes Gebäude");
        });
    }

    private static void TestNonOverlappingBuildings()
    {
        Test("Nicht überlappende Gebäude", () => {
            int[][] buildings = new int[][] {
                new int[] { 1, 3, 10 },
                new int[] { 5, 8, 15 },
                new int[] { 10, 13, 8 }
            };
            List<int[]> result = SkylineSolver.GetSkyline(buildings);
            List<int[]> expected = new List<int[]> {
                new int[] { 1, 10 },
                new int[] { 3, 0 },
                new int[] { 5, 15 },
                new int[] { 8, 0 },
                new int[] { 10, 8 },
                new int[] { 13, 0 }
            };
            AssertListEquals(expected, result, "Skyline für nicht überlappende Gebäude");
        });
    }

    private static void TestOverlappingBuildings()
    {
        Test("Überlappende Gebäude", () => {
            int[][] buildings = new int[][] {
                new int[] { 1, 5, 10 },
                new int[] { 3, 7, 15 }
            };
            List<int[]> result = SkylineSolver.GetSkyline(buildings);
            List<int[]> expected = new List<int[]> {
                new int[] { 1, 10 },
                new int[] { 3, 15 },
                new int[] { 7, 0 }
            };
            AssertListEquals(expected, result, "Skyline für überlappende Gebäude");
        });
    }

    private static void TestExampleCase()
    {
        Test("Beispielfall aus Aufgabenstellung", () => {
            int[][] buildings = new int[][] {
                new int[] { 2, 9, 10 },
                new int[] { 3, 7, 15 },
                new int[] { 5, 12, 12 },
                new int[] { 15, 20, 10 },
                new int[] { 19, 24, 8 }
            };
            List<int[]> result = SkylineSolver.GetSkyline(buildings);
            List<int[]> expected = new List<int[]> {
                new int[] { 2, 10 },
                new int[] { 3, 15 },
                new int[] { 7, 12 },
                new int[] { 12, 0 },
                new int[] { 15, 10 },
                new int[] { 20, 8 },
                new int[] { 24, 0 }
            };
            AssertListEquals(expected, result, "Skyline für den Beispielfall");
        });
    }

    private static void TestComplexCase()
    {
        Test("Komplexer Fall", () => {
            int[][] buildings = new int[][] {
                new int[] { 1, 3, 5 },
                new int[] { 2, 4, 10 },
                new int[] { 3, 5, 15 },
                new int[] { 4, 6, 8 },
                new int[] { 5, 8, 12 },
                new int[] { 7, 9, 7 }
            };
            List<int[]> result = SkylineSolver.GetSkyline(buildings);
            List<int[]> expected = new List<int[]> {
                new int[] { 1, 5 },
                new int[] { 2, 10 },
                new int[] { 3, 15 },
                new int[] { 5, 12 },
                new int[] { 8, 7 },
                new int[] { 9, 0 }
            };
            AssertListEquals(expected, result, "Skyline für komplexen Fall");
        });
    }

    private static void TestSameHeightBuildings()
    {
        Test("Gebäude mit gleicher Höhe", () => {
            int[][] buildings = new int[][] {
                new int[] { 1, 5, 10 },
                new int[] { 6, 10, 10 }
            };
            List<int[]> result = SkylineSolver.GetSkyline(buildings);
            List<int[]> expected = new List<int[]> {
                new int[] { 1, 10 },
                new int[] { 5, 0 },
                new int[] { 6, 10 },
                new int[] { 10, 0 }
            };
            AssertListEquals(expected, result, "Skyline für Gebäude mit gleicher Höhe");
        });
    }

    private static void TestBuildingsWithSameStartOrEnd()
    {
        Test("Gebäude mit gleichem Start oder Ende", () => {
            int[][] buildings = new int[][] {
                new int[] { 1, 5, 10 },
                new int[] { 1, 3, 12 },
                new int[] { 3, 5, 8 }
            };
            List<int[]> result = SkylineSolver.GetSkyline(buildings);
            List<int[]> expected = new List<int[]> {
                new int[] { 1, 12 },
                new int[] { 3, 10 },
                new int[] { 5, 0 }
            };
            AssertListEquals(expected, result, "Skyline für Gebäude mit gleichem Start oder Ende");
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

    private static void AssertEmptyList(List<int[]> list, string message = null)
    {
        if (list == null || list.Count > 0)
        {
            throw new Exception($"{message ?? ""}\nErwartet: leere Liste, Erhalten: {(list == null ? "null" : $"Liste mit {list.Count} Elementen")}");
        }
    }

    private static void AssertListEquals(List<int[]> expected, List<int[]> actual, string message = null)
    {
        if (expected == null && actual == null) return;
        if (expected == null || actual == null)
        {
            throw new Exception($"{message ?? ""}\nErwartet: {(expected == null ? "null" : "nicht null")}, Erhalten: {(actual == null ? "null" : "nicht null")}");
        }
        
        if (expected.Count != actual.Count)
        {
            throw new Exception($"{message ?? ""}\nErwartete Listenlänge: {expected.Count}, Erhaltene Listenlänge: {actual.Count}");
        }
        
        for (int i = 0; i < expected.Count; i++)
        {
            if (expected[i].Length != actual[i].Length)
            {
                throw new Exception($"{message ?? ""}\nArraylänge unterschiedlich bei Index {i}");
            }
            
            for (int j = 0; j < expected[i].Length; j++)
            {
                if (expected[i][j] != actual[i][j])
                {
                    throw new Exception($"{message ?? ""}\nArrays unterschiedlich bei Index {i}:\nErwartet: [{string.Join(", ", expected[i])}], Erhalten: [{string.Join(", ", actual[i])}]");
                }
            }
        }
    }
}