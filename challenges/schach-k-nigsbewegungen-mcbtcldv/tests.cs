using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Schach - Königsbewegungen");
        Console.WriteLine(new string('=', 50));

        TestExample();
        TestNoPath();
        TestDirectPath();
        TestEmptyBoard();
        TestBlockedByEnemies();
        TestComplexPath();
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
            char[,] board = new char[,]
            {
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', 'X', 'X', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', 'X', '.', '.'},
                {'.', 'X', 'K', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', 'X', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', 'T', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'}
            };

            bool result = ChessKingMoves.CanKingReachTarget(board);
            AssertEqual(true, result, "Der König sollte das Ziel erreichen können");
        });
    }

    private static void TestNoPath()
    {
        Test("Kein Weg möglich", () => {
            char[,] board = new char[,]
            {
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', 'X', 'X', '.', '.', '.', '.'},
                {'.', '.', 'X', 'X', 'X', 'X', '.', '.'},
                {'.', 'X', 'K', 'X', 'X', 'X', '.', '.'},
                {'.', '.', 'X', 'X', 'X', 'X', '.', '.'},
                {'.', '.', 'X', 'X', '.', '.', '.', '.'},
                {'.', '.', '.', 'T', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'}
            };

            bool result = ChessKingMoves.CanKingReachTarget(board);
            AssertEqual(false, result, "Der König sollte durch die Barriere aus bedrohten Feldern nicht zum Ziel gelangen können");
        });
    }

    private static void TestDirectPath()
    {
        Test("Direkter Weg", () => {
            char[,] board = new char[,]
            {
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', 'K', 'T', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'}
            };

            bool result = ChessKingMoves.CanKingReachTarget(board);
            AssertEqual(true, result, "Der König sollte direkt zum benachbarten Zielfeld gehen können");
        });
    }

    private static void TestEmptyBoard()
    {
        Test("Leeres Brett", () => {
            char[,] board = new char[0, 0];

            bool result = ChessKingMoves.CanKingReachTarget(board);
            AssertEqual(false, result, "Bei leerem Brett sollte false zurückgegeben werden");
        });
    }

    private static void TestBlockedByEnemies()
    {
        Test("Von Feinden umzingelt", () => {
            char[,] board = new char[,]
            {
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', 'X', 'X', 'X', '.', '.', '.'},
                {'.', '.', 'X', 'K', 'X', '.', '.', '.'},
                {'.', '.', 'X', 'X', 'X', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', 'T', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', '.'}
            };

            bool result = ChessKingMoves.CanKingReachTarget(board);
            AssertEqual(false, result, "Der König sollte nicht zum Ziel gelangen können, wenn er von Feinden umzingelt ist");
        });
    }

    private static void TestComplexPath()
    {
        Test("Komplexer Pfad", () => {
            char[,] board = new char[,]
            {
                {'K', '.', 'X', '.', '.', '.', '.', '.'},
                {'.', '.', 'X', 'X', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', 'X', '.', '.'},
                {'.', 'X', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', 'X', '.', '.'},
                {'.', '.', 'X', '.', '.', '.', '.', '.'},
                {'.', 'X', '.', '.', '.', '.', '.', '.'},
                {'.', '.', '.', '.', '.', '.', '.', 'T'}
            };

            bool result = ChessKingMoves.CanKingReachTarget(board);
            AssertEqual(true, result, "Der König sollte einen komplexen Pfad zum Ziel finden können");
        });
    }

    private static void TestNullBoard()
    {
        Test("Null-Brett", () => {
            bool result = ChessKingMoves.CanKingReachTarget(null);
            AssertEqual(false, result, "Bei null-Brett sollte false zurückgegeben werden");
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