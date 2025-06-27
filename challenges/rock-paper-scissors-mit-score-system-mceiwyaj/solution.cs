/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public enum Move
{
    Rock,
    Paper,
    Scissors
}

public static class RPSLogic
{
    public static int DetermineWinner(Move player1, Move player2)
    {
        if (player1 == player2) return 0;

        if ((player1 == Move.Rock && player2 == Move.Scissors) ||
            (player1 == Move.Scissors && player2 == Move.Paper) ||
            (player1 == Move.Paper && player2 == Move.Rock))
        {
            return 1;
        }

        return 2;
    }
}

public class Scoreboard
{
    public int Player1Score { get; private set; } = 0;
    public int Player2Score { get; private set; } = 0;

    public void PlayRound(Move player1, Move player2)
    {
        int result = RPSLogic.DetermineWinner(player1, player2);
        if (result == 1) Player1Score++;
        else if (result == 2) Player2Score++;
    }

    public string GetScore()
    {
        return $"Spieler 1: {Player1Score} | Spieler 2: {Player2Score}";
    }
}