using System;

public class TestRunner
{
    public static void Main(string[] args)
    {
        Console.WriteLine("🎯 hAL");
        Console.WriteLine("Führe Tests aus...\n");
        
        try
        {
            // ✅ KORRIGIERT: Verwendet standardisierten Klassennamen
            Tests.RunAllTests();
        }
        catch (Exception ex)
        {
            Console.WriteLine($"❌ Fehler: {ex.Message}");
            Environment.Exit(1);
        }
        
        Console.WriteLine("\nTests abgeschlossen.");
    }
}