/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;
using System.Collections.Generic;

public class GamePathfinder
{
    /// <summary>
    /// Findet den kürzesten Weg vom Start- zum Zielpunkt auf dem Spielbrett.
    /// </summary>
    /// <param name="board">Das Spielbrett als 2D-Array: 0=frei, 1=Hindernis, 2=Start, 3=Ziel</param>
    /// <returns>Die minimale Anzahl von Schritten oder -1, wenn kein Weg existiert</returns>
    public static int FindShortestPath(int[,] board)
    {
        if (board == null)
            return -1;

        int rows = board.GetLength(0);
        int cols = board.GetLength(1);

        // Finde Start- und Zielposition
        int startRow = -1, startCol = -1;
        int targetRow = -1, targetCol = -1;

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (board[i, j] == 2)  // Start
                {
                    startRow = i;
                    startCol = j;
                }
                else if (board[i, j] == 3)  // Ziel
                {
                    targetRow = i;
                    targetCol = j;
                }
            }
        }

        // Wenn Start oder Ziel nicht gefunden wurden
        if (startRow == -1 || targetRow == -1)
            return -1;

        // BFS zum Finden des kürzesten Weges
        Queue<(int, int, int)> queue = new Queue<(int, int, int)>();  // (Zeile, Spalte, Schritte)
        bool[,] visited = new bool[rows, cols];
        
        queue.Enqueue((startRow, startCol, 0));
        visited[startRow, startCol] = true;

        // Mögliche Bewegungsrichtungen: oben, rechts, unten, links
        int[] dr = { -1, 0, 1, 0 };
        int[] dc = { 0, 1, 0, -1 };

        while (queue.Count > 0)
        {
            var (row, col, steps) = queue.Dequeue();

            // Ziel erreicht
            if (row == targetRow && col == targetCol)
                return steps;

            // Prüfe alle vier Richtungen
            for (int i = 0; i < 4; i++)
            {
                int newRow = row + dr[i];
                int newCol = col + dc[i];

                // Prüfe, ob die neue Position gültig ist
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols 
                    && !visited[newRow, newCol] && board[newRow, newCol] != 1)
                {
                    queue.Enqueue((newRow, newCol, steps + 1));
                    visited[newRow, newCol] = true;
                }
            }
        }

        // Kein Weg gefunden
        return -1;
    }
}