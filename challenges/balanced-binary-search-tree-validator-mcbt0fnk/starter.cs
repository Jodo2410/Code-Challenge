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
        // TODO: Implementiere deine Lösung hier
        throw new NotImplementedException();
    }
}