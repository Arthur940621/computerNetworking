#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define TTL 64
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
    
    sockaddr_in mul_adr;
    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));

    int time_live = TTL;
    int send_sock = socket(PF_INET, SOCK_DGRAM, 0);
    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, &time_live, sizeof(time_live));

    FILE* fp;
    if ((fp = fopen("news.txt", "r")) == NULL) {
        error_handling("fopen() error!");
    }

    while (!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(send_sock, buf, strlen(buf), 0, reinterpret_cast<sockaddr*>(&mul_adr), sizeof(mul_adr));
        sleep(2);
    }
    
    fclose(fp);
    close(send_sock);
    return 0;
}