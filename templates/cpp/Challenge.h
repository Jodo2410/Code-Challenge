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
 * {{CHALLENGE_TITLE}} - C++ Template-Challenge
 * 
 * Beschreibung: {{CHALLENGE_DESCRIPTION}}
 * Schwierigkeit: {{DIFFICULTY}}
 * Sprache: C++17
 * 
 * Template-Version: {{TEMPLATE_VERSION}}
 * Erstellt: {{CREATED_DATE}}
 * Autor: {{AUTHOR}}
 */

class Challenge {
public:
    /**
     * Haupt-Challenge Funktion
     * TODO: Implementiere deine Lösung hier
     * 
     * @param input - Eingabe Parameter (anpassen je nach Challenge)
     * @return Ergebnis (Typ anpassen je nach Challenge)
     */
    static int solve(const std::vector<int>& input);
    
    /**
     * Hilfsfunktion (optional)
     * TODO: Füge weitere Hilfsfunktionen hinzu falls benötigt
     */
    static bool isValid(int value);
    
    /**
     * Weitere Hilfsfunktionen (Beispiele)
     */
    static std::vector<int> processInput(const std::vector<int>& input);
    static void printResult(int result);
    
private:
    /**
     * Private Hilfsfunktionen
     */
    static int calculateSum(const std::vector<int>& data);
    static bool validateInput(const std::vector<int>& input);
};

#endif // CHALLENGE_H
