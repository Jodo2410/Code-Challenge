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
using System.Text;

public static class MorseCodeConverter
{
    private static readonly Dictionary<char, string> MorseTable = new Dictionary<char, string>()
    {
        {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."},
        {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"},
        {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"},
        {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
        {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"}, {'Z', "--.."},
        {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"},
        {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, {'9', "----."}
    };

    public static string ToMorse(string input)
    {
        input = input.ToUpper();
        var sb = new StringBuilder();
        foreach (char c in input)
        {
            if (c == ' ')
            {
                sb.Append(" / ");
            }
            else if (MorseTable.ContainsKey(c))
            {
                sb.Append(MorseTable[c] + " ");
            }
        }
        return sb.ToString().Trim();
    }

    public static string FromMorse(string morse)
    {
        var reverseTable = new Dictionary<string, char>();
        foreach (var kv in MorseTable)
            reverseTable[kv.Value] = kv.Key;

        var words = morse.Split(new[] {" / "}, StringSplitOptions.None);
        var result = new StringBuilder();

        foreach (string word in words)
        {
            var letters = word.Split(' ');
            foreach (string letter in letters)
            {
                if (reverseTable.ContainsKey(letter))
                    result.Append(reverseTable[letter]);
            }
            result.Append(' ');
        }

        return result.ToString().Trim();
    }
}