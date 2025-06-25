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

public class ChessKingMoves
{
    /// <summary>
    /// Überprüft, ob der König sicher von seiner Startposition zur Zielposition gelangen kann.
    /// </summary>
    /// <param name="board">Das Schachbrett als 8x8 char-Array</param>
    /// <returns>True, wenn der König sicher zum Ziel gelangen kann, sonst False</returns>
    public static bool CanKingReachTarget(char[,] board)
    {
        if (board == null)
            return false;
        
        int rows = board.GetLength(0);
        int cols = board.GetLength(1);
        
        // Finde die Position des Königs und des Ziels
        int kingRow = -1, kingCol = -1;
        int targetRow = -1, targetCol = -1;
        
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (board[i, j] == 'K')
                {
                    kingRow = i;
                    kingCol = j;
                }
                else if (board[i, j] == 'T')
                {
                    targetRow = i;
                    targetCol = j;
                }
            }
        }
        
        // Überprüfe, ob König oder Ziel gefunden wurden
        if (kingRow == -1 || targetRow == -1)
            return false;
        
        // Breitensuche (BFS) zur Wegfindung
        Queue<(int, int)> queue = new Queue<(int, int)>();
        bool[,] visited = new bool[rows, cols];
        
        queue.Enqueue((kingRow, kingCol));
        visited[kingRow, kingCol] = true;
        
        // Mögliche Bewegungsrichtungen für den König (8 Richtungen)
        int[] dr = { -1, -1, 0, 1, 1, 1, 0, -1 };
        int[] dc = { 0, 1, 1, 1, 0, -1, -1, -1 };
        
        while (queue.Count > 0)
        {
            var (row, col) = queue.Dequeue();
            
            // Ziel erreicht
            if (row == targetRow && col == targetCol)
                return true;
            
            // Prüfe alle acht Richtungen
            for (int i = 0; i < 8; i++)
            {
                int newRow = row + dr[i];
                int newCol = col + dc[i];
                
                // Prüfe, ob die neue Position gültig, sicher und noch nicht besucht ist
                if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols && 
                    !visited[newRow, newCol] && board[newRow, newCol] != 'X')
                {
                    queue.Enqueue((newRow, newCol));
                    visited[newRow, newCol] = true;
                }
            }
        }
        
        // Kein Weg gefunden
        return false;
    }
}