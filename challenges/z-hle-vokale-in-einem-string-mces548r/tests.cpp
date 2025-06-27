#include <iostream>
#include <string>
using namespace std;

bool test(string input, int expected) {
    int result = countVowels(input);
    if (result == expected) {
        cout << "✔ Test bestanden für: '" << input << "' → Ergebnis: " << result << endl;
        return true;
    } else {
        cout << "✘ Test FEHLGESCHLAGEN für: '" << input << "' → Erwartet: " << expected << ", Bekommen: " << result << endl;
        return false;
    }
}

void RunAllTests() {
    int passed = 0;
    int total = 5;

    passed += test("Hallo Welt", 3);
    passed += test("CPP ist cool", 3);
    passed += test("xyz", 0);
    passed += test("AEIOUaeiou", 10);
    passed += test("123!@#", 0);

    cout << "\nTests bestanden: " << passed << "/" << total << endl;
}

int main() {
    RunAllTests();
    return 0;
}