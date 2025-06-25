#ifndef TESTS_H
#define TESTS_H

#include "BerprfeObEinStringEinPalindromIst.h"
#include <iostream>
#include <stdexcept>
#include <string>

class Tests {
private:
    static int testsPassed;
    static int totalTests;

public:
    static void RunAllTests() {
        std::cout << "🧪 Starte Tests für: Überprüfe, ob ein String ein Palindrom ist" << std::endl;
        std::cout << std::string(50, '=') << std::endl;

        TestExample();

        std::cout << "\n" << testsPassed << "/" << totalTests << " Tests bestanden" << std::endl;
        
        if (testsPassed == totalTests) {
            std::cout << "🎉 Alle Tests bestanden!" << std::endl;
        } else {
            std::cout << "❌ Einige Tests fehlgeschlagen." << std::endl;
            exit(1);
        }
    }

private:
    static void TestExample() {
        Test("Beispiel Test", []() {
            // TODO: Implementiere echte Tests
            AssertEqual(1, 1);
        });
    }

    template<typename TestFunc>
    static void Test(const std::string& description, TestFunc testFunction) {
        totalTests++;
        try {
            testFunction();
            std::cout << "  ✅ " << description << std::endl;
            testsPassed++;
        } catch (const std::exception& ex) {
            std::cout << "  ❌ " << description << ": " << ex.what() << std::endl;
        }
    }

    template<typename T>
    static void AssertEqual(const T& expected, const T& actual) {
        if (expected != actual) {
            throw std::runtime_error("Erwartet: " + std::to_string(expected) + ", Erhalten: " + std::to_string(actual));
        }
    }
};

// Static member definitions
int Tests::testsPassed = 0;
int Tests::totalTests = 0;

#endif // TESTS_H