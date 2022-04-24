#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main() {
    sockaddr_in addr1, addr2;
    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    char* str_ptr = inet_ntoa(addr1.sin_addr);
    char str_arr[20];
    strcpy(str_arr, str_ptr);
    cout << str_ptr << endl;

    inet_ntoa(addr2.sin_addr);
    cout << str_ptr << endl;
    cout << str_arr << endl;
    
    return 0;
}