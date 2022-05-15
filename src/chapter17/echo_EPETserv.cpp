#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <error.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

using namespace std;

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}

void setnonblockingmode(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port>" << endl;
        exit(1);
    }

    char buf[BUF_SIZE];

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

    int epfd = epoll_create(EPOLL_SIZE);
    epoll_event* ep_events;
    ep_events = new epoll_event[EPOLL_SIZE];

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    sockaddr_in clnt_addr;

    while (1) {
        int event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1) {
            cout << "epoll_wait() error!" << endl;
            break;
        }

        cout << "return epoll_wait" << endl;

        for (int i = 0; i != event_cnt; ++i) {
            if (ep_events[i].data.fd == serv_sock) {
                socklen_t clnt_addr_size = sizeof(clnt_addr);
                int clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_addr_size);
                setnonblockingmode(clnt_sock);
                // event.events = EPOLLIN;
                event.events = EPOLLIN|EPOLLET;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                cout << "connected cliend: " << clnt_sock << endl;
            } else {
                int str_len = 0;
                while (1) {
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if (str_len == 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        cout << "closed client: " << ep_events[i].data.fd << endl;
                        break;
                    } else if (str_len < 0) {
                        if (errno == EAGAIN) {
                            break;
                        } 
                    } else {
                            write(ep_events[i].data.fd, buf, str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}