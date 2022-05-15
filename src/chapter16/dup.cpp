#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    char str1[] = "Hi~ \n";
    char str2[] = "It's nice day~ \n";

    int cfd1 = dup(1);
    int cfd2 = dup2(cfd1, 7);

    cout << "fd1 = " << cfd1 << ", fd2 = " << cfd2 << endl;
    write(cfd1, str1, sizeof(str1));
    write(cfd2, str2, sizeof(str2));

    close(cfd1);
    close(cfd2);
    write(1, str1, sizeof(str1));
    close(1);
    write(1, str2, sizeof(str2));
    return 0;
}