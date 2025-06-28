#include "Tests.h"
#include "Challenge.h"
#include <iostream>
#include <chrono>
#include <exception>

/**
 * C++ Template Test Runner (Separate .h/.cpp Files)
 * 
 * Führt alle Tests für die Challenge aus
 * Template-Version: {{TEMPLATE_VERSION}}
 */

void printWelcomeMessage() {
    std::cout << "🚀 C++ Template Challenge Test Runner" << std::endl;
    std::cout << "Template-Version: {{TEMPLATE_VERSION}}" << std::endl;
    std::cout << "Datei-Struktur: Separate .h/.cpp Dateien" << std::endl;
    std::cout << "Kompiliert mit: " << __VERSION__ << std::endl;
    std::cout << "C++ Standard: ";
    
    #if __cplusplus >= 202002L
        std::cout << "C++20 oder höher";
    #elif __cplusplus >= 201703L
        std::cout << "C++17";
    #elif __cplusplus >= 201402L
        std::cout << "C++14";
    #elif __cplusplus >= 201103L
        std::cout << "C++11";
    #else
        std::cout << "Pre-C++11";
    #endif
    
    std::cout << " (Code: " << __cplusplus << ")" << std::endl;
    std::cout << std::endl;
}

void printSystemInfo() {
    std::cout << "💻 System Information:" << std::endl;
    std::cout << "   Compiler: " << 
    #ifdef __GNUC__
        "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__
    #elif defined(__clang__)
        "Clang " << __clang_major__ << "." << __clang_minor__ << "." << __clang_patchlevel__
    #elif defined(_MSC_VER)
        "MSVC " << _MSC_VER
    #else
        "Unknown"
    #endif
    << std::endl;
    
    std::cout << "   Platform: ";
    #ifdef _WIN32
        std::cout << "Windows";
    #elif defined(__linux__)
        std::cout << "Linux";
    #elif defined(__APPLE__)
        std::cout << "macOS";
    #else
        std::cout << "Unknown";
    #endif
    std::cout << std::endl;
    
    std::cout << "   Architecture: ";
    #ifdef _WIN64
        std::cout << "x64";
    #elif defined(_WIN32)
        std::cout << "x86";
    #elif defined(__x86_64__)
        std::cout << "x86_64";
    #elif defined(__i386__)
        std::cout << "i386";
    #elif defined(__arm__)
        std::cout << "ARM";
    #elif defined(__aarch64__)
        std::cout << "ARM64";
    #else
        std::cout << "Unknown";
    #endif
    std::cout << std::endl;
    
    #ifdef DEBUG
        std::cout << "   Build Type: Debug" << std::endl;
    #else
        std::cout << "   Build Type: Release" << std::endl;
    #endif
    
    std::cout << std::endl;
}

void runQuickSelfTest() {
    std::cout << "🔧 Template Self-Test:" << std::endl;
    
    try {
        // Test 1: Basic Challenge Function verfügbar
        std::vector<int> testInput = {1, 2, 3};
        int result = Challenge::solve(testInput);
        std::cout << "   ✅ Challenge::solve() funktioniert (Ergebnis: " << result << ")" << std::endl;
        
        // Test 2: Hilfsfunktionen verfügbar
        bool validResult = Challenge::isValid(42);
        std::cout << "   ✅ Challenge::isValid() funktioniert (42 ist " << (validResult ? "gültig" : "ungültig") << ")" << std::endl;
        
        // Test 3: Edge Case Test
        std::vector<int> emptyInput = {};
        int emptyResult = Challenge::solve(emptyInput);
        std::cout << "   ✅ Edge Case (leer) funktioniert (Ergebnis: " << emptyResult << ")" << std::endl;
        
    } catch (const std::exception& ex) {
        std::cout << "   ⚠️  Self-Test Warning: " << ex.what() << std::endl;
        std::cout << "   💡 Hinweis: Challenge möglicherweise noch nicht vollständig implementiert" << std::endl;
    }
    
    std::cout << std::endl;
}

int main() {
    try {
        // Welcome & System Info
        printWelcomeMessage();
        printSystemInfo();
        
        // Quick Self-Test
        runQuickSelfTest();
        
        // Zeitmessung für gesamte Test-Suite
        std::cout << "⏱️  Starte Zeitmessung..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        
        // Alle Tests ausführen
        Tests::runAllTests();
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        // Erfolgreiche Ausführung
        std::cout << std::endl;
        std::cout << "⏱️  Gesamte Test-Dauer: " << duration.count() << "ms" << std::endl;
        std::cout << "🎯 C++ Template-Challenge erfolgreich abgeschlossen!" << std::endl;
        std::cout << std::endl;
        
        // Performance-Bewertung
        if (duration.count() < 100) {
            std::cout << "🚀 Excellent Performance! (< 100ms)" << std::endl;
        } else if (duration.count() < 500) {
            std::cout << "✅ Good Performance! (< 500ms)" << std::endl;
        } else if (duration.count() < 1000) {
            std::cout << "👍 Acceptable Performance (< 1s)" << std::endl;
        } else {
            std::cout << "⚠️  Consider
