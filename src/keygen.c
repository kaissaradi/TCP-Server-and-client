// keygen.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "ERROR: Invalid number of arguments\n");
        return EXIT_FAILURE;
    }

    int keyLength = atoi(argv[1]);
    if (keyLength <= 0) {
        fprintf(stderr, "ERROR: Key length must be a positive integer\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    char *allowedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    char key[keyLength + 2]; // +1 for newline and +1 for null terminator

    for (int i = 0; i < keyLength; i++) {
        key[i] = allowedCharacters[rand() % 27];
    }
    key[keyLength] = '\n';
    key[keyLength + 1] = '\0';

    printf("%s", key);
    return EXIT_SUCCESS;
}
