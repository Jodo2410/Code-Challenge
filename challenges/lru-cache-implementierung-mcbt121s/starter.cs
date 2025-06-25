using System;
using System.Collections.Generic;

public class LRUCache
{
    // TODO: Implementiere die notwendigen Felder und Datenstrukturen
    
    /// <summary>
    /// Initialisiert einen neuen LRU Cache mit der angegebenen Kapazität.
    /// </summary>
    /// <param name="capacity">Die Kapazität des Caches</param>
    public LRUCache(int capacity)
    {
        // TODO: Initialisierung
    }
    
    /// <summary>
    /// Gibt den Wert des angegebenen Schlüssels zurück, wenn er im Cache vorhanden ist.
    /// Aktualisiert den Schlüssel als kürzlich verwendet.
    /// </summary>
    /// <param name="key">Der zu suchende Schlüssel</param>
    /// <returns>Den Wert des Schlüssels, oder -1 wenn nicht gefunden</returns>
    public int Get(int key)
    {
        // TODO: Implementiere die Get-Methode
        throw new NotImplementedException();
    }
    
    /// <summary>
    /// Fügt einen Schlüssel-Wert-Paar in den Cache ein oder aktualisiert es.
    /// Wenn die Kapazität überschritten wird, entferne den am längsten nicht verwendeten Schlüssel.
    /// </summary>
    /// <param name="key">Der einzufügende Schlüssel</param>
    /// <param name="value">Der einzufügende Wert</param>
    public void Put(int key, int value)
    {
        // TODO: Implementiere die Put-Methode
        throw new NotImplementedException();
    }
}