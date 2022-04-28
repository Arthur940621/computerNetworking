#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <IP>" << endl;
        exit(1);
    }
    sockaddr_in addr;
    memset(&addr, 0 ,sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    hostent* host = gethostbyaddr(reinterpret_cast<char*>(&addr.sin_addr), sizeof(&addr.sin_addr), AF_INET);
    if (!host) {
        error_handling("gethostbyaddr() error");
    }
    cout << "Official name: " << host->h_name << endl;
    for (int i = 0; host->h_aliases[i]; ++i) {
        cout << "Aliaaes " << i + 1 << ": " << host->h_aliases[i] << endl;
    }
    cout << "Address type: " << (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6") << endl;
    for (int i = 0; host->h_addr_list[i]; ++i) {
        cout << "IP addr " << i + 1 << ": " << inet_ntoa(*reinterpret_cast<in_addr*>(host->h_addr_list[i])) << endl;
    }
    return 0;
}