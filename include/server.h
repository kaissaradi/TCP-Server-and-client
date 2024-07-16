// server.h
#ifndef SERVER_H
#define SERVER_H

void handleClient(int connectionFD);
int parseRequest(char *msg, char **msgArray);

#endif // SERVER_H
