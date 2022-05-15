#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define BUF_SIZE 1024

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    char message[BUF_SIZE];
    memset(&message, 0, sizeof(message));
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error!");
    }
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("connect() error!");
    } else {
        cout << "Connected......" << endl;
    }
    FILE* readfp = fdopen(sock, "r");
    FILE* writefp = fdopen(sock, "w");
    while (1) {
        cout << "Input message(Q to quit): ";
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        fputs(message, writefp);
        fflush(writefp);
        fgets(message, BUF_SIZE, readfp);
        cout << "Message from server: " << message;
    }
    fclose(writefp);
    fclose(readfp);
    return 0;
}