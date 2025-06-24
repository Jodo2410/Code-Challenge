/* 
 * 📖 REFERENZ-LÖSUNG - NUR ZUM ANSCHAUEN!
 * Diese Datei dient nur als Referenz.
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

bool isPalindrome(string s) {
    // String bereinigen: nur Buchstaben und Zahlen, alles in Kleinbuchstaben
    string cleaned = "";
    for (char c : s) {
        if (isalnum(c)) {
            cleaned += tolower(c);
        }
    }
    
    // Zwei-Zeiger-Ansatz
    int left = 0;
    int right = cleaned.length() - 1;
    
    while (left < right) {
        if (cleaned[left] != cleaned[right]) {
            return false;
        }
        left++;
        right--;
    }
    
    return true;
}

int main() {
    // Test cases
    cout << boolalpha;
    cout << isPalindrome("racecar") << endl;  // Expected: true
    cout << isPalindrome("A man a plan a canal Panama") << endl;  // Expected: true
    cout << isPalindrome("race a car") << endl;  // Expected: false
    cout << isPalindrome("hello") << endl;  // Expected: false
    
    return 0;
}