// cipher.h
#ifndef CIPHER_H
#define CIPHER_H

void encodeCipher(char *plainText, char *keyText, char *cipherText);
void decodeCipher(char *plainText, char *keyText, char *cipherText);
void error(const char *msg);

#endif // CIPHER_H
