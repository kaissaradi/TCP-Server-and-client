// otc_d.c
#include "cipher.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <time.h>

int parseRequest(char *msg, char **msgArray) {
    msgArray[0] = strtok(msg, " ");   // Store user name
    char *key = strtok(NULL, "\0");
    msgArray[1] = key;                // Store message
    if (key == NULL) {
        return 0; // GET request
    }

    int count = strlen(key);
    char charCount[100];
    sprintf(charCount, "%d", count);
    msgArray[2] = charCount; // POST request
    return 1;
}

void handleClient(int connectionFD) {
    int bufferSize = 200000;
    char buffer[bufferSize];
    char cipherMessage[bufferSize];
    int charsRead;

    memset(buffer, '\0', bufferSize);
    charsRead = recv(connectionFD, buffer, bufferSize - 1, 0); // Read the client's message from the socket
    if (charsRead < 0) {
        error("ERROR reading from socket");
    }

    char *msgArray[3] = {NULL}; // Create an array of strings to hold the message from the buffer
    int req = parseRequest(buffer, msgArray); // Parse buffer request

    char path[1080];
    sprintf(path, "./%s/", msgArray[0]); // Path to user folder

    if (req == 0) { // GET request
        DIR *dir;
        struct dirent *entry;
        struct stat stats;
        char oldestCipher[1080];
        time_t oldestFile = INT_MAX;

        dir = opendir(path);
        if (!dir) {
            error("ERROR: User not found");
        }

        while ((entry = readdir(dir))) {
            char filePath[1080];
            sprintf(filePath, "%s%s", path, entry->d_name);
            stat(filePath, &stats);

            if (entry->d_name[0] == 'c') {
                if (stats.st_mtime < oldestFile) {
                    sprintf(oldestCipher, "%s", entry->d_name);
                    oldestFile = stats.st_mtime;
                }
            }
        }

        closedir(dir);

        if (oldestFile == INT_MAX) {
            error("ERROR: User has no files");
        }

        char fullPath[1080];
        sprintf(fullPath, "%s%s", path, oldestCipher);
        FILE *file = fopen(fullPath, "r");
        if (!file) {
            sprintf(cipherMessage, "ERROR");
            send(connectionFD, cipherMessage, bufferSize, 0);
        } else {
            fgets(cipherMessage, bufferSize, file);
            fclose(file);
            send(connectionFD, cipherMessage, bufferSize, 0);
        }
    } else { // POST request
        mkdir(path, 0777); // Create a directory for the user
        char fullPath[1080];
        sprintf(fullPath, "%scipherText", path);
        FILE *file = fopen(fullPath, "w");
        if (!file) {
            error("ERROR opening file");
        }

        fprintf(file, "%s\n", msgArray[1]);
        fclose(file);

        char cwd[4097];
        getcwd(cwd, sizeof(cwd));
        sprintf(buffer, "%s/%s/cipherText", cwd, msgArray[0]);
        send(connectionFD, "\n", 1, 0);
    }

    close(connectionFD);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "ERROR: Too few arguments\n");
        exit(EXIT_FAILURE);
    }

    int portNumber = atoi(argv[1]);
    int listenSocketFD, connectionFD;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLen = sizeof(clientAddress);

    listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocketFD < 0) {
        error("ERROR opening socket");
    }

    memset((char *)&serverAddress, '\0', sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portNumber);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        error("ERROR on binding");
    }

    listen(listenSocketFD, 5);

    while (1) {
        connectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &clientLen);
        if (connectionFD < 0) {
            error("ERROR on accept");
        }

        pid_t pid = fork();
        if (pid < 0) {
            error("ERROR on fork");
        }

        if (pid == 0) {
            close(listenSocketFD);
            handleClient(connectionFD);
            exit(0);
        } else {
            close(connectionFD);
        }
    }

    close(listenSocketFD);
        return 0;
}