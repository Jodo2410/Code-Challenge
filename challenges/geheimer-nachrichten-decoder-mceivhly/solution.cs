/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * 
 * Diese Datei wird beim Build IGNORIERT und dient nur als Referenz.
 * Arbeite in der starter.cs Datei für deine eigene Implementation.
 * 
 * Tipp: Vergleiche deine Lösung mit dieser nachdem du fertig bist!
 */

using System;

public static class CaesarDecoder
{
    public static string DecodeMessage(string input, int shift)
    {
        string result = "";

        foreach (char c in input)
        {
            if (char.IsLetter(c))
            {
                char offset = char.IsUpper(c) ? 'A' : 'a';
                int pos = (c - offset + shift + 26) % 26;
                result += (char)(offset + pos);
            }
            else
            {
                result += c;
            }
        }

        return result;
    }
}