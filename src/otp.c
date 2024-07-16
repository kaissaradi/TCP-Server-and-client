// otp.c
#include "cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "ERROR: Too Few Arguments\n");
        exit(EXIT_FAILURE);
    }
    if (argc > 6) {
        fprintf(stderr, "ERROR: Too Many Arguments\n");
        exit(EXIT_FAILURE);
    }

    int socketFD, portNumber, charsWritten, charsRead;
    struct sockaddr_in serverAddress;
    struct hostent *serverHostInfo;
    int bufferSize = 200000;
    char buffer[bufferSize];
    char user[4098], plainText[bufferSize], keyText[bufferSize], cipherText[bufferSize];

    // Determine request type and set port number
    int isPost = 0;
    if (strcmp(argv[1], "post") == 0) {
        portNumber = atoi(argv[5]);
        isPost = 1;
    } else if (strcmp(argv[1], "get") == 0) {
        portNumber = atoi(argv[4]);
    } else {
        error("ERROR: Invalid First Argument");
    }

    // Set up server address struct
    memset((char *)&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNumber);
    serverHostInfo = gethostbyname("localhost");
    if (serverHostInfo == NULL) {
        error("CLIENT: ERROR, no such host");
    }
    memcpy((char *)&serverAddress.sin_addr.s_addr, (char *)serverHostInfo->h_addr, serverHostInfo->h_length);

    // Create socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD < 0) {
        error("CLIENT: ERROR opening socket");
    }

    // Connect to server
    if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        error("CLIENT: ERROR connecting");
    }

    strcpy(user, argv[2]);
    memset(buffer, '\0', sizeof(buffer));

    if (isPost) {
        // Read plaintext file
        FILE *file = fopen(argv[3], "r");
        if (file == NULL) {
            error("ERROR opening file");
        }
        fread(plainText, sizeof(char), bufferSize, file);
        plainText[strcspn(plainText, "\n")] = '\0';
        fclose(file);

        // Read key file
        file = fopen(argv[4], "r");
        if (file == NULL) {
            error("ERROR opening file");
        }
        fread(keyText, sizeof(char), bufferSize, file);
        keyText[strcspn(keyText, "\n")] = '\0';
        fclose(file);

        // Encode cipher
        encodeCipher(plainText, keyText, cipherText);
        sprintf(buffer, "%s %s", user, cipherText);
    } else {
        strcpy(buffer, user);
    }

    // Send message to server
    charsWritten = send(socketFD, buffer, strlen(buffer), 0);
    if (charsWritten < 0) {
        error("CLIENT: ERROR writing to socket");
    }
    if (charsWritten < strlen(buffer)) {
        printf("CLIENT: WARNING: Not all data written to socket!\n");
    }

    // Receive response from server
    memset(buffer, '\0', sizeof(buffer));
    charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
    if (charsRead < 0) {
        error("CLIENT: ERROR reading from socket");
    }

    if (isPost) {
        printf("%s", buffer);
    } else {
        // Read key file
        FILE *file = fopen(argv[3], "r");
        if (file == NULL) {
            error("ERROR opening file");
        }
        fread(keyText, sizeof(char), bufferSize, file);
        keyText[strcspn(keyText, "\n")] = '\0';
        buffer[strcspn(buffer, "\n")] = '\0';
        fclose(file);

        if (strcmp(buffer, "ERROR") == 0) {
            error("CLIENT: ERROR no message for user");
        } else {
            decodeCipher(plainText, keyText, buffer);
            printf("%s\n", plainText);
        }
    }

    close(socketFD);
    return 0;
}
