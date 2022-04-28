#include <iostream>
#include <sys/socket.h>

using namespace std;

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}
int main() {
    int tcp_socket = socket(PF_INET, SOCK_STREAM, 0);
    int udp_socket = socket(PF_INET, SOCK_DGRAM, 0);
    int sock_type;
    socklen_t optlen = sizeof(sock_type);
    cout << "SOCK_STREAM: " << SOCK_STREAM << endl;
    cout << "SOCK_DGRAM: " << SOCK_DGRAM << endl;
    int state = getsockopt(tcp_socket, SOL_SOCKET, SO_TYPE, reinterpret_cast<void*>(&sock_type), &optlen);
    if (state) {
        error_handling("getsockopt() error!");
    }
    cout << "Soctet type one: " << sock_type << endl;

    state = getsockopt(udp_socket, SOL_SOCKET, SO_TYPE, reinterpret_cast<void*>(&sock_type), &optlen);
    if (state) {
        error_handling("getsockopt() error!");
    }
    cout << "Soctet type one: " << sock_type << endl;
    return 0;
}