#include <iostream>
#include <arpa/inet.h>

using namespace std;

int main() {
    const char* addr1 = "1.2.3.4";
    const char* addr2 = "1.2.3.256";
    int32_t conv_addr = inet_addr(addr1);
    if (conv_addr == INADDR_NONE) {
        cout << "Error occured!" << endl;
    } else {
        cout << hex;
        cout << "Network ordered integer addr: 0x" << conv_addr << endl;
        cout << dec;
    }

    conv_addr = inet_addr(addr2);
    if (conv_addr == INADDR_NONE) {
        cout << "Error occured!" << endl;
    } else {
        cout << hex;
        cout << "Network ordered integer addr: 0x" << conv_addr << endl;
        cout << dec;
    }
    return 0;
}