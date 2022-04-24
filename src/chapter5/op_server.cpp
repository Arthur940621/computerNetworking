#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define BUF_SIZE 1024
#define OPSZ 4 // 操作数的长度为 4 个字节

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int calculate(int opnum, int opnds[], char op) {
    int result = opnds[0];
    switch(op) {
        case '+':
            for (int i = 1; i < opnum; ++i) { result += opnds[i]; }
            break;
        case '-':
            for (int i = 1; i < opnum; ++i) { result -= opnds[i]; }
            break;
        case '*':
            for (int i = 1; i < opnum; ++i) { result *= opnds[i]; }
            break;    
    }
    return result;
}

int main(int argc, char* argv[]) {
    char opinfo[BUF_SIZE];
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket() error");
    }
    sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_adr), sizeof(serv_adr)) == -1) {
        error_handling("bind() error");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }
    sockaddr_in clnt_addr;
    socklen_t clnt_adr_sz = sizeof(clnt_addr);
    for (int i = 0; i != 5; ++i) {
        int opnd_cnt = 0;
        int clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_adr_sz);
        if (clnt_sock == -1) {
            error_handling("accept() error!");
        } else {
            cout << "Connected client " << i + 1 << endl;
        }
        read(clnt_sock, &opnd_cnt, 1);
        int recv_len = 0;
        while (recv_len < (opnd_cnt * OPSZ + 1)) {
            int recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
            recv_len += recv_cnt;
        }
        int result = calculate(opnd_cnt, reinterpret_cast<int*>(opinfo), opinfo[recv_len - 1]);
        write(clnt_sock, &result, sizeof(result));
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}