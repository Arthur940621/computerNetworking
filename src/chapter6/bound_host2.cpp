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
    char msg1[] = "Hi!\n";
    char msg2[] = "I'm another UDP host!\n";
    char msg3[] = "Nice to meet you\n";
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    sockaddr_in your_adr;
    memset(&your_adr, 0, sizeof(your_adr));
    your_adr.sin_family = AF_INET;
    your_adr.sin_addr.s_addr = inet_addr(argv[1]);
    your_adr.sin_port = htons(atoi(argv[2]));
    sendto(sock, msg1, sizeof(msg1), 0, reinterpret_cast<sockaddr*>(&your_adr), sizeof(your_adr));
    sendto(sock, msg2, sizeof(msg2), 0, reinterpret_cast<sockaddr*>(&your_adr), sizeof(your_adr));
    sendto(sock, msg3, sizeof(msg3), 0, reinterpret_cast<sockaddr*>(&your_adr), sizeof(your_adr));
    close(sock);
    return 0;
}