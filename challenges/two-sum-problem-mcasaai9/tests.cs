using System;

public class TwoSumTests
{
    private static int testsPassed = 0;
    private static int totalTests = 0;

    public static void RunAllTests()
    {
        Console.WriteLine("🧪 Starte Tests für: Two Sum Problem");
        Console.WriteLine("=" + new string('=', 50));

        TestBasicCase();
        TestNegativeNumbers();
        TestZeroTarget();
        TestDuplicateNumbers();

        Console.WriteLine($"\n{testsPassed}/{totalTests} Tests bestanden");
        
        if (testsPassed == totalTests)
        {
            Console.WriteLine("🎉 Alle Tests bestanden! Challenge gelöst!");
        }
        else
        {
            Console.WriteLine("❌ Einige Tests sind fehlgeschlagen.");
            Environment.Exit(1);
        }
    }

    private static void TestBasicCase()
    {
        Test("Basis Test [2,7,11,15], target=9", () => {
            int[] nums = {2, 7, 11, 15};
            int target = 9;
            int[] result = TwoSum.FindTwoSum(nums, target);
            AssertArrayEqual(new int[] {0, 1}, result);
        });
    }

    private static void TestNegativeNumbers()
    {
        Test("Negative Zahlen [-1,-2,-3,-4,-5], target=-8", () => {
            int[] nums = {-1, -2, -3, -4, -5};
            int target = -8;
            int[] result = TwoSum.FindTwoSum(nums, target);
            AssertArrayEqual(new int[] {2, 4}, result);
        });
    }

    private static void TestZeroTarget()
    {
        Test("Ziel ist 0 [-3,4,-1,2,1,-5,6], target=0", () => {
            int[] nums = {-3, 4, -1, 2, 1, -5, 6};
            int target = 0;
            int[] result = TwoSum.FindTwoSum(nums, target);
            AssertArrayEqual(new int[] {1, 5}, result);
        });
    }

    private static void TestDuplicateNumbers()
    {
        Test("Doppelte Zahlen [3,3], target=6", () => {
            int[] nums = {3, 3};
            int target = 6;
            int[] result = TwoSum.FindTwoSum(nums, target);
            AssertArrayEqual(new int[] {0, 1}, result);
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
            Console.WriteLine($"  ❌ {description}");
            Console.WriteLine($"     Fehler: {ex.Message}");
        }
    }

    private static void AssertArrayEqual(int[] expected, int[] actual)
    {
        if (expected.Length != actual.Length)
        {
            throw new Exception($"Array Längen unterschiedlich. Erwartet: {expected.Length}, Erhalten: {actual.Length}");
        }

        for (int i = 0; i < expected.Length; i++)
        {
            if (expected[i] != actual[i])
            {
                throw new Exception($"Arrays unterschiedlich an Index {i}. Erwartet: [{string.Join(",", expected)}], Erhalten: [{string.Join(",", actual)}]");
            }
        }
    }
}