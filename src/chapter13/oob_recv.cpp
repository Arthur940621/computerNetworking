#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#define BUF_SIZE 30

int acpt_sock;
int recv_sock;

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

void urg_handler(int signi) {
    char buf[BUF_SIZE];
    int str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_OOB);
    buf[str_len] = 0;
    cout << "Urgent message: " << buf << endl;
}

int main(int argc, char* argv[]) {
    char buf[BUF_SIZE];

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (acpt_sock == -1) {
        error_handling("socket() error!");
    }

    sockaddr_in recv_addr;
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(acpt_sock, reinterpret_cast<sockaddr*>(&recv_addr), sizeof(recv_addr)) == -1) {
        error_handling("bind() error!");
    }
    if (listen(acpt_sock, 5) == -1) {
        error_handling("listen() error!");
    }

    sockaddr_in serv_addr;
    socklen_t serv_addr_sz = sizeof(serv_addr);
    recv_sock = accept(acpt_sock, reinterpret_cast<sockaddr*>(&serv_addr), &serv_addr_sz);
    if (recv_sock == -1) {
        error_handling("accept() error!");
    }

    fcntl(recv_sock, F_SETOWN, getpid());
    int state = sigaction(SIGURG, &act, 0);

    int str_len = 0;
    while ((str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0)) != 0) {
        if (str_len == -1) {
            continue;
        }
        cout << str_len << endl;
        buf[str_len] = 0;
        cout << buf << endl;
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;
}