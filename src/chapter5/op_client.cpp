#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define BUF_SIZE 1024
#define OPSZ 4 // 操作数的长度为 4 个字节
#define RLT_SIZE 4 // 待接收的数据长度为 4 字节

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    char opmsg[BUF_SIZE];
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("connect() error");
    } else {
        cout << "Connected......" << endl;
    }
    cout << "Operand count: ";
    int opnd_cnt = 0;
    cin >> opnd_cnt;
    opmsg[0] = static_cast<char>(opnd_cnt);
    for (int i = 0; i < opnd_cnt; ++i) {
        cout << "Operator " << i + 1 << ": ";
        scanf("%d", reinterpret_cast<int*>(&opmsg[i * OPSZ + 1]));
    }
    // fgetc(stdin); // 吞回车，但其实 cin 会跳过回车，scanf("%c", ch) 不会跳过回车
    cout << "Operator: ";
    cin >> opmsg[opnd_cnt * OPSZ + 1];
    write(sock, opmsg, opnd_cnt * OPSZ + 2);
    int result = 0;
    read(sock, &result, RLT_SIZE);
    cout << "Operation result: " << result << endl;
    close(sock);
    return 0;
}