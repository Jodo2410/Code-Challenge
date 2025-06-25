/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * Diese Datei dient nur als Referenz.
 */

#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int countVowels(const string& input) {
    int count = 0;
    for (char c : input) {
        c = tolower(c);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    string text;
    cout << "Gib einen Text ein: ";
    getline(cin, text);
    cout << "Anzahl der Vokale: " << countVowels(text) << endl;
    return 0;
}