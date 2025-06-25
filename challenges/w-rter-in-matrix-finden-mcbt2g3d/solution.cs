/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public class WordFinder
{
    /// <summary>
    /// Prüft, ob ein gegebenes Wort in einer 2D-Matrix gefunden werden kann.
    /// Das Wort kann gebildet werden, indem benachbarte Zellen in der Matrix verknüpft werden.
    /// Benachbarte Zellen sind horizontal oder vertikal miteinander verbunden (nicht diagonal).
    /// Die gleiche Zelle darf nicht mehrfach verwendet werden.
    /// </summary>
    /// <param name="board">Die 2D-Matrix von Buchstaben</param>
    /// <param name="word">Das zu suchende Wort</param>
    /// <returns>True, wenn das Wort gefunden werden kann, sonst False</returns>
    public static bool Exist(char[,] board, string word)
    {
        int rows = board.GetLength(0);
        int cols = board.GetLength(1);
        
        // Sonderfall: Leeres Wort ist immer gefunden
        if (string.IsNullOrEmpty(word))
        {
            return true;
        }
        
        // Sonderfall: Leeres Board kann kein Wort enthalten
        if (rows == 0 || cols == 0)
        {
            return false;
        }
        
        // Suche nach dem ersten Buchstaben des Wortes in der Matrix
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                // Wenn der erste Buchstabe gefunden wurde, starte die Suche
                if (board[i, j] == word[0])
                {
                    // Verwende eine 2D-Matrix, um zu verfolgen, welche Zellen bereits besucht wurden
                    bool[,] visited = new bool[rows, cols];
                    
                    // Markiere die aktuelle Zelle als besucht
                    visited[i, j] = true;
                    
                    // Starte die rekursive Suche nach dem Rest des Wortes
                    if (DFS(board, word, 1, i, j, visited))
                    {
                        return true;
                    }
                }
            }
        }
        
        // Wort wurde nicht gefunden
        return false;
    }
    
    /// <summary>
    /// Führt eine Tiefensuche (DFS) durch, um zu prüfen, ob der Rest des Wortes in der Matrix gefunden werden kann.
    /// </summary>
    /// <param name="board">Die 2D-Matrix von Buchstaben</param>
    /// <param name="word">Das zu suchende Wort</param>
    /// <param name="index">Der aktuelle Index im Wort</param>
    /// <param name="row">Die aktuelle Zeile in der Matrix</param>
    /// <param name="col">Die aktuelle Spalte in der Matrix</param>
    /// <param name="visited">Matrix, die angibt, welche Zellen bereits besucht wurden</param>
    /// <returns>True, wenn der Rest des Wortes gefunden werden kann, sonst False</returns>
    private static bool DFS(char[,] board, string word, int index, int row, int col, bool[,] visited)
    {
        // Wenn wir das Ende des Wortes erreicht haben, wurde das Wort gefunden
        if (index == word.Length)
        {
            return true;
        }
        
        int rows = board.GetLength(0);
        int cols = board.GetLength(1);
        
        // Die vier möglichen Richtungen: oben, rechts, unten, links
        int[] dx = { -1, 0, 1, 0 };
        int[] dy = { 0, 1, 0, -1 };
        
        // Überprüfe alle vier Richtungen
        for (int i = 0; i < 4; i++)
        {
            int newRow = row + dx[i];
            int newCol = col + dy[i];
            
            // Überprüfe, ob die neue Position gültig ist
            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols)
            {
                // Überprüfe, ob die Zelle nicht besucht wurde und der Buchstabe übereinstimmt
                if (!visited[newRow, newCol] && board[newRow, newCol] == word[index])
                {
                    // Markiere die Zelle als besucht
                    visited[newRow, newCol] = true;
                    
                    // Rekursiver Aufruf für den nächsten Buchstaben
                    if (DFS(board, word, index + 1, newRow, newCol, visited))
                    {
                        return true;
                    }
                    
                    // Backtracking: Markiere die Zelle wieder als nicht besucht
                    visited[newRow, newCol] = false;
                }
            }
        }
        
        // Keine gültige Lösung gefunden
        return false;
    }
}