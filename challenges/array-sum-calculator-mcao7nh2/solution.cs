using System;

public class ArraySumCalculator
{
    /// <summary>
    /// Berechnet die Gesamtsumme aller Zahlen im Array
    /// </summary>
    /// <param name="numbers">Array von Ganzzahlen</param>
    /// <returns>Summe aller Zahlen</returns>
    public static int TotalSum(int[] numbers)
    {
        if (numbers == null || numbers.Length == 0)
            return 0;

        int sum = 0;
        foreach (int number in numbers)
        {
            sum += number;
        }
        return sum;
    }

    /// <summary>
    /// Berechnet die Summe aller geraden Zahlen im Array
    /// </summary>
    /// <param name="numbers">Array von Ganzzahlen</param>
    /// <returns>Summe aller geraden Zahlen</returns>
    public static int SumEvenNumbers(int[] numbers)
    {
        if (numbers == null || numbers.Length == 0)
            return 0;

        int sum = 0;
        foreach (int number in numbers)
        {
            if (number % 2 == 0)
            {
                sum += number;
            }
        }
        return sum;
    }

    /// <summary>
    /// Berechnet die Summe aller ungeraden Zahlen im Array
    /// </summary>
    /// <param name="numbers">Array von Ganzzahlen</param>
    /// <returns>Summe aller ungeraden Zahlen</returns>
    public static int SumOddNumbers(int[] numbers)
    {
        if (numbers == null || numbers.Length == 0)
            return 0;

        int sum = 0;
        foreach (int number in numbers)
        {
            if (number % 2 != 0)
            {
                sum += number;
            }
        }
        return sum;
    }

    /// <summary>
    /// Berechnet die Summe aller Zahlen zwischen min und max (inklusive)
    /// </summary>
    /// <param name="numbers">Array von Ganzzahlen</param>
    /// <param name="min">Minimaler Wert (inklusive)</param>
    /// <param name="max">Maximaler Wert (inklusive)</param>
    /// <returns>Summe aller Zahlen im angegebenen Bereich</returns>
    public static int SumInRange(int[] numbers, int min, int max)
    {
        if (numbers == null || numbers.Length == 0)
            return 0;

        int sum = 0;
        foreach (int number in numbers)
        {
            if (number >= min && number <= max)
            {
                sum += number;
            }
        }
        return sum;
    }
}