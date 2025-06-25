using System;

public static class Tests {
    public static void RunAllTests() {
        int passed = 0;
        int total = 3;

        int[][] grid1 = new int[][] {
            new int[] {1, 3, 1},
            new int[] {1, 5, 1},
            new int[] {4, 2, 1}
        };
        if (Solution.MinPathSum(grid1) == 7) {
            Console.WriteLine("Test 1 bestanden ✓");
            passed++;
        } else {
            Console.WriteLine("Test 1 fehlgeschlagen ✗");
        }

        int[][] grid2 = new int[][] {
            new int[] {1, 2, 3},
            new int[] {4, 5, 6}
        };
        if (Solution.MinPathSum(grid2) == 12) {
            Console.WriteLine("Test 2 bestanden ✓");
            passed++;
        } else {
            Console.WriteLine("Test 2 fehlgeschlagen ✗");
        }

        int[][] grid3 = new int[][] {
            new int[] {5, 9},
            new int[] {6, 1}
        };
        if (Solution.MinPathSum(grid3) == 12) {
            Console.WriteLine("Test 3 bestanden ✓");
            passed++;
        } else {
            Console.WriteLine("Test 3 fehlgeschlagen ✗");
        }

        Console.WriteLine($"{passed}/{total} Tests bestanden.");
    }
}