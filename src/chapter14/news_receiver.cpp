#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 30
#define TTL 64

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <BoradcastIP> <PORT>" << endl;
        exit(1);
    }

    char buf[BUF_SIZE];

    sockaddr_in adr;
    memset(&adr, 0, sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = inet_addr(argv[1]);
    adr.sin_port = htons(atoi(argv[2]));

    int time_live = TTL;
    int recv_sock = socket(PF_INET, SOCK_DGRAM, 0);

    if (bind(recv_sock, reinterpret_cast<sockaddr*>(&adr), sizeof(adr)) == -1) {
        error_handling("bind() error!");
    }

    ip_mreq join_adr;
    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &join_adr, sizeof(join_adr));
    
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