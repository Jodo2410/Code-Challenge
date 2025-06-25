#include "Tests.h"

int main() {
    cout << "🎯 Überprüfe, ob ein String ein Palindrom ist" << endl;
    cout << "Führe Tests aus...\n" << endl;
    
    try {
        Tests::RunAllTests();
    } catch (const exception& ex) {
        cout << "❌ Fehler: " << ex.what() << endl;
        return 1;
    }
    
    cout << "\nTests abgeschlossen." << endl;
    return 0;
}