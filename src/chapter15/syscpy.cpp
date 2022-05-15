#include <iostream>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 3

using namespace std;

int main() {
    char buf[BUF_SIZE];
    int fd1 = open("news.txt", O_RDONLY, O_RSYNC);
    int fd2 = open("cpy.txt", O_WRONLY | O_CREAT | O_TRUNC);
    int len = 0;

    while ((len = read(fd1, buf, sizeof(buf))) > 0) {
        write(fd2, buf, len);
    }
    
    close(fd1);
    close(fd2);
    return 0;
}