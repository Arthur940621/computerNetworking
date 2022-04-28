#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

void error_handling(const string& buf) {
    cerr << buf << endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <addr>" << endl;
        exit(1);
    }
    hostent* host = gethostbyname(argv[1]);
    if (!host) {
        error_handling("gethostbyname() error");
    }
    cout << "Official name: " << host->h_name << endl;
    cout << "Address type: " << (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6") << endl;
    for (int i = 0; host->h_addr_list[i]; ++i) {
        cout << "IP addr " << i + 1 << ": " << inet_ntoa(*reinterpret_cast<in_addr*>(host->h_addr_list[i])) << endl;
    }
    return 0;
}