#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define BUF_SIZE 100

using namespace std;

void* send_msg(void* arg);
void* recv_msg(void* arg);
void error_handling(const string& buf);

char msg[BUF_SIZE];
string name = "[DEFAULT]";

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <IP> <port> <name>" << endl;
        exit(1);
    }

    name = argv[3];
    name = "[" + name + "]";

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if ((connect(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr))) == -1) {
        error_handling("connect() error!");
    }

    pthread_t snd_thread, rcv_thread;
    pthread_create(&snd_thread, NULL, send_msg, &sock);
    pthread_create(&rcv_thread, NULL, recv_msg, &sock);
    void* thread_return;
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);

    close(sock);
    return 0;
}

void* send_msg(void* arg) {
    int sock = *(reinterpret_cast<int*>(arg));
    char name_msg[name.size() + BUF_SIZE + 1];
    while (1) {
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(sock);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name.c_str(), msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}

void* recv_msg(void* arg) {
    int sock = *(reinterpret_cast<int*>(arg));
    char name_msg[name.size() + BUF_SIZE + 1];
    int str_len = 0;
    while (1) {
        str_len = read(sock, name_msg, name.size() + BUF_SIZE);
        if (str_len == -1) {
            return reinterpret_cast<void*>(-1);
        }
        name_msg[str_len] = 0;
        cout << name_msg;
    }
    return NULL;
}

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}