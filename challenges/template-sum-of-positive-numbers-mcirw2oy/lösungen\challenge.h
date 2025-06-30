#ifndef CHALLENGE_H
#define CHALLENGE_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <utility>
#include <cmath>
#include <climits>

/**
 * Sum of Positive Numbers - C++ Template-Challenge
 * 
 * Beschreibung: Berechne die Summe aller positiven Zahlen in einem Vektor.
 *               Negative Zahlen und Null werden ignoriert.
 * 
 * Schwierigkeit: easy
 * Sprache: C++17
 * Kategorie: Arrays, Basic Math
 * 
 * Template-Version: 2.0
 * Erstellt: 30.6.2025
 * Autor: Template-Generator
 */

class Challenge {
public:
    /**
     * Haupt-Challenge Funktion
     * Berechnet die Summe aller positiven Zahlen im gegebenen Vektor
     * 
     * @param input - Vektor mit Integer-Zahlen
     * @return Summe aller positiven Zahlen (int)
     */
    static int solve(const std::vector<int>& input);

    /**
     * Hilfsfunktion: Prüft ob eine Zahl positiv ist
     * 
     * @param value - Zu prüfende Zahl
     * @return true wenn value > 0, sonst false
     */
    static bool isPositive(int value);

    /**
     * Hilfsfunktion: Validiert das Ergebnis
     * 
     * @param result - Das berechnete Ergebnis
     * @return true wenn das Ergebnis gültig ist (>= 0)
     */
    static bool isValid(int result);

    /**
     * Hilfsfunktion: Zählt positive Zahlen im Vektor
     * 
     * @param input - Eingabe-Vektor
     * @return Anzahl der positiven Zahlen
     */
    static int countPositive(const std::vector<int>& input);

    /**
     * Hilfsfunktion: Findet die größte positive Zahl
     * 
     * @param input - Eingabe-Vektor
     * @return Größte positive Zahl, oder 0 wenn keine gefunden
     */
    static int maxPositive(const std::vector<int>& input);
    
private:
    // Private Hilfsfunktionen falls benötigt
};

#endif // CHALLENGE_H