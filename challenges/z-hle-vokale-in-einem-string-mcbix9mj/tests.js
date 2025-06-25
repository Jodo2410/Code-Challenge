#include <cassert>

void runTests() {
    assert(countVowels("Hallo Welt") == 3);
    assert(countVowels("OpenAI ist großartig!") == 8);
    assert(countVowels("xyz") == 0);
    assert(countVowels("AEIOUaeiou") == 10);
    assert(countVowels("") == 0);
    cout << "Alle Tests bestanden!" << endl;
}