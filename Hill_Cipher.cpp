#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// Function to find the modulo inverse of a number under modulo 26
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

// Function to calculate the determinant of a 2x2 matrix
int calculateDeterminant(vector<vector<int>> key) {
    return (key[0][0] * key[1][1] - key[0][1] * key[1][0]);
}

// Function to find the inverse of a 2x2 matrix modulo 26
vector<vector<int>> invertMatrix(vector<vector<int>> key) {
    int det = calculateDeterminant(key);
    int detMod26 = (det % 26 + 26) % 26;
    int detInverse = modInverse(detMod26, 26);

    if (detInverse == -1) {
        cout << "Determinant has no inverse modulo 26. Decryption is not possible.\n";
        exit(1);
    }

    vector<vector<int>> inverse(2, vector<int>(2));
    inverse[0][0] = key[1][1];
    inverse[1][1] = key[0][0];
    inverse[0][1] = -key[0][1];
    inverse[1][0] = -key[1][0];

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            inverse[i][j] = ((inverse[i][j] * detInverse) % 26 + 26) % 26;
        }
    }

    return inverse;
}

// Function to perform matrix multiplication modulo 26
string matrixMultiply(vector<vector<int>> matrix, string text) {
    string result = "";
    for (size_t i = 0; i < text.length(); i += 2) {
        int char1 = text[i] - 'A';
        int char2 = text[i + 1] - 'A';
        int enc1 = (matrix[0][0] * char1 + matrix[0][1] * char2) % 26;
        int enc2 = (matrix[1][0] * char1 + matrix[1][1] * char2) % 26;
        result += (enc1 + 'A');
        result += (enc2 + 'A');
    }
    return result;
}

// Hill Cipher Encryption Function
string encrypt(string plaintext, vector<vector<int>> key) {
    if (plaintext.length() % 2 != 0) {
        plaintext += 'X';  // Padding if odd length
    }
    return matrixMultiply(key, plaintext);
}

// Hill Cipher Decryption Function
string decrypt(string ciphertext, vector<vector<int>> key) {
    vector<vector<int>> inverseKey = invertMatrix(key);
    return matrixMultiply(inverseKey, ciphertext);
}

int main() {
    string plaintext;
    cout << "Enter the plaintext (uppercase only, no spaces): ";
    cin >> plaintext;

    vector<vector<int>> key = {{6, 24}, {1, 13}};  // Example 2x2 key matrix
    cout << "Encryption Key Matrix: " << endl;
    for (auto row : key) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    string encryptedText = encrypt(plaintext, key);
    cout << "\nEncrypted Text: " << encryptedText << endl;

    string decryptedText = decrypt(encryptedText, key);
    cout << "Decrypted Text: " << decryptedText << endl;

    return 0;
}
