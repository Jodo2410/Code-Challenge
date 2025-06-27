/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public static class Rot13Cipher
{
    public static string Rot13(string input)
    {
        char Transform(char c)
        {
            if (char.IsLetter(c))
            {
                char offset = char.IsUpper(c) ? 'A' : 'a';
                return (char)((((c - offset) + 13) % 26) + offset);
            }
            return c;
        }

        string result = "";
        foreach (char c in input)
        {
            result += Transform(c);
        }
        return result;
    }
}