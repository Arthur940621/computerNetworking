#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 1024

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

void read_childproc(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        cout << "Removed proc id: " << id << ", Child seed: " << WEXITSTATUS(status) << endl;
    }
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

    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    
    if (bind(serv_socket, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == -1) {
        error_handling("bind() error!");
    }
    if (listen(serv_socket, 5) == -1) {
        error_handling("listen() error!");
    }
    
    int fds[2];
    pipe(fds);
    pid_t pid = fork();
    if (pid == 0) {
        FILE* fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        for (int i = 0; i != 10; ++i) {
            int len = read(fds[0], msgbuf, BUF_SIZE);
            fwrite(msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0;
    }

    sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int str_len = 0;
    while (1) {
        int clnt_sock = accept(serv_socket, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_size);
        if (clnt_sock == -1) {
            continue;
        } else {
            cout << "new connected client" << endl;
        }

        pid = fork();
        if (pid == -1) {
            close(clnt_sock);
            continue;
        }
        if (pid == 0) {
            close(serv_socket);
            while ((str_len = read(clnt_sock, message, BUF_SIZE - 1)) != 0) {
                message[str_len] = 0;
                cout << "Message from client: " << message;
                write(clnt_sock, message, str_len);
                write(fds[1], message, str_len);
            }
            close(clnt_sock);
            cout << "client disconnected..." << endl;
            return 0;
        } else {
            close(clnt_sock);
        }
    }
    close(serv_socket);
    return 0;
}