#include <iostream>
#include <string>
using namespace std;

// Caesar Cipher Encryption Function
string encrypt(string text, int shift) {
    string result = "";

    for (char c : text) {
        if (isupper(c)) {
            result += char(int(c + shift - 'A') % 26 + 'A');
        } else if (islower(c)) {
            result += char(int(c + shift - 'a') % 26 + 'a');
        } else {
            result += c;  // Non-alphabet characters are kept unchanged
        }
    }
    return result;
}

// Caesar Cipher Decryption Function
string decrypt(string text, int shift) {
    return encrypt(text, 26 - (shift % 26));  // Decrypt by shifting backward
}

int main() {
    string text;
    int shift;

    cout << "Enter the text: ";
    getline(cin, text);

    cout << "Enter the shift value: ";
    cin >> shift;

    string encryptedText = encrypt(text, shift);
    cout << "\nEncrypted Text: " << encryptedText << endl;

    string decryptedText = decrypt(encryptedText, shift);
    cout << "Decrypted Text: " << decryptedText << endl;

    return 0;
}
