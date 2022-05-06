#include <iostream>
#include <unistd.h>

#define BUF_SIZE 30

using namespace std;

int main() {
    int fds1[2], fds2[2];
    char str1[] = "Who are you?\n";
    char str2[] = "Think you for message\n";
    char buf[BUF_SIZE];
    pipe(fds1), pipe(fds2);
    pid_t pid = fork();
    if (pid == 0) {
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], buf, BUF_SIZE);
        cout << "Child proc output: " << buf;
    } else {
        read(fds1[0], buf, BUF_SIZE);
        cout << "Parent proc output: " << buf;
        write(fds2[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}