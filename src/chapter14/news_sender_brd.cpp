#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 30

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}


int main(int argc, char* argv[]) {    
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <GroupIP> <PORT>" << endl;
        exit(1);
    }

    char buf[BUF_SIZE];
    
    sockaddr_in broad_adr;
    memset(&broad_adr, 0, sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_adr.sin_port = htons(atoi(argv[2]));

    int send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    int so_brd = 1;
    setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, &so_brd, sizeof(so_brd));

    FILE* fp;
    if ((fp = fopen("news.txt", "r")) == NULL) {
        error_handling("fopen() error!");
    }

    while (!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, reinterpret_cast<sockaddr*>(&broad_adr), sizeof(broad_adr));
        sleep(2);
    }
    
    fclose(fp);
    close(send_sock);
    return 0;
}