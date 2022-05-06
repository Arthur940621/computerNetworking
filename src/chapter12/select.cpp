#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#define BUF_SIZE 1024

using namespace std;

int main() {
    char buf[BUF_SIZE];
    
    timeval timeout;
    // timeout.tv_sec = 5;
    // timeout.tv_usec = 5000;

    fd_set reads, temps;
    FD_ZERO(&reads);
    FD_SET(0, &reads);

    while (1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int result = select(1, &temps, 0, 0, &timeout);
        if (result == -1) {
            cout << "select() error!" << endl;
            break;
        } else if (result == 0) {
            cout << "Time-out!" << endl;
        } else {
            if (FD_ISSET(0, &temps)) {
                int str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                cout << buf;
            }
        }
    }

    return 0;
}