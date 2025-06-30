#include "Tests.h"
#include <random>

/**
 * Test Implementation - Sum of Positive Numbers
 * Template-Version: 2.0
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
    std::cout << "Challenge: Sum of Positive Numbers" << std::endl;
    // std::cout << "Compiler: " << __VERSION__ << std::endl; // __VERSION__ is not standard on all compilers
    std::cout << "C++ Standard: " << __cplusplus << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    // Alle Test-Kategorien ausführen
    testBasicFunctionality();
    testEdgeCases();
    testPerformance();
    testInvalidInput();
    testLargeInput();
    testSpecialCases();
    testHelperFunctions();

    // Test-Ergebnisse ausgeben
    printTestResults();
}

/**
 * Test 1: Grundlegende Funktionalität
 */
void Tests::testBasicFunctionality() {
    std::cout << "\n📋 Kategorie: Basic Functionality" << std::endl;
    
    test("Standard Positive Numbers Test", []() {
        std::vector<int> input = {1, 2, 3, 4, 5};
        int result = Challenge::solve(input);
        
        int expected = 15; // 1+2+3+4+5 = 15
        assertEqual(expected, result, "Summe von [1,2,3,4,5] sollte 15 sein");
    });

    test("Mixed Positive Negative Test", []() {
        std::vector<int> input = {1, -4, 7, 12};
        int result = Challenge::solve(input);
        
        int expected = 20; // 1+7+12 = 20 (ignoriere -4)
        assertEqual(expected, result, "Summe von [1,-4,7,12] sollte 20 sein");
    });

    test("Single Positive Element Test", []() {
        std::vector<int> input = {42};
        int result = Challenge::solve(input);
        
        int expected = 42;
        assertEqual(expected, result, "Einzelelement [42] sollte 42 ergeben");
    });

    test("Two Elements Test", []() {
        std::vector<int> input = {10, 20};
        int result = Challenge::solve(input);
        
        int expected = 30; // 10+20 = 30
        assertEqual(expected, result, "Summe von [10,20] sollte 30 sein");
    });

    test("Large Numbers Test", []() {
        std::vector<int> input = {1000, 2000, 3000};
        int result = Challenge::solve(input);
        
        int expected = 6000; // 1000+2000+3000 = 6000
        assertEqual(expected, result, "Summe großer Zahlen sollte korrekt sein");
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

    test("Only Zero Values Test", []() {
        std::vector<int> zeroInput = {0, 0, 0};
        int result = Challenge::solve(zeroInput);
        assertEqual(0, result, "Nur Nullen sollten 0 ergeben");
    });

    test("Only Negative Numbers Test", []() {
        std::vector<int> negativeInput = {-1, -2, -3, -10, -100};
        int result = Challenge::solve(negativeInput);
        assertEqual(0, result, "Nur negative Zahlen sollten 0 ergeben");
    });

    test("Mixed with Zero Test", []() {
        std::vector<int> mixedInput = {-5, 0, 10, -3, 0, 7, -1};
        int result = Challenge::solve(mixedInput);
        assertEqual(17, result, "Gemischte Zahlen mit Nullen: nur positive (10+7) = 17");
    });

    test("Single Zero Test", []() {
        std::vector<int> zeroInput = {0};
        int result = Challenge::solve(zeroInput);
        assertEqual(0, result, "Einzelne Null sollte 0 ergeben");
    });

    test("Single Negative Test", []() {
        std::vector<int> negativeInput = {-42};
        int result = Challenge::solve(negativeInput);
        assertEqual(0, result, "Einzelne negative Zahl sollte 0 ergeben");
    });

    test("Very Large Single Value Test", []() {
        std::vector<int> largeInput = {1000000};
        int result = Challenge::solve(largeInput);
        assertEqual(1000000, result, "Große Einzelzahl sollte korrekt verarbeitet werden");
    });

    test("Alternating Pattern Test", []() {
        std::vector<int> alternating = {1, -1, 2, -2, 3, -3, 4, -4};
        int result = Challenge::solve(alternating);
        assertEqual(10, result, "Alternierendes Muster: 1+2+3+4 = 10");
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
            
            // Vergleiche mit erwarteter Summe
            int expectedMin = 1000; // mindestens 1000 (wenn alle 1 sind)
            int expectedMax = 100000; // maximal 100000 (wenn alle 100 sind)
            assertTrue(result >= expectedMin && result <= expectedMax, 
                      "Ergebnis sollte in erwartetem Bereich liegen");
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
            
            // Bei 100000 Elementen mit Werten 1-10, sollte Ergebnis zwischen 100000 und 1000000 liegen
            assertTrue(result >= 100000 && result <= 1000000, 
                      "Ergebnis sollte in plausiblem Bereich liegen");
        });
    });

    test("Performance with Mixed Values", []() {
        std::vector<int> mixedInput = generateLargeInput(50000, -100, 100);
        
        measurePerformance("50000 Mixed Elements", [&mixedInput]() {
            int result = Challenge::solve(mixedInput);
            assertTrue(result >= 0, "Ergebnis sollte nie negativ sein");
        });
    });
}

/**
 * Test 4: Invalid Input Handling
 */
void Tests::testInvalidInput() {
    std::cout << "\n🚨 Kategorie: Invalid Input Handling" << std::endl;
    
    test("Extremely Large Values", []() {
        // Test mit sehr großen Werten (INT_MAX Grenze)
        std::vector<int> extremeInput = {2000000000, 1000000000};
        
        try {
            int result = Challenge::solve(extremeInput);
            // Falls keine Exception geworfen wird, prüfe das Verhalten
            assertTrue(result >= 0, "Bei großen Werten sollte ein gültiges Ergebnis entstehen");
            std::cout << "      📝 Große Werte verarbeitet: " << result << std::endl;
        } catch (const std::exception& e) {
            // Exception ist auch ein gültiges Verhalten bei Overflow
            std::cout << "      📝 Exception bei extremen Werten: " << e.what() << std::endl;
        }
    });

    test("Maximum Integer Values", []() {
        std::vector<int> maxInput = {INT_MAX};
        
        try {
            int result = Challenge::solve(maxInput);
            assertEqual(INT_MAX, result, "INT_MAX sollte korrekt verarbeitet werden");
        } catch (const std::exception& e) {
            std::cout << "      📝 Exception bei INT_MAX: " << e.what() << std::endl;
        }
    });

    test("Minimum Integer Values", []() {
        std::vector<int> minInput = {INT_MIN, INT_MIN + 1};
        int result = Challenge::solve(minInput);
        assertEqual(0, result, "Sehr negative Werte sollten ignoriert werden");
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

    test("Stress Test - All Positive", []() {
        std::vector<int> allPositive = generateLargeInput(30000, 1, 100);
        
        auto start = std::chrono::high_resolution_clock::now();
        int result = Challenge::solve(allPositive);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "      ⏱️  All Positive Test: " << duration.count() << "ms" << std::endl;
        assertTrue(result > 0, "Alle positiven Zahlen sollten positive Summe ergeben");
    });

    test("Stress Test - All Negative", []() {
        std::vector<int> allNegative = generateLargeInput(20000, -1000, -1);
        
        auto start = std::chrono::high_resolution_clock::now();
        int result = Challenge::solve(allNegative);
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cout << "      ⏱️  All Negative Test: " << duration.count() << "ms" << std::endl;
        assertEqual(0, result, "Alle negativen Zahlen sollten 0 ergeben");
    });
}

/**
 * Test 6: Spezielle Cases
 */
void Tests::testSpecialCases() {
    std::cout << "\n🎯 Kategorie: Special Cases" << std::endl;
    
    test("All Same Positive Values", []() {
        std::vector<int> sameValues = {5, 5, 5, 5, 5};
        int result = Challenge::solve(sameValues);
        assertEqual(25, result, "Fünf gleiche positive Werte (5) sollten 25 ergeben");
    });

    test("All Same Negative Values", []() {
        std::vector<int> sameNegative = {-7, -7, -7, -7};
        int result = Challenge::solve(sameNegative);
        assertEqual(0, result, "Gleiche negative Werte sollten 0 ergeben");
    });

    test("Ascending Order Test", []() {
        std::vector<int> ascending = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int result = Challenge::solve(ascending);
        assertEqual(55, result, "Aufsteigende Reihenfolge 1-10 sollte 55 ergeben");
    });

    test("Descending Order Test", []() {
        std::vector<int> descending = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        int result = Challenge::solve(descending);
        assertEqual(55, result, "Absteigende Reihenfolge sollte gleiche Summe ergeben");
    });

    test("Boundary Values Test", []() {
        std::vector<int> boundary = {0, 1, -1, 1000, -1000, 999, -999};
        int result = Challenge::solve(boundary);
        assertEqual(2000, result, "Grenzwerte: positive sind 1+1000+999 = 2000");
    });

    test("One Positive Among Negatives", []() {
        std::vector<int> onePositive = {-10, -20, -30, 42, -40, -50};
        int result = Challenge::solve(onePositive);
        assertEqual(42, result, "Eine positive Zahl unter negativen sollte diese Zahl ergeben");
    });

    test("Mathematical Sequence Test", []() {
        // Fibonacci-ähnliche positive Zahlen
        std::vector<int> fibonacci = {1, 1, 2, 3, 5, 8, 13, 21};
        int result = Challenge::solve(fibonacci);
        assertEqual(54, result, "Fibonacci-Sequenz sollte korrekt summiert werden");
    });

    test("Powers of Two Test", []() {
        std::vector<int> powersOfTwo = {1, 2, 4, 8, 16, 32};
        int result = Challenge::solve(powersOfTwo);
        assertEqual(63, result, "Potenzen von 2 sollten 63 ergeben");
    });
}

/**
 * Test 7: Helper Functions Testing
 */
void Tests::testHelperFunctions() {
    std::cout << "\n🔧 Kategorie: Helper Functions" << std::endl;
    
    test("isPositive Function Test", []() {
        assertTrue(Challenge::isPositive(1), "1 sollte positiv sein");
        assertTrue(Challenge::isPositive(100), "100 sollte positiv sein");
        assertFalse(Challenge::isPositive(0), "0 sollte nicht positiv sein");
        assertFalse(Challenge::isPositive(-1), "-1 sollte nicht positiv sein");
        assertFalse(Challenge::isPositive(-100), "-100 sollte nicht positiv sein");
    });

    test("isValid Function Test", []() {
        assertTrue(Challenge::isValid(0), "0 sollte gültiges Ergebnis sein");
        assertTrue(Challenge::isValid(42), "42 sollte gültiges Ergebnis sein");
        assertTrue(Challenge::isValid(1000000), "1000000 sollte gültiges Ergebnis sein");
        assertFalse(Challenge::isValid(-1), "-1 sollte ungültiges Ergebnis sein");
        assertFalse(Challenge::isValid(-100), "-100 sollte ungültiges Ergebnis sein");
    });

    test("countPositive Function Test", []() {
        std::vector<int> mixed = {1, -2, 3, -4, 5, 0, 6};
        int count = Challenge::countPositive(mixed);
        assertEqual(4, count, "Anzahl positiver Zahlen sollte 4 sein (1,3,5,6)");
        
        std::vector<int> allNegative = {-1, -2, -3};
        int countNeg = Challenge::countPositive(allNegative);
        assertEqual(0, countNeg, "Keine positiven Zahlen sollten 0 ergeben");
        
        std::vector<int> empty = {};
        int countEmpty = Challenge::countPositive(empty);
        assertEqual(0, countEmpty, "Leerer Vektor sollte 0 positive Zahlen haben");
    });

    test("maxPositive Function Test", []() {
        std::vector<int> mixed = {1, -10, 5, -2, 3, 0, 7};
        int maxPos = Challenge::maxPositive(mixed);
        assertEqual(7, maxPos, "Größte positive Zahl sollte 7 sein");
        
        std::vector<int> allNegative = {-1, -2, -3};
        int maxNeg = Challenge::maxPositive(allNegative);
        assertEqual(0, maxNeg, "Keine positiven Zahlen sollten 0 als Maximum ergeben");
        
        std::vector<int> singlePositive = {42};
        int maxSingle = Challenge::maxPositive(singlePositive);
        assertEqual(42, maxSingle, "Einzelne positive Zahl sollte sich selbst als Maximum haben");
    });

    test("Helper Functions Integration Test", []() {
        std::vector<int> testData = {10, -5, 20, -10, 30, 0, 15};
        
        // Test Integration aller Helper-Funktionen
        int sum = Challenge::solve(testData);
        int count = Challenge::countPositive(testData);
        int maxVal = Challenge::maxPositive(testData);
        bool valid = Challenge::isValid(sum);
        
        assertEqual(75, sum, "Summe sollte 75 sein (10+20+30+15)");
        assertEqual(4, count, "Anzahl positive sollte 4 sein");
        assertEqual(30, maxVal, "Maximum sollte 30 sein");
        assertTrue(valid, "Ergebnis sollte gültig sein");
        
        // Konsistenz-Check
        assertTrue(maxVal <= sum, "Maximum sollte nie größer als Summe sein");
        assertTrue(count > 0, "Anzahl sollte positiv sein bei positiven Zahlen");
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
        std::cout << "  🧪 " << std::setw(50) << std::left << description << "... ";
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
        std::cout << "✅ Sum of Positive Numbers Challenge erfolgreich implementiert." << std::endl;
        std::cout << std::endl << "📈 Test-Statistiken:" << std::endl;
        std::cout << "   • Basic Functionality: ✅" << std::endl;
        std::cout << "   • Edge Cases: ✅" << std::endl;
        std::cout << "   • Performance: ✅" << std::endl;
        std::cout << "   • Helper Functions: ✅" << std::endl;
        std::cout << "   • Stress Tests: ✅" << std::endl;
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
    
    // Performance-Bewertung
    if (duration.count() < 10) {
        std::cout << "      🚀 Excellent Performance!" << std::endl;
    } else if (duration.count() < 100) {
        std::cout << "      ✅ Good Performance!" << std::endl;
    } else if (duration.count() < 500) {
        std::cout << "      👍 Acceptable Performance" << std::endl;
    } else {
        std::cout << "      ⚠️  Consider optimization" << std::endl;
    }
}