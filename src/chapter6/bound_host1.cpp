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
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(atoi(argv[1]));
    if (bind(sock, reinterpret_cast<sockaddr*>(&my_addr), sizeof(my_addr)) == -1) {
        error_handling("bind() error");
    }
    for (int i = 0; i != 3; ++i) {
        sleep(5);
        sockaddr_in your_addr;
        socklen_t adr_sz = sizeof(your_addr);
        int str_len = recvfrom(sock, message, BUF_SIZE - 1, 0, reinterpret_cast<sockaddr*>(&your_addr), &adr_sz);
        message[str_len] = 0;
        cout << "Message " << i + 1 << ": " << message;
    }
    close(sock);
    return 0;
}