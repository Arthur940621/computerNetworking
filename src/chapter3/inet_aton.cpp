#include <iostream>
#include <string>
#include <arpa/inet.h>

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main() {
    const char* addr = "172.131.124.79";
    sockaddr_in addr_inet;
    int conv_addr = inet_aton(addr, &addr_inet.sin_addr);
    if (conv_addr == 0) {
        error_handling("Conversion error");
    } else {
        cout << hex;
        cout << "Network ordered integer addr: 0x" << addr_inet.sin_addr.s_addr << endl;
        cout << dec;
    }
    return 0;
}