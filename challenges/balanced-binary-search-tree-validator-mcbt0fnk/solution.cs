/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public class TreeNode
{
    public int Value;
    public TreeNode Left;
    public TreeNode Right;

    public TreeNode(int value)
    {
        Value = value;
        Left = null;
        Right = null;
    }
}

public class BalancedBSTValidator
{
    /// <summary>
    /// Überprüft, ob der gegebene Baum ein ausgeglichener binärer Suchbaum (BST) ist.
    /// Ein ausgeglichener Baum bedeutet, dass für jeden Knoten der Höhenunterschied zwischen
    /// dem linken und rechten Teilbaum nicht größer als 1 ist.
    /// Ein BST bedeutet, dass für jeden Knoten alle Werte im linken Teilbaum kleiner und
    /// alle Werte im rechten Teilbaum größer sind als der Wert des Knotens selbst.
    /// </summary>
    /// <param name="root">Der Wurzelknoten des zu überprüfenden Baums</param>
    /// <returns>True, wenn der Baum ein ausgeglichener BST ist, sonst False</returns>
    public static bool IsBalancedBST(TreeNode root)
    {
        // Ein leerer Baum ist per Definition ein ausgeglichener BST
        if (root == null)
        {
            return true;
        }

        // Prüfe, ob der Baum ein BST ist
        if (!IsBST(root, int.MinValue, int.MaxValue))
        {
            return false;
        }

        // Prüfe, ob der Baum ausgeglichen ist
        return IsBalanced(root) != -1;
    }

    // Hilfsmethode, die überprüft, ob der Baum ein binärer Suchbaum ist
    private static bool IsBST(TreeNode node, int min, int max)
    {
        // Ein leerer Knoten ist ein gültiger BST
        if (node == null)
        {
            return true;
        }

        // Überprüfe, ob der aktuelle Knoten im gültigen Bereich liegt
        if (node.Value <= min || node.Value >= max)
        {
            return false;
        }

        // Überprüfe rekursiv den linken und rechten Teilbaum
        // Für den linken Teilbaum: alle Werte müssen kleiner als der aktuelle Knoten sein
        // Für den rechten Teilbaum: alle Werte müssen größer als der aktuelle Knoten sein
        return IsBST(node.Left, min, node.Value) && IsBST(node.Right, node.Value, max);
    }

    // Hilfsmethode, die die Höhe des Baums berechnet und gleichzeitig prüft,
    // ob er ausgeglichen ist. Gibt -1 zurück, wenn nicht ausgeglichen.
    private static int IsBalanced(TreeNode node)
    {
        // Ein leerer Knoten hat Höhe 0
        if (node == null)
        {
            return 0;
        }

        // Berechne die Höhe des linken Teilbaums
        int leftHeight = IsBalanced(node.Left);
        if (leftHeight == -1)
        {
            return -1; // Linker Teilbaum ist nicht ausgeglichen
        }

        // Berechne die Höhe des rechten Teilbaums
        int rightHeight = IsBalanced(node.Right);
        if (rightHeight == -1)
        {
            return -1; // Rechter Teilbaum ist nicht ausgeglichen
        }

        // Überprüfe, ob der Höhenunterschied größer als 1 ist
        if (Math.Abs(leftHeight - rightHeight) > 1)
        {
            return -1; // Baum ist nicht ausgeglichen
        }

        // Gib die Höhe des aktuellen Knotens zurück
        return Math.Max(leftHeight, rightHeight) + 1;
    }
}