/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * Diese Datei dient nur als Referenz.
 */

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

bool isPalindrome(string s) {
    string cleaned;
    for (char c : s) {
        if (isalpha(c)) {
            cleaned += tolower(c);
        }
    }
    string reversed = cleaned;
    reverse(reversed.begin(), reversed.end());
    return cleaned == reversed;
}

int main() {
    vector<string> tests = {"Lagerregal", "Otto", "Hallo", "Reliefpfeiler", "Test"};
    for (string t : tests) {
        cout << "\"" << t << "\" ist Palindrom? " << (isPalindrome(t) ? "true" : "false") << endl;
    }
    return 0;
}