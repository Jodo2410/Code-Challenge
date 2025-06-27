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
        // TODO: Implementiere die Spielregel-Logik
        return -1;
    }
}

public class Scoreboard
{
    public int Player1Score { get; private set; }
    public int Player2Score { get; private set; }

    public void PlayRound(Move player1, Move player2)
    {
        // TODO: Verwende RPSLogic.DetermineWinner und aktualisiere die Punkte
    }

    public string GetScore()
    {
        // TODO: Gib den Punktestand zurück (z. B. "Spieler 1: 2 | Spieler 2: 1")
        return "";
    }
}