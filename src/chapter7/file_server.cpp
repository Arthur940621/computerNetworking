#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define BUF_SIZE 30

using namespace std;

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    char buf[BUF_SIZE];
    memset(&buf, 0, sizeof(buf));
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }
    FILE* fp = fopen("./file_server.cpp", "rb");
    if (fp == NULL) {
        error_handling("fopen() error!");
    }
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error!");
    }
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("bind() error!");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error!");
    }
    sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz = sizeof(clnt_adr);
    int clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_adr), &clnt_adr_sz);
    if (clnt_sock == -1) {
        error_handling("accept() error!");
    } else {
        cout << "Connected client " << endl;
    }
    while (1) {
        int read_cnt = fread(buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            write(clnt_sock, buf, read_cnt);
            break;
        }
        write(clnt_sock, buf, BUF_SIZE);
    }
    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, buf, BUF_SIZE);
    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}