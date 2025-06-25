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
using System.Linq;

public class SkylineSolver
{
    /// <summary>
    /// Berechnet die Skyline einer Stadt aus einer Liste von Gebäuden.
    /// </summary>
    /// <param name="buildings">Liste von Gebäuden im Format [Li, Ri, Hi]</param>
    /// <returns>Skyline als Liste von Schlüsselpunkten [x, y]</returns>
    public static List<int[]> GetSkyline(int[][] buildings)
    {
        List<int[]> result = new List<int[]>();
        
        // Sonderfall: Keine Gebäude
        if (buildings == null || buildings.Length == 0)
        {
            return result;
        }
        
        // Liste aller kritischen Punkte (linke und rechte Kante jedes Gebäudes)
        List<BuildingPoint> points = new List<BuildingPoint>();
        
        // Füge alle linken und rechten Kanten der Gebäude hinzu
        foreach (var building in buildings)
        {
            // Linke Kante (negativer Höhenwert für Sortierung)
            points.Add(new BuildingPoint(building[0], -building[2], 0));
            // Rechte Kante (positiver Höhenwert für Sortierung)
            points.Add(new BuildingPoint(building[1], building[2], 1));
        }
        
        // Sortiere die Punkte nach x-Koordinate
        // Bei gleicher x-Koordinate: links vor rechts und höheres Gebäude vor niedrigerem
        points.Sort((a, b) => {
            if (a.X != b.X) return a.X - b.X;
            if (a.Type != b.Type) return a.Type - b.Type;
            if (a.Type == 0) return a.Height - b.Height; // linke Kanten: höhere zuerst
            return b.Height - a.Height; // rechte Kanten: niedrigere zuerst
        });
        
        // Max-Heap für die aktuellen Höhen
        // In C# gibt es keine direkte Max-Heap-Implementierung, daher verwenden wir eine SortedSet
        // mit negativen Werten als Workaround
        SortedSet<int> heights = new SortedSet<int>(Comparer<int>.Create((a, b) => b.CompareTo(a)));
        heights.Add(0); // Füge Bodenhöhe hinzu
        
        int prevMaxHeight = 0;
        
        // Verarbeite alle Punkte
        foreach (var point in points)
        {
            if (point.Type == 0) // Linke Kante
            {
                heights.Add(-point.Height); // Füge die Höhe zum Max-Heap hinzu (negativ, weil wir einen Max-Heap simulieren)
            }
            else // Rechte Kante
            {
                heights.Remove(-point.Height); // Entferne die Höhe aus dem Max-Heap
            }
            
            // Aktuelle maximale Höhe
            int currentMaxHeight = heights.Min(); // Da wir negative Werte verwenden, ist das Minimum tatsächlich das Maximum
            
            // Wenn sich die maximale Höhe geändert hat, füge einen neuen Schlüsselpunkt hinzu
            if (currentMaxHeight != prevMaxHeight)
            {
                result.Add(new int[] { point.X, currentMaxHeight });
                prevMaxHeight = currentMaxHeight;
            }
        }
        
        return result;
    }
    
    private class BuildingPoint
    {
        public int X { get; set; }      // x-Koordinate
        public int Height { get; set; } // Höhe des Gebäudes
        public int Type { get; set; }   // 0 = linke Kante, 1 = rechte Kante
        
        public BuildingPoint(int x, int height, int type)
        {
            X = x;
            Height = height;
            Type = type;
        }
    }
}