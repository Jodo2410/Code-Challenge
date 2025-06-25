using System;
using System.Diagnostics;

public static class Tests {
    public static void RunTests() {
        Debug.Assert(Solution.CountVowels("Hallo Welt") == 3);
        Debug.Assert(Solution.CountVowels("Programmieren macht Spaß") == 6);
        Debug.Assert(Solution.CountVowels("XYZ") == 0);
        Debug.Assert(Solution.CountVowels("AEIOUaeiou") == 10);
        Debug.Assert(Solution.CountVowels("") == 0);

        Console.WriteLine("Alle Tests erfolgreich bestanden.");
    }
}
