#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

#define BUF_SIZE 1024

using namespace std;

int main(int argc, char* argv[]) {
    char buf[BUF_SIZE];

    int sock = socket(PF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
    FILE* readfp = fdopen(sock, "r");
    FILE* writefp = fdopen(sock ,"w");

    while(1) {
        if (fgets(buf, sizeof(buf), readfp) == NULL) {
            break;
        }
        fputs(buf, stdout);
        fflush(stdout);
    }

    fputs("FROM CLIENT: Thank you! \n", writefp);
    fflush(writefp);
    fclose(writefp);
    fclose(readfp);
    return 0;

}