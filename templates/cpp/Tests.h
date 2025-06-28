#ifndef TESTS_H
#define TESTS_H

#include "Challenge.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>
#include <iomanip>
#include <chrono>
#include <functional>

/**
 * Test-Framework für C++ Template-Challenges
 * 
 * Template-Version: {{TEMPLATE_VERSION}}
 * Sprache: C++17
 */

class Tests {
private:
    static int testsPassed;
    static int totalTests;
    static std::vector<std::string> failedTests;

public:
    /**
     * Führt alle Tests aus
     */
    static void runAllTests();

private:
    // Test-Kategorien
    static void testBasicFunctionality();
    static void testEdgeCases();
    static void testPerformance();
    static void testInvalidInput();
    static void testLargeInput();
    static void testSpecialCases();

    // Test-Framework Helper Methods
    template<typename TestFunc>
    static void test(const std::string& description, TestFunc testFunction);

    // Assertion Methods
    template<typename T>
    static void assertEqual(const T& expected, const T& actual, const std::string& message = "");
    
    static void assertEqual(const std::string& expected, const std::string& actual, const std::string& message = "");
    
    template<typename T>
    static void assertEqual(const std::vector<T>& expected, const std::vector<T>& actual, const std::string& message = "");
    
    static void assertTrue(bool condition, const std::string& message = "");
    static void assertFalse(bool condition, const std::string& message = "");
    
    template<typename ExceptionType, typename TestFunc>
    static void assertThrows(TestFunc testFunction, const std::string& message = "");
    
    // Utility Methods
    static void printTestResults();
    static std::vector<int> generateLargeInput(int size, int minVal = 1, int maxVal = 1000);
    static void measurePerformance(const std::string& testName, std::function<void()> testFunc);
};

#endif // TESTS_H
