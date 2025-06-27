#include "Tests.h"

int main() {
    std::cout << "🎯 Zähle Vokale in einem String" << std::endl;
    std::cout << "Führe Tests aus...\n" << std::endl;
    
    try {
        Tests::RunAllTests();
    } catch (const std::exception& ex) {
        std::cerr << "❌ Fehler: " << ex.what() << std::endl;
        return 1;
    }
    
    return 0;
}