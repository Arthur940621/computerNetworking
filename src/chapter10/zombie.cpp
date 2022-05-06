#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        cout << "Hi, I am a child process" << endl;
    } else {
        cout << "Child Process ID: " << pid << endl;
        sleep(30);
    }
    if (pid == 0) {
        cout << "End child process" << endl;
    } else {
        cout << "End parent process" << endl;
    }
    return 0;
}