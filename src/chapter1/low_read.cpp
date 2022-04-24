#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 100

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main() {
    int fd;
    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf));
    fd = open("data.txt", O_RDONLY);
    if (fd == -1) {
        error_handling("open() error!");
    }
    cout << "file descriptor: " << fd << endl;
    if (read(fd, buf, sizeof(buf)) == -1) {
        error_handling("read() error!");
    }
    cout << "file data: " << buf;
    close(fd);
    return 0;
}