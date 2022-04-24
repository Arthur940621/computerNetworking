#include <iostream>
#include <arpa/inet.h>

using namespace std;

int main() {
    int16_t host_port = 0x1234;
    int32_t host_addr = 0x12345678;
    
    int16_t net_port = htons(host_port);
    int32_t net_addr = htonl(host_addr);

    cout << hex;
    cout << "Host ordered port: 0x" << host_port << endl;
    cout << "Network ordered port: 0x" << net_port << endl;
    cout << "Host ordered address: 0x" << host_addr << endl;
    cout << "Network ordered address: 0x" << net_addr << endl;
    cout << dec;

    return 0;
}