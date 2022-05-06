#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

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

    fd_set reads, cpy_reads;
    FD_ZERO(&reads);
    FD_SET(serv_socket, &reads);
    int fd_max = serv_socket;
    timeval timeout;
    int fd_num = 0;
    
    sockaddr_in clnt_addr;
    
    while (1) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1) {
            break;
        }
        if (fd_num == 0) {
            continue;
        }

        for (int i = 0; i != fd_max + 1; ++i) {
            if (FD_ISSET(i, &cpy_reads)) {
                if (i == serv_socket) {
                    socklen_t clnt_addr_size = sizeof(clnt_addr);
                    int clnt_socket = accept(serv_socket, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_size);
                    FD_SET(clnt_socket, &reads);
                    if (fd_max < clnt_socket) {
                        fd_max = clnt_socket;
                    }
                    cout << "connected cliend: " << clnt_socket << endl;
                } else {
                    int str_len = read(i, message, BUF_SIZE);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        cout << "closed client: " << i << endl;
                    } else {
                        write(i, message, str_len);
                    }

                }
            }
        }
    }
    close(serv_socket);
    return 0;
}