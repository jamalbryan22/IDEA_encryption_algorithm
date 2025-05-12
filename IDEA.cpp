//Jamal Bryan
//CEN4020 - Secure Computing Assignments

#include "IDEA.h"
#include <string>
using namespace std;

//Adds two 4-bit integers and returns the result modulo 16.
int ModuloAdd(const int l, const int r) {
    return (l + r) % 16;
}

//  Multiplies two 4-bit integers, treating zeros as 16, and returns the result modulo 17.
int ModuloMul(const int l, const int r) {
    int l_mod = (l == 0) ? 16 : l; // Treat 0 as 16
    int r_mod = (r == 0) ? 16 : r; // Treat 0 as 16
    return (l_mod * r_mod) % 17;
}

//Performs a bitwise XOR operation on two 4-bit integers.
int ModuloXor(const int l, const int r) {
    return l ^ r;
}

//Performs a circular left shift on a 32-bit integer.
int CircularShiftLeft(const int w, int a) {
    return ((w << a) | (w >> (32 - a))) & 0xFFFFFFFF;
}

//Extracts a specific nibble (4 bits) from a 32-bit word.
int GetNibble(const int w, int nibble) {
    return (w >> (4 * (8 - nibble))) & 0xF;
}

//Converts a hexadecimal character to its integer value.
int ConvertChartoHex(char c) {
    // Debugging: Print the input character
    // cout << "Converting character: '" << c << "'" << endl;

    if (c >= '0' && c <= '9') {
        return c - '0';  // 0 to 9
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;  // a to f
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;  // A to F
    } else {
        cerr << "Error: Invalid hexadecimal character '" << c << "'" << endl;
        exit(1);  // Exit if an invalid character is found
    }
}

//Converts an integer (0-15) to its hexadecimal character.
char ConvertInttoChar(const int x) {
    if (x >= 0 && x <= 9) return '0' + x;
    if (x >= 10 && x <= 15) return 'a' + (x - 10);
    return '\0'; // Invalid integer
}

//Generates the key schedule for the IDEA encryption algorithm.
void IDEA::GenerateKeySchedule(const string &s) {
    // Validate the length of the key
    if (s.size() != 8) { 
        cerr << "Error: Key must be exactly 8 hexadecimal characters, but got " << s.size() << " characters." << endl;
        exit(1); // Exit to signal an error
    }

    // Validate that the key contains only valid hexadecimal characters
    for (char c : s) {
        if (!isxdigit(c)) { 
            cerr << "Error: Key contains invalid character '" << c << "'. Must be 0-9, a-f, or A-F." << endl;
            exit(1);
        }
    }

    keycode = s; // Save the validated key

    // Generate the keyschedule
    cout << "Generating keyschedule for keycode: " << keycode << endl; // Debugging output
    size_t k = 0;  // Key index
    for (size_t i = 1; i < 6; ++i) {
        for (size_t j = 1; j < 7; ++j) {
            // Ensure we wrap around the keycode using modulus
            size_t index = k % keycode.size();

            // DEBUG: Check if we are accessing valid characters
            if (index >= keycode.size()) {
                cerr << "Error: Index " << index << " is out of bounds for keycode of size " << keycode.size() << endl;
                exit(1); // Exit on invalid index access
            }
            
            //cout << "Accessing keycode at index " << index << ": " << keycode[index] << endl; // Debugging
            keyschedule[i][j] = ConvertChartoHex(keycode[index]);

            k++; // Increment the key index
        }
    }
    cout << "Keyschedule successfully generated." << endl;
}

// Print out encrypted secrect 
void IDEA::PrintSecretCode() {
    cout << "Encrypted Secret Code: ";
    for (char c : codeword) {
        cout << c;
    }
    cout << endl;
}

// Executes round 5 of the IDEA encryption algorithm.
void IDEA::Round5() {

    X1 = ModuloMul(X1, keyschedule[1][1]);   // Modify with the correct operation
    X2 = ModuloAdd(X2, keyschedule[1][2]);   // Modify with the correct operation
    X3 = ModuloAdd(X3, keyschedule[1][3]);   // Modify with the correct operation
    X4 = ModuloMul(X4, keyschedule[1][4]);   // Modify with the correct operation
}


//Encrypts the input ASCII string using the IDEA algorithm.
string &IDEA::GenerateSecretCode(const string &s) {
    string modified_s = s;  // Make a copy of s
    secretcode.clear(); // Clear any previous encryption results

    if (modified_s.size() % 4 != 0) {
        size_t padding = 4 - (modified_s.size() % 4);
        modified_s.append(padding, '\0'); // Pad with null characters
    }


    // Process the input string 4 characters (a 32-bit block) at a time
    for (size_t i = 0; i < modified_s.size(); i += 4) {
        // Convert each character to its ASCII value
        X1 = static_cast<int>(modified_s[i]);
        X2 = static_cast<int>(modified_s[i + 1]);
        X3 = static_cast<int>(modified_s[i + 2]);
        X4 = static_cast<int>(modified_s[i + 3]);

        // Debugging output
        cout << "Encrypting block: '" << modified_s[i] << modified_s[i + 1] << modified_s[i + 2] << modified_s[i + 3] << "'" << endl;

        // Perform encryption rounds
        Round(1);
        Round(2);
        Round(3);
        Round(4);
        Round5();

        // Append encrypted nibbles to the secret code
        secretcode.push_back(X1);
        secretcode.push_back(X2);
        secretcode.push_back(X3);
        secretcode.push_back(X4);

    }

    // Convert the secret code to a hex string
    for (int x : secretcode) {
        codeword += ConvertInttoChar(x);
    }
    return codeword;
}

//Executes a single round of the IDEA encryption algorithm.
void IDEA::Round(const int rnum) {
    int K1 = keyschedule[rnum][1];
    int K2 = keyschedule[rnum][2];
    int K3 = keyschedule[rnum][3];
    int K4 = keyschedule[rnum][4];
    int K5 = keyschedule[rnum][5];
    int K6 = keyschedule[rnum][6];

    // Perform operations for the round using K1 to K5
    int temp1 = ModuloMul(X1, K1);
    int temp2 = ModuloAdd(X2, K2);
    int temp3 = ModuloAdd(X3, K3);
    int temp4 = ModuloMul(X4, K4);

    int temp5 = ModuloXor(temp1, temp3);
    int temp6 = ModuloXor(temp2, temp4);

    int temp7 = ModuloMul(temp5, K5);
    int temp8 = ModuloAdd(temp7, temp6);
    int temp9 = ModuloMul(temp8, K6);
    int temp10 = ModuloAdd(temp9, temp7);

    // Update values after the round
    X1 = ModuloXor(temp1, temp9);
    X2 = ModuloXor(temp2, temp10);
    X3 = ModuloXor(temp3, temp9);
    X4 = ModuloXor(temp4, temp10);
}
