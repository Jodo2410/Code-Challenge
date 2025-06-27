using System;

public static class Tests
{
    public static void RunAllTests()
    {
        int passed = 0;
        int total = 6;

        Console.WriteLine("=== Rock-Paper-Scissors Tests ===\n");

        if (Test("Rock vs Scissors", RPSLogic.DetermineWinner(Move.Rock, Move.Scissors), 1)) passed++;
        if (Test("Scissors vs Paper", RPSLogic.DetermineWinner(Move.Scissors, Move.Paper), 1)) passed++;
        if (Test("Paper vs Rock", RPSLogic.DetermineWinner(Move.Paper, Move.Rock), 1)) passed++;
        if (Test("Draw: Rock vs Rock", RPSLogic.DetermineWinner(Move.Rock, Move.Rock), 0)) passed++;
        if (Test("Scissors vs Rock", RPSLogic.DetermineWinner(Move.Scissors, Move.Rock), 2)) passed++;

        Scoreboard board = new Scoreboard();
        board.PlayRound(Move.Rock, Move.Scissors); // Spieler 1 gewinnt
        board.PlayRound(Move.Scissors, Move.Rock); // Spieler 2 gewinnt
        board.PlayRound(Move.Paper, Move.Paper);   // Unentschieden
        string score = board.GetScore();
        if (Test("Scoreboard Ergebnis", score, "Spieler 1: 1 | Spieler 2: 1")) passed++;

        Console.WriteLine($"\n{passed}/{total} Tests bestanden");
    }

    private static bool Test(string name, object actual, object expected)
    {
        bool success = actual.Equals(expected);
        Console.WriteLine(success
            ? $"[PASS] {name}"
            : $"[FAIL] {name} | Erwartet: '{expected}', Erhalten: '{actual}'");
        return success;
    }
}
