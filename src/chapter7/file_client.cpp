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
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <IP> <port>" << endl;
        exit(1);
    }
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handling("socket() error!");
    }
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (connect(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("connect() error!");
    } else {
        cout << "Connected......" << endl;
    }
    FILE* fp = fopen("receive.dat", "wb");
    int read_cnt = 0;
    while ((read_cnt = read(sock, buf, BUF_SIZE)) != 0) {
        fwrite(buf, 1, read_cnt, fp);
    }
    cout << "Received file data" << endl;
    write(sock, "Think you", 10);
    fclose(fp);
    close(sock);
    return 0;
}