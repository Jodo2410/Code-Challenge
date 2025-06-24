#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

bool isPalindrome(string s) {
    // Dein Code hier
    
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