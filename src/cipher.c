// cipher.c
#include "cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void encodeCipher(char *plainText, char *keyText, char *cipherText) {
    int len = strlen(plainText);
    if (len > strlen(keyText)) {
        error("ERROR: Key Too Short");
    }
    for (int i = 0; i < len; i++) {
        int ptChar = plainText[i] == ' ' ? 26 : plainText[i] - 'A';
        int ktChar = keyText[i] == ' ' ? 26 : keyText[i] - 'A';
        if (ptChar < 0 || ptChar > 26 || ktChar < 0 || ktChar > 26) {
            error("ERROR: Bad Character");
        }
        int ctChar = (ptChar + ktChar) % 27;
        cipherText[i] = ctChar == 26 ? ' ' : ctChar + 'A';
    }
    cipherText[len] = '\0';
}

void decodeCipher(char *plainText, char *keyText, char *cipherText) {
    int len = strlen(cipherText);
    if (len > strlen(keyText)) {
        error("ERROR: Key Too Short");
    }
    for (int i = 0; i < len; i++) {
        int ctChar = cipherText[i] == ' ' ? 26 : cipherText[i] - 'A';
        int ktChar = keyText[i] == ' ' ? 26 : keyText[i] - 'A';
        if (ctChar < 0 || ctChar > 26 || ktChar < 0 || ktChar > 26) {
            error("ERROR: Bad Character");
        }
        int ptChar = (ctChar - ktChar + 27) % 27;
        plainText[i] = ptChar == 26 ? ' ' : ptChar + 'A';
    }
    plainText[len] = '\0';
}
