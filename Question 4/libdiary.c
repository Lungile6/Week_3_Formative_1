#include <string.h>

// Simple Caesar cipher with a fixed key of 4
// This function modifies the data in-place.
void encrypt(char *data) {
    if (!data) return;
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] = data[i] + 4;
    }
}

// Reverses the encryption
void decrypt(char *data) {
    if (!data) return;
    for (int i = 0; data[i] != '\0'; i++) {
        data[i] = data[i] - 4;
    }
}
