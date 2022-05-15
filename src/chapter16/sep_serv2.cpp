#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#define BUF_SIZE 1024

using namespace std;

int main(int argc, char* argv[]) {
    char buf[BUF_SIZE] = {0, };
    
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
    listen(serv_sock, 5);

    int clnt_addr;
    socklen_t clnt_adr_sz = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr*>(&clnt_addr), &clnt_adr_sz);

    FILE* readfp = fdopen(clnt_sock, "r");
    FILE* writefp = fdopen(dup(clnt_sock), "w");

    fputs("FROM SERVER: Hi~client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);

    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    fgets(buf, sizeof(buf), readfp);
    fputs(buf, stdout);
    fclose(readfp);

    return 0;
}