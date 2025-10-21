#include "Challenge.h"
#include <numeric>
#include <stdexcept>

/**
 * Challenge Implementation - Sum of Positive Numbers
 * Template-Version: 2.0
 */

/**
 * Haupt-Challenge Funktion
 * Berechnet die Summe aller positiven Zahlen im gegebenen Vektor
 */
int Challenge::solve(const std::vector<int>& input) {
    int sum = 0;
    
    // Durchlaufe alle Elemente im Vektor
    for (const int& value : input) {
        // Prüfe ob die Zahl positiv ist (> 0)
        if (isPositive(value)) {
            sum += value;
        }
    }
    
    return sum;
}

/**
 * Hilfsfunktion: Prüft ob eine Zahl positiv ist
 */
bool Challenge::isPositive(int value) {
    return value > 0;
}

/**
 * Hilfsfunktion: Validiert das Ergebnis
 */
bool Challenge::isValid(int result) {
    // Das Ergebnis sollte nie negativ sein, da wir nur positive Zahlen summieren
    return result >= 0;
}

/**
 * Hilfsfunktion: Zählt positive Zahlen im Vektor
 */
int Challenge::countPositive(const std::vector<int>& input) {
    int count = 0;
    
    for (const int& value : input) {
        if (isPositive(value)) {
            count++;
        }
    }
    
    return count;
}

/**
 * Hilfsfunktion: Findet die größte positive Zahl
 */
int Challenge::maxPositive(const std::vector<int>& input) {
    int maxVal = 0;
    
    for (const int& value : input) {
        if (isPositive(value) && value > maxVal) {
            maxVal = value;
        }
    }
    
    return maxVal;
}