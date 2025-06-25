#include <cassert>
#include <string>
#include <iostream>
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
    assert(isPalindrome("Lagerregal") == true);
    assert(isPalindrome("Otto") == true);
    assert(isPalindrome("Hallo") == false);
    assert(isPalindrome("Reliefpfeiler") == true);
    assert(isPalindrome("Test") == false);
    cout << "Alle Tests erfolgreich bestanden." << endl;
    return 0;
}