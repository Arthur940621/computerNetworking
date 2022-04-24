#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

void error_handling(const string& message) {
    cerr << message << endl;
    exit(1);
}

int main() {
    int fd;
    char buf[] = "Let's go!\n";
    fd = open("data.txt", O_CREAT|O_WRONLY|O_TRUNC, 0666);
    if (fd == -1) {
        error_handling("open() error!");
    }
    cout << "file descriptor: " << fd << endl;
    if (write(fd, buf, sizeof(buf)) == -1) {
        error_handling("write() error!");
    }
    close(fd);
    return 0;
}