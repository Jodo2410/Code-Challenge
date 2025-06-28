#include "Tests.h"
#include <random>

/**
 * Test Implementation
 * Template-Version: {{TEMPLATE_VERSION}}
 */

// Statische Member-Initialisierung
int Tests::testsPassed = 0;
int Tests::totalTests = 0;
std::vector<std::string> Tests::failedTests = {};

/**
 * Führt alle Tests aus
 */
void Tests::runAllTests() {
    std::cout << "🧪 Starte C++ Challenge Tests" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "Template: C++ mit separaten .h/.cpp Dateien" << std::endl;
    std::cout << "Challenge: {{CHALLENGE_TITLE}}" << std::endl;
    std::cout << "Compiler: " << __VERSION__ << std::endl;
    std::cout << "C++ Standard: " << __cplusplus << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    // Alle Test-Kategorien ausführen
    testBasicFunctionality();
    testEdgeCases();
    testPerformance();
    testInvalidInput();
    testLargeInput();
    testSpecialCases();

    // Test-Ergebnisse ausgeben
    printTestResults();
}

/**
 * Test 1: Grundlegende Funktionalität
 */
void Tests::testBasicFunctionality() {
    std::cout << "\n📋 Kategorie: Basic Functionality" << std::endl;
    
    test("Standard Input Test", []() {
        // TODO: Ersetze durch echte Tests für deine Challenge
        std::vector<int> input = {1, 2, 3, 4, 5};
        int result = Challenge::solve(input);
        
        // Beispiel-Assertion (anpassen)
        int expected = 15; // Summe der positiven Zahlen: 1+2+3+4+5
        assertEqual(expected, result, "Summe von [1,2,3,4,5] sollte 15 sein");
    });

    test("Different Values Test", []() {
        std::vector<int> input = {10, 20, 30};
        int result = Challenge::solve(input);
        
        int expected = 60; // 10+20+30
        assertEqual(expected, result, "Summe von [10,20,30] sollte 60 sein");
    });

    test("Single Element Test", []() {
        std::vector<int> input = {42};
        int result = Challenge::solve(input);
        
        int expected = 42;
        assertEqual(expected, result, "Einzelelement [42] sollte 42 ergeben");
    });
}

/**
 * Test 2: Edge Cases (Grenzfälle)
 */
void Tests::testEdgeCases() {
    std::cout << "\n🔍 Kategorie: Edge Cases" << std::endl;
    
    test("Empty Input Test", []() {
        std::vector<int> emptyInput = {};
        int result = Challenge::solve(emptyInput);
        assertEqual(0, result, "Leerer Vektor sollte 0 ergeben");
    });

    test("Zero Values Test", []() {
        std::vector<int> zeroInput = {0, 0, 0};
        int result = Challenge::solve(zeroInput);
        assertEqual(0, result, "Nur Nullen sollten 0 ergeben");
    });

    test("Negative Numbers Test", []() {
        std::vector<int> negativeInput = {-1, -2, -3};
        int result = Challenge::solve(negativeInput);
        assertEqual(0, result, "Nur negative Zahlen sollten 0 ergeben (da nur positive summiert werden)");
    });

    test("Mixed Positive Negative Test", []() {
        std::vector<int> mixedInput = {-5, 10, -3, 7, -1};
        int result = Challenge::solve(mixedInput);
        assertEqual(17, result, "Gemischte Zahlen: nur positive (10+7) = 17");
    });

    test("Large Single Value Test", []() {
        std::vector<int> largeInput = {1000000};
        int result = Challenge::solve(largeInput);
        assertEqual(1000000, result, "Große Einzelzahl sollte korrekt verarbeitet werden");
    });
}

/**
 * Test 3: Performance Tests
 */
void Tests::testPerformance() {
    std::cout << "\n⚡ Kategorie: Performance" << std::endl;
    
    test("Medium Size Performance", []() {
        std::vector<int> mediumInput = generateLargeInput(1000, 1, 100);
        
        measurePerformance("1000 Elements", [&mediumInput]() {
            int result = Challenge::solve(mediumInput);
            assertTrue(result > 0, "Ergebnis sollte positiv sein bei positiven Eingaben");
        });
    });

    test("Large Size Performance", []() {
        std::vector<int> largeInput = generateLargeInput(10000, 1, 1000);
        
        measurePerformance("10000 Elements", [&largeInput]() {
            int result = Challenge::solve(largeInput);
            assertTrue(result > 0, "Ergebnis sollte positiv sein bei großen positiven Eingaben");
        });
    });

    test("Very Large Size Performance", []() {
        std::vector<int> veryLargeInput = generateLargeInput(100000, 1, 10);
        
        measurePerformance("100000 Elements", [&veryLargeInput]() {
            int result = Challenge::solve(veryLargeInput);
            assertTrue(result > 0, "Ergebnis sollte auch bei sehr großen Eingaben funktionieren");
        });
    });
}

/**
 * Test 4: Invalid Input Handling
 */
void Tests::testInvalidInput() {
    std::cout << "\n🚨 Kategorie: Invalid Input Handling" << std::endl;
    
    test("Extremely Large Values", []() {
        // Test mit sehr großen Werten (sollte durch Validierung abgefangen werden)
        std::vector<int> extremeInput = {2000000000, 1000000000};
        
        // Je nach Implementation könnte dies eine Exception werfen oder spezifisch behandelt werden
        try {
            int result = Challenge::solve(extremeInput);
            // Falls keine Exception geworfen wird, prüfe das Verhalten
            assertTrue(result >= 0, "Bei großen Werten sollte ein gültiges Ergebnis entstehen");
        } catch (const std::exception& e) {
            // Exception ist auch ein gültiges Verhalten
            std::cout << "      📝 Exception bei extremen Werten: " << e.what() << std::endl;
        }
    });
}

/**
 * Test 5: Large Input Stress Test
 */
void Tests::testLargeInput() {
    std::cout << "\n💪 Kategorie: Large Input Stress Test" << std::endl;
    
    test("Stress Test - Random Data", []() {
        std::vector<int> stressInput = generateLargeInput(50000, -1000, 1000);
        
        auto start = std::chrono::high_resolution_clock::now();
        int result = Challenge::solve(stressInput);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "      ⏱️  Stress Test: " << duration.count() << "ms für 50000 Elemente" << std::endl;
        assertTrue(duration.count() < 1000, "Stress Test sollte unter 1 Sekunde dauern");
        assertTrue(result >= 0, "Stress Test sollte gültiges Ergebnis liefern");
    });
}

/**
 * Test 6: Spezielle Cases
 */
void Tests::testSpecialCases() {
    std::cout << "\n🎯 Kategorie: Special Cases" << std::endl;
    
    test("All Same Values", []() {
        std::vector<int> sameValues = {5, 5, 5, 5, 5};
        int result = Challenge::solve(sameValues);
        assertEqual(25, result, "Fünf gleiche Werte (5) sollten 25 ergeben");
    });

    test("Alternating Values", []() {
        std::vector<int> alternating = {1, -1, 2, -2, 3, -3};
        int result = Challenge::solve(alternating);
        assertEqual(6, result, "Alternierende Werte: nur positive (1+2+3) = 6");
    });

    test("Boundary Values", []() {
        std::vector<int> boundary = {0, 1, -1, 1000, -1000};
        int result = Challenge::solve(boundary);
        assertEqual(1001, result, "Grenzwerte: 0 wird ignoriert, positive sind 1+1000 = 1001");
    });
}

// ===== TEST FRAMEWORK IMPLEMENTATION =====

/**
 * Führt einen einzelnen Test aus
 */
template<typename TestFunc>
void Tests::test(const std::string& description, TestFunc testFunction) {
    totalTests++;
    try {
        std::cout << "  🧪 " << std::setw(40) << std::left << description << "... ";
        testFunction();
        std::cout << "✅ BESTANDEN" << std::endl;
        testsPassed++;
    } catch (const std::exception& ex) {
        std::cout << "❌ FEHLGESCHLAGEN" << std::endl;
        std::cout << "      Fehler: " << ex.what() << std::endl;
        failedTests.push_back(description + ": " + ex.what());
    }
}

/**
 * Überprüft Gleichheit von zwei Werten
 */
template<typename T>
void Tests::assertEqual(const T& expected, const T& actual, const std::string& message) {
    if (expected != actual) {
        throw std::runtime_error(message + " - Erwartet: " + std::to_string(expected) + 
                               ", Erhalten: " + std::to_string(actual));
    }
}

/**
 * String-Gleichheit (Spezialisierung)
 */
void Tests::assertEqual(const std::string& expected, const std::string& actual, const std::string& message) {
    if (expected != actual) {
        throw std::runtime_error(message + " - Erwartet: \"" + expected + 
                               "\", Erhalten: \"" + actual + "\"");
    }
}

/**
 * Vektor-Gleichheit (Spezialisierung)
 */
template<typename T>
void Tests::assertEqual(const std::vector<T>& expected, const std::vector<T>& actual, const std::string& message) {
    if (expected.size() != actual.size()) {
        throw std::runtime_error(message + " - Vektorgröße unterschiedlich. Erwartet: " + 
                               std::to_string(expected.size()) + ", Erhalten: " + std::to_string(actual.size()));
    }
    
    for (size_t i = 0; i < expected.size(); i++) {
        if (expected[i] != actual[i]) {
            throw std::runtime_error(message + " - Element an Index " + std::to_string(i) + 
                                   " unterschiedlich. Erwartet: " + std::to_string(expected[i]) + 
                                   ", Erhalten: " + std::to_string(actual[i]));
        }
    }
}

/**
 * Boolean Assertions
 */
void Tests::assertTrue(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message + " - Bedingung war false, aber true erwartet");
    }
}

void Tests::assertFalse(bool condition, const std::string& message) {
    if (condition) {
        throw std::runtime_error(message + " - Bedingung war true, aber false erwartet");
    }
}

/**
 * Exception Testing
 */
template<typename ExceptionType, typename TestFunc>
void Tests::assertThrows(TestFunc testFunction, const std::string& message) {
    try {
        testFunction();
        throw std::runtime_error(message + " - Exception erwartet, aber keine geworfen");
    } catch (const ExceptionType&) {
        // Erwartete Exception - Test bestanden
    } catch (const std::exception& ex) {
        throw std::runtime_error(message + " - Falsche Exception-Type: " + ex.what());
    }
}

/**
 * Gibt die Test-Ergebnisse aus
 */
void Tests::printTestResults() {
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "📊 Test-Ergebnisse:" << std::endl;
    std::cout << "   Bestanden: " << testsPassed << "/" << totalTests << std::endl;
    
    double percentage = totalTests > 0 ? (static_cast<double>(testsPassed) / totalTests) * 100 : 0;
    std::cout << "   Success Rate: " << std::fixed << std::setprecision(1) << percentage << "%" << std::endl;
    
    if (testsPassed == totalTests) {
        std::cout << std::endl << "🎉 Alle Tests bestanden! Challenge bereit zum Upload." << std::endl;
        std::cout << "✅ C++ Template-Challenge erfolgreich implementiert." << std::endl;
    } else {
        std::cout << std::endl << "❌ " << (totalTests - testsPassed) << " Test(s) fehlgeschlagen:" << std::endl;
        for (const auto& failure : failedTests) {
            std::cout << "   • " << failure << std::endl;
        }
        std::cout << std::endl << "🔧 Korrigiere deine Lösung und führe die Tests erneut aus." << std::endl;
        exit(1);
    }
    
    std::cout << std::string(60, '=') << std::endl;
}

/**
 * Generiert große Test-Eingaben
 */
std::vector<int> Tests::generateLargeInput(int size, int minVal, int maxVal) {
    std::vector<int> result;
    result.reserve(size);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minVal, maxVal);
    
    for (int i = 0; i < size; i++) {
        result.push_back(dis(gen));
    }
    
    return result;
}

/**
 * Misst Performance einer Test-Funktion
 */
void Tests::measurePerformance(const std::string& testName, std::function<void()> testFunc) {
    auto start = std::chrono::high_resolution_clock::now();
    
    testFunc();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "      ⏱️  " << testName << ": " << duration.count() << "ms" << std::endl;
}
