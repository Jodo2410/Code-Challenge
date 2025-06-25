using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Spielbrett-Pathfinding");
        Console.WriteLine(new string('=', 50));

        TestExample();
        TestNoPath();
        TestDirectPath();
        TestEmptyBoard();
        TestLargeBoard();
        TestNullBoard();

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

    private static void TestExample()
    {
        Test("Beispiel aus der Aufgabenstellung", () => {
            int[,] board = new int[,]
            {
                {0, 0, 0, 0},
                {0, 1, 1, 0},
                {2, 0, 0, 0},
                {1, 1, 0, 3}
            };

            int result = GamePathfinder.FindShortestPath(board);
            AssertEqual(5, result, "Der kürzeste Weg sollte 5 Schritte haben");
        });
    }

    private static void TestNoPath()
    {
        Test("Kein Weg möglich", () => {
            int[,] board = new int[,]
            {
                {0, 1, 0, 0},
                {0, 1, 1, 0},
                {2, 1, 0, 0},
                {1, 1, 0, 3}
            };

            int result = GamePathfinder.FindShortestPath(board);
            AssertEqual(-1, result, "Es sollte kein Weg gefunden werden");
        });
    }

    private static void TestDirectPath()
    {
        Test("Direkter Weg", () => {
            int[,] board = new int[,]
            {
                {2, 0, 0, 3},
            };

            int result = GamePathfinder.FindShortestPath(board);
            AssertEqual(3, result, "Der kürzeste Weg sollte 3 Schritte haben");
        });
    }

    private static void TestEmptyBoard()
    {
        Test("Leeres Brett", () => {
            int[,] board = new int[,] {};

            int result = GamePathfinder.FindShortestPath(board);
            AssertEqual(-1, result, "Bei leerem Brett sollte -1 zurückgegeben werden");
        });
    }

    private static void TestLargeBoard()
    {
        Test("Großes Brett", () => {
            int[,] board = new int[10, 10];
            // Setze Start- und Zielpunkt
            board[0, 0] = 2;
            board[9, 9] = 3;
            // Erstelle ein Labyrinth
            for (int i = 1; i < 9; i += 2)
            {
                for (int j = 0; j < 9; j++)
                {
                    board[i, j] = 1;
                }
                // Lasse eine Lücke
                board[i, i] = 0;
            }

            int result = GamePathfinder.FindShortestPath(board);
            AssertEqual(18, result, "Der kürzeste Weg durch das Labyrinth sollte 18 Schritte haben");
        });
    }

    private static void TestNullBoard()
    {
        Test("Null-Brett", () => {
            int result = GamePathfinder.FindShortestPath(null);
            AssertEqual(-1, result, "Bei null-Brett sollte -1 zurückgegeben werden");
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