using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Wörter in Matrix finden");
        Console.WriteLine(new string('=', 50));

        TestBasicCases();
        TestEmptyWordAndBoard();
        TestWordTooLong();
        TestNonExistentWord();
        TestWordWithRepeatedCharacters();
        TestLargeBoard();
        TestZigZagPattern();
        TestSingleCharacterWord();

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
        Test("Beispiel aus der Aufgabenstellung - ABCCED", () => {
            char[,] board = {
                {'A', 'B', 'C', 'E'},
                {'S', 'F', 'C', 'S'},
                {'A', 'D', 'E', 'E'}
            };
            bool result = WordFinder.Exist(board, "ABCCED");
            AssertEqual(true, result, "Das Wort 'ABCCED' sollte in der Matrix gefunden werden");
        });

        Test("Beispiel aus der Aufgabenstellung - SEE", () => {
            char[,] board = {
                {'A', 'B', 'C', 'E'},
                {'S', 'F', 'C', 'S'},
                {'A', 'D', 'E', 'E'}
            };
            bool result = WordFinder.Exist(board, "SEE");
            AssertEqual(true, result, "Das Wort 'SEE' sollte in der Matrix gefunden werden");
        });

        Test("Beispiel aus der Aufgabenstellung - ABCB", () => {
            char[,] board = {
                {'A', 'B', 'C', 'E'},
                {'S', 'F', 'C', 'S'},
                {'A', 'D', 'E', 'E'}
            };
            bool result = WordFinder.Exist(board, "ABCB");
            AssertEqual(false, result, "Das Wort 'ABCB' sollte nicht in der Matrix gefunden werden");
        });
    }

    private static void TestEmptyWordAndBoard()
    {
        Test("Leeres Wort", () => {
            char[,] board = {
                {'A', 'B'},
                {'C', 'D'}
            };
            bool result = WordFinder.Exist(board, "");
            AssertEqual(true, result, "Ein leeres Wort sollte immer gefunden werden");
        });

        Test("Leeres Board", () => {
            char[,] board = new char[0, 0];
            bool result = WordFinder.Exist(board, "ABC");
            AssertEqual(false, result, "Ein Wort kann in einer leeren Matrix nicht gefunden werden");
        });
    }

    private static void TestWordTooLong()
    {
        Test("Wort länger als mögliche Pfade", () => {
            char[,] board = {
                {'A', 'B'},
                {'C', 'D'}
            };
            bool result = WordFinder.Exist(board, "ABCDEFG");
            AssertEqual(false, result, "Ein Wort, das länger ist als mögliche Pfade, sollte nicht gefunden werden");
        });
    }

    private static void TestNonExistentWord()
    {
        Test("Nicht existierendes Wort", () => {
            char[,] board = {
                {'A', 'B', 'C'},
                {'D', 'E', 'F'},
                {'G', 'H', 'I'}
            };
            bool result = WordFinder.Exist(board, "XYZ");
            AssertEqual(false, result, "Ein Wort mit Buchstaben, die nicht im Board vorkommen, sollte nicht gefunden werden");
        });
    }

    private static void TestWordWithRepeatedCharacters()
    {
        Test("Wort mit wiederholten Zeichen", () => {
            char[,] board = {
                {'A', 'A', 'A'},
                {'A', 'B', 'A'},
                {'A', 'A', 'A'}
            };
            bool result = WordFinder.Exist(board, "AAAAA");
            AssertEqual(true, result, "Ein Wort mit wiederholten Zeichen sollte gefunden werden, wenn es einen gültigen Pfad gibt");
        });
    }

    private static void TestLargeBoard()
    {
        Test("Großes Board", () => {
            char[,] board = {
                {'A', 'B', 'C', 'D', 'E'},
                {'F', 'G', 'H', 'I', 'J'},
                {'K', 'L', 'M', 'N', 'O'},
                {'P', 'Q', 'R', 'S', 'T'},
                {'U', 'V', 'W', 'X', 'Y'}
            };
            bool result = WordFinder.Exist(board, "FGLQVWST");
            AssertEqual(true, result, "Ein komplexes Wort in einem großen Board sollte gefunden werden");
        });
    }

    private static void TestZigZagPattern()
    {
        Test("Zick-Zack-Muster", () => {
            char[,] board = {
                {'A', 'B', 'C', 'D'},
                {'E', 'F', 'G', 'H'},
                {'I', 'J', 'K', 'L'}
            };
            bool result = WordFinder.Exist(board, "AFJGKL");
            AssertEqual(true, result, "Ein Wort, das einen Zick-Zack-Pfad bildet, sollte gefunden werden");
        });
    }

    private static void TestSingleCharacterWord()
    {
        Test("Einzelnes Zeichen", () => {
            char[,] board = {
                {'A', 'B'},
                {'C', 'D'}
            };
            bool result = WordFinder.Exist(board, "A");
            AssertEqual(true, result, "Ein einzelnes Zeichen, das im Board vorkommt, sollte gefunden werden");

            result = WordFinder.Exist(board, "Z");
            AssertEqual(false, result, "Ein einzelnes Zeichen, das nicht im Board vorkommt, sollte nicht gefunden werden");
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