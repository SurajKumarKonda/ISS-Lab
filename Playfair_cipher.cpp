#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_set>

using namespace std;

const int MATRIX_SIZE = 5;
vector<vector<char>> keyMatrix(MATRIX_SIZE, vector<char>(MATRIX_SIZE));

// Function to prepare the key matrix
void generateKeyMatrix(string key) {
    unordered_set<char> used;
    key.erase(remove(key.begin(), key.end(), 'J'), key.end());  // Treat 'I' and 'J' as the same
    key += "ABCDEFGHIKLMNOPQRSTUVWXYZ";

    int index = 0;
    for (char c : key) {
        c = toupper(c);
        if (used.find(c) == used.end() && isalpha(c)) {
            keyMatrix[index / MATRIX_SIZE][index % MATRIX_SIZE] = c;
            used.insert(c);
            index++;
        }
    }
}

// Helper function to locate a letter's position in the key matrix
pair<int, int> findPosition(char letter) {
    for (int row = 0; row < MATRIX_SIZE; ++row) {
        for (int col = 0; col < MATRIX_SIZE; ++col) {
            if (keyMatrix[row][col] == letter) {
                return {row, col};
            }
        }
    }
    return {-1, -1};  // This should never occur
}

// Helper function to format the plaintext into pairs
string preparePlainText(string text) {
    string formattedText;
    text.erase(remove_if(text.begin(), text.end(), ::isspace), text.end());
    for (size_t i = 0; i < text.length(); i++) {
        formattedText += toupper(text[i]);
        if (i < text.length() - 1 && text[i] == text[i + 1]) {
            formattedText += 'X';  // Insert filler 'X' for repeated letters
        }
    }
    if (formattedText.length() % 2 != 0) {
        formattedText += 'X';  // Padding if odd length
    }
    return formattedText;
}

// Playfair encryption function
string encrypt(string plaintext) {
    plaintext = preparePlainText(plaintext);
    string encryptedText;

    for (size_t i = 0; i < plaintext.length(); i += 2) {
        char first = plaintext[i];
        char second = plaintext[i + 1];

        auto [row1, col1] = findPosition(first);
        auto [row2, col2] = findPosition(second);

        if (row1 == row2) {  // Same row
            encryptedText += keyMatrix[row1][(col1 + 1) % MATRIX_SIZE];
            encryptedText += keyMatrix[row2][(col2 + 1) % MATRIX_SIZE];
        } else if (col1 == col2) {  // Same column
            encryptedText += keyMatrix[(row1 + 1) % MATRIX_SIZE][col1];
            encryptedText += keyMatrix[(row2 + 1) % MATRIX_SIZE][col2];
        } else {  // Rectangle swap
            encryptedText += keyMatrix[row1][col2];
            encryptedText += keyMatrix[row2][col1];
        }
    }
    return encryptedText;
}

// Playfair decryption function
string decrypt(string ciphertext) {
    string decryptedText;

    for (size_t i = 0; i < ciphertext.length(); i += 2) {
        char first = ciphertext[i];
        char second = ciphertext[i + 1];

        auto [row1, col1] = findPosition(first);
        auto [row2, col2] = findPosition(second);

        if (row1 == row2) {  // Same row
            decryptedText += keyMatrix[row1][(col1 - 1 + MATRIX_SIZE) % MATRIX_SIZE];
            decryptedText += keyMatrix[row2][(col2 - 1 + MATRIX_SIZE) % MATRIX_SIZE];
        } else if (col1 == col2) {  // Same column
            decryptedText += keyMatrix[(row1 - 1 + MATRIX_SIZE) % MATRIX_SIZE][col1];
            decryptedText += keyMatrix[(row2 - 1 + MATRIX_SIZE) % MATRIX_SIZE][col2];
        } else {  // Rectangle swap
            decryptedText += keyMatrix[row1][col2];
            decryptedText += keyMatrix[row2][col1];
        }
    }
    return decryptedText;
}

int main() {
    string key, plaintext;
    cout << "Enter the key (no spaces): ";
    cin >> key;

    generateKeyMatrix(key);

    cout << "Generated Key Matrix: \n";
    for (auto row : keyMatrix) {
        for (char c : row) {
            cout << c << " ";
        }
        cout << endl;
    }

    cout << "\nEnter the plaintext: ";
    cin.ignore();
    getline(cin, plaintext);

    string encryptedText = encrypt(plaintext);
    cout << "\nEncrypted Text: " << encryptedText << endl;

    string decryptedText = decrypt(encryptedText);
    cout << "Decrypted Text: " << decryptedText << endl;

    return 0;
}
