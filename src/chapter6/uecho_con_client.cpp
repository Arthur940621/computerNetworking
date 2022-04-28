#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#define BUF_SIZE 30

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    char message[BUF_SIZE];
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    }
    while (1) {
        cout << "Insert message(q to quit): ";
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        // sendto(sock, message, strlen(message), 0, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
        write(sock, message, strlen(message));
        // sockaddr_in from_adr;
        // socklen_t adr_sz = sizeof(from_adr);
        // int str_len = recvfrom(sock, &message, BUF_SIZE - 1, 0, reinterpret_cast<sockaddr*>(&from_adr), &adr_sz);
        int str_len = read(sock, &message, BUF_SIZE - 1);
        message[str_len] = 0;
        cout << "Message from server: " << message;
    }
    close(sock);
    return 0;
}