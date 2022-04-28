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

int main(int argc, char* argv[]) {
    char message[BUF_SIZE];
    memset(&message, 0, sizeof(message));
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }
    int serv_socket = socket(PF_INET, SOCK_STREAM, 0);

    int option = 1;
    socklen_t optlen = sizeof(option);
    setsockopt(serv_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<void*>(&option), optlen);

    if (serv_socket == -1) {
        error_handling("socket() error!");
    }
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_socket, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("bind() error!");
    }
    if (listen(serv_socket, 5) == -1) {
        error_handling("listen() error!");
    }
    sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int str_len = 0;
    int clnt_sock = accept(serv_socket, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_size);
    if (clnt_sock == -1) {
        error_handling("accept() error!");
    } else {
        cout << "Connected client......" << endl;
    }
    while ((str_len = read(clnt_sock, message, BUF_SIZE - 1)) != 0) {
        message[str_len] = 0;
        cout << "Message from client: " << message;
        write(clnt_sock, message, str_len);
    }
    close(clnt_sock);
    close(serv_socket);
    return 0;
}