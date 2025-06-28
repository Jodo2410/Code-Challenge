#include "Challenge.h"
#include <numeric>
#include <stdexcept>

/**
 * Challenge Implementation
 * Template-Version: {{TEMPLATE_VERSION}}
 */

/**
 * Haupt-Challenge Funktion
 * TODO: Implementiere deine Lösung hier
 */
int Challenge::solve(const std::vector<int>& input) {
    // Input-Validierung
    if (!validateInput(input)) {
        throw std::invalid_argument("Ungültige Eingabe");
    }
    
    // TODO: Implementiere deine Challenge-Lösung hier
    // Beispiel-Implementation (entfernen und durch echte Lösung ersetzen):
    
    // Beispiel: Summe aller positiven Elemente
    int sum = 0;
    for (int num : input) {
        if (num > 0) {
            sum += num;
        }
    }
    
    return sum;
    
    // Alternative Implementierung mit STL:
    // return std::accumulate(input.begin(), input.end(), 0, 
    //     [](int sum, int val) { return val > 0 ? sum + val : sum; });
    
    // TODO: Ersetze durch deine echte Challenge-Logik
    // throw std::runtime_error("Implementiere deine Challenge-Lösung");
}

/**
 * Hilfsfunktion zur Validierung
 */
bool Challenge::isValid(int value) {
    // Beispiel einer Validierungsfunktion
    return value >= 0 && value <= 1000; // Anpassen je nach Challenge
}

/**
 * Input-Verarbeitung
 */
std::vector<int> Challenge::processInput(const std::vector<int>& input) {
    std::vector<int> processed;
    processed.reserve(input.size());
    
    for (int value : input) {
        if (isValid(value)) {
            processed.push_back(value);
        }
    }
    
    return processed;
}

/**
 * Ergebnis ausgeben (für Debugging)
 */
void Challenge::printResult(int result) {
    std::cout << "Ergebnis: " << result << std::endl;
}

/**
 * Private: Summe berechnen
 */
int Challenge::calculateSum(const std::vector<int>& data) {
    return std::accumulate(data.begin(), data.end(), 0);
}

/**
 * Private: Input validieren
 */
bool Challenge::validateInput(const std::vector<int>& input) {
    // Basis-Validierung
    if (input.empty()) {
        return true; // Leere Eingabe ist oft erlaubt
    }
    
    // Größe prüfen
    if (input.size() > 10000) {
        return false; // Zu große Eingabe
    }
    
    // Werte prüfen
    for (int value : input) {
        if (value < -1000000 || value > 1000000) {
            return false; // Wert außerhalb des erlaubten Bereichs
        }
    }
    
    return true;
}
