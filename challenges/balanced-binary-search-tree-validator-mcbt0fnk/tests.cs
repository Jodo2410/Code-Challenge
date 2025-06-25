using System;

public class Tests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Balanced Binary Search Tree Validator");
        Console.WriteLine(new string('=', 50));

        TestEmptyTree();
        TestSingleNodeTree();
        TestBalancedBST();
        TestUnbalancedBST();
        TestNonBST();
        TestComplexBalancedBST();
        TestComplexUnbalancedBST();

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

    private static void TestEmptyTree()
    {
        Test("Leerer Baum", () => {
            TreeNode root = null;
            bool result = BalancedBSTValidator.IsBalancedBST(root);
            AssertEqual(true, result, "Ein leerer Baum sollte als ausgeglichener BST gelten");
        });
    }

    private static void TestSingleNodeTree()
    {
        Test("Baum mit einem Knoten", () => {
            TreeNode root = new TreeNode(5);
            bool result = BalancedBSTValidator.IsBalancedBST(root);
            AssertEqual(true, result, "Ein Baum mit einem Knoten sollte als ausgeglichener BST gelten");
        });
    }

    private static void TestBalancedBST()
    {
        Test("Ausgeglichener BST", () => {
            TreeNode root = new TreeNode(5);
            root.Left = new TreeNode(3);
            root.Right = new TreeNode(7);
            root.Left.Left = new TreeNode(2);
            root.Left.Right = new TreeNode(4);
            root.Right.Left = new TreeNode(6);
            root.Right.Right = new TreeNode(8);
            
            bool result = BalancedBSTValidator.IsBalancedBST(root);
            AssertEqual(true, result, "Der gegebene Baum sollte ein ausgeglichener BST sein");
        });
    }

    private static void TestUnbalancedBST()
    {
        Test("Nicht ausgeglichener BST", () => {
            TreeNode root = new TreeNode(5);
            root.Left = new TreeNode(3);
            root.Right = new TreeNode(7);
            root.Left.Left = new TreeNode(1);
            root.Right.Right = new TreeNode(9);
            root.Right.Right.Right = new TreeNode(11);
            
            bool result = BalancedBSTValidator.IsBalancedBST(root);
            AssertEqual(false, result, "Der gegebene Baum sollte ein BST, aber nicht ausgeglichen sein");
        });
    }

    private static void TestNonBST()
    {
        Test("Kein BST", () => {
            TreeNode root = new TreeNode(5);
            root.Left = new TreeNode(7); // Fehler: 7 > 5, sollte nicht im linken Teilbaum sein
            root.Right = new TreeNode(3); // Fehler: 3 < 5, sollte nicht im rechten Teilbaum sein
            
            bool result = BalancedBSTValidator.IsBalancedBST(root);
            AssertEqual(false, result, "Der gegebene Baum sollte kein BST sein");
        });
    }

    private static void TestComplexBalancedBST()
    {
        Test("Komplexer ausgeglichener BST", () => {
            TreeNode root = new TreeNode(15);
            root.Left = new TreeNode(10);
            root.Right = new TreeNode(20);
            root.Left.Left = new TreeNode(5);
            root.Left.Right = new TreeNode(12);
            root.Right.Left = new TreeNode(17);
            root.Right.Right = new TreeNode(25);
            root.Left.Left.Left = new TreeNode(3);
            root.Left.Left.Right = new TreeNode(7);
            
            bool result = BalancedBSTValidator.IsBalancedBST(root);
            AssertEqual(true, result, "Der komplexe Baum sollte ein ausgeglichener BST sein");
        });
    }

    private static void TestComplexUnbalancedBST()
    {
        Test("Komplexer nicht ausgeglichener BST", () => {
            TreeNode root = new TreeNode(15);
            root.Left = new TreeNode(10);
            root.Right = new TreeNode(20);
            root.Left.Left = new TreeNode(5);
            root.Left.Right = new TreeNode(12);
            root.Left.Left.Left = new TreeNode(3);
            root.Left.Left.Left.Left = new TreeNode(1); // Unausgeglichen hier
            
            bool result = BalancedBSTValidator.IsBalancedBST(root);
            AssertEqual(false, result, "Der komplexe Baum sollte ein BST, aber nicht ausgeglichen sein");
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