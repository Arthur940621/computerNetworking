#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 30

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <PORT>" << endl;
        exit(1);
    }

    char buf[BUF_SIZE];

    sockaddr_in adr;
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[1]));

    int recv_sock = socket(PF_INET, SOCK_DGRAM, 0);

    if (bind(recv_sock, reinterpret_cast<sockaddr*>(&adr), sizeof(adr)) == -1) {
        error_handling("bind() error!");
    }
    
    while (1) {
        int str_len = recvfrom(recv_sock, buf, BUFSIZ - 1, 0, NULL, 0);
        if (str_len < 0) {
            break;
        }
        buf[str_len] = 0;
        cout << buf;
    }

    close(recv_sock);
    return 0;
}