#include <iostream>
#include <sys/socket.h>

using namespace std;

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}
int main() {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    int snd_buf = 1024 * 3;
    socklen_t len = sizeof(snd_buf);
    int state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<void*>(&snd_buf), len);
    if (state) {
        error_handling("getsockopt() error!");
    }
    cout << "Output buffer size: " << snd_buf << endl;

    int rcv_buf = 1024 * 4;
    len = sizeof(rcv_buf);
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<void*>(&rcv_buf), len);
    if (state) {
        error_handling("getsockopt() error!");
    }
    cout << "Input buffer size: " << rcv_buf << endl;
    return 0;
}