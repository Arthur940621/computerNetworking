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
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }
    int serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    while (1) {
        sockaddr_in clnt_adr;
        socklen_t clnt_adr_sz = sizeof(clnt_adr);
        int str_len = recvfrom(serv_sock, &message, BUF_SIZE - 1, 0, reinterpret_cast<sockaddr*>(&clnt_adr), &clnt_adr_sz);
        message[str_len] = 0;
        cout << "Client IP: " << inet_ntoa(clnt_adr.sin_addr) << ", Client Port: " << ntohs(clnt_adr.sin_port) << endl;
        cout << "Message from client: " << message;
        sendto(serv_sock, message, str_len, 0,  reinterpret_cast<sockaddr*>(&clnt_adr), clnt_adr_sz);
    }
    close(serv_sock);
    return 0;
}