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

public class LRUCache
{
    private int capacity;
    private Dictionary<int, LinkedListNode<CacheItem>> cache;
    private LinkedList<CacheItem> lruList;
    
    // Innere Klasse für Schlüssel-Wert-Paare
    private class CacheItem
    {
        public int Key { get; set; }
        public int Value { get; set; }
        
        public CacheItem(int key, int value)
        {
            Key = key;
            Value = value;
        }
    }
    
    /// <summary>
    /// Initialisiert einen neuen LRU Cache mit der angegebenen Kapazität.
    /// </summary>
    /// <param name="capacity">Die Kapazität des Caches</param>
    public LRUCache(int capacity)
    {
        this.capacity = capacity;
        cache = new Dictionary<int, LinkedListNode<CacheItem>>(capacity);
        lruList = new LinkedList<CacheItem>();
    }
    
    /// <summary>
    /// Gibt den Wert des angegebenen Schlüssels zurück, wenn er im Cache vorhanden ist.
    /// Aktualisiert den Schlüssel als kürzlich verwendet.
    /// </summary>
    /// <param name="key">Der zu suchende Schlüssel</param>
    /// <returns>Den Wert des Schlüssels, oder -1 wenn nicht gefunden</returns>
    public int Get(int key)
    {
        // Prüfe, ob der Schlüssel im Cache vorhanden ist
        if (!cache.ContainsKey(key))
        {
            return -1;
        }
        
        // Hole den Knoten
        LinkedListNode<CacheItem> node = cache[key];
        int value = node.Value.Value;
        
        // Bewege den Knoten an den Anfang der Liste (als kürzlich verwendet markieren)
        lruList.Remove(node);
        lruList.AddFirst(node);
        
        return value;
    }
    
    /// <summary>
    /// Fügt einen Schlüssel-Wert-Paar in den Cache ein oder aktualisiert es.
    /// Wenn die Kapazität überschritten wird, entferne den am längsten nicht verwendeten Schlüssel.
    /// </summary>
    /// <param name="key">Der einzufügende Schlüssel</param>
    /// <param name="value">Der einzufügende Wert</param>
    public void Put(int key, int value)
    {
        // Falls der Schlüssel bereits existiert, aktualisiere den Wert und bewege den Knoten
        if (cache.ContainsKey(key))
        {
            LinkedListNode<CacheItem> existingNode = cache[key];
            existingNode.Value.Value = value;
            
            // Bewege den Knoten an den Anfang der Liste
            lruList.Remove(existingNode);
            lruList.AddFirst(existingNode);
        }
        else
        {
            // Prüfe, ob die Kapazität erreicht ist
            if (cache.Count >= capacity)
            {
                // Entferne den am längsten nicht verwendeten Eintrag (letzter in der Liste)
                LinkedListNode<CacheItem> lastNode = lruList.Last;
                lruList.RemoveLast();
                cache.Remove(lastNode.Value.Key);
            }
            
            // Füge den neuen Eintrag hinzu
            CacheItem newItem = new CacheItem(key, value);
            LinkedListNode<CacheItem> newNode = new LinkedListNode<CacheItem>(newItem);
            lruList.AddFirst(newNode);
            cache.Add(key, newNode);
        }
    }
}