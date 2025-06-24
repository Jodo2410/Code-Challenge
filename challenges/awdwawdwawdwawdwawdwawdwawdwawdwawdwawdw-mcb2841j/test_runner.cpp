#include "tests.cpp"
#include "starter.cpp"

int main() {
    std::cout << "🎯 awdwawdwawdwawdwawdwawdwawdwawdwawdwawdw" << std::endl;
    std::cout << "Führe Tests aus...\n" << std::endl;
    
    try {
        Tests::RunAllTests();
    } catch (const std::exception& ex) {
        std::cout << "❌ Fehler: " << ex.what() << std::endl;
        return 1;
    }
    
    std::cout << "\nTests abgeschlossen." << std::endl;
    return 0;
}