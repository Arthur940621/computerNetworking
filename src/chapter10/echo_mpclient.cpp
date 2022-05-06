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

void read_routine(int sock, char *buf) {
    while (1) {
        int str_len = read(sock, buf, BUF_SIZE - 1);
        if (str_len == -1) {
            error_handling("read() error!");
        }
        if (str_len == 0) {
            return;
        }
        buf[str_len] = 0;
        cout << "Message from server: " << buf;
    }
}

void write_routine(int sock, char *buf) {
    while (1) {
        fgets(buf, BUF_SIZE, stdin);
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
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
    
    pid_t pid = fork();
    if (pid == 0) {
        write_routine(sock, message);
    } else {
        read_routine(sock, message);
    }
    close(sock);
    return 0;
}