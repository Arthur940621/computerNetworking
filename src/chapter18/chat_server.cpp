#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <list>
#include <algorithm>

#define BUF_SIZE 100


using namespace std;

void* handle_clnt(void* arg);
void send_msg(char* msg, int len);
void error_handling(const string& buf);

list<int> clnt_socks;
pthread_mutex_t mutex;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    pthread_mutex_init(&mutex, NULL);
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("bind() error!");
    }
    if (listen(serv_sock, 5) == -1) {
        error_handling("serv_sock() error!");
    }

    sockaddr_in clnt_addr;
    socklen_t clnt_addr_sz = sizeof(clnt_addr);
    while (1) {
        int clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_sz);

        pthread_mutex_lock(&mutex);
        clnt_socks.push_back(clnt_sock);
        pthread_mutex_unlock(&mutex);
        pthread_t t_id;
        pthread_create(&t_id, NULL, handle_clnt, &clnt_sock);
        pthread_detach(t_id);
        cout << "Connected client IP: " << inet_ntoa(clnt_addr.sin_addr) << endl;
    }

    close(serv_sock);
    return 0;
}

void* handle_clnt(void* arg) {
    int clnt_sock = *(reinterpret_cast<int*>(arg));
    char msg[BUF_SIZE];
    int str_len = 0;

    while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0) {
        send_msg(msg, str_len);        
    }

    pthread_mutex_lock(&mutex);
    list<int>::iterator it = find(clnt_socks.begin(), clnt_socks.end(), clnt_sock);
    if (it != clnt_socks.end()) {
        clnt_socks.erase(it);
    }
    pthread_mutex_unlock(&mutex);

    close(clnt_sock);
    return NULL;
}

void send_msg(char* msg, int len) {
    pthread_mutex_lock(&mutex);
    for (auto clnt_sock : clnt_socks) {
        write(clnt_sock, msg, len);
    }
    pthread_mutex_unlock(&mutex);
}

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}