#include <iostream>
#include <wait.h>
#include <signal.h>
#include <unistd.h>

using namespace std;

void read_childproc(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        cout << "Removed proc id: " << id << endl;
        cout << "Child seend: " << WEXITSTATUS(status) << endl;
    }
}

int main() {
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);
    pid_t pid = fork();
    if (pid == 0) {
        cout << "Hi! I'm child process!" << endl;
        sleep(10);
        return 12;
    } else {
        cout << "Child proc id: " << pid << endl;
        pid = fork();
        if (pid == 0) {
            cout << "Hi! I'm child process!" << endl;
            sleep(10);
            exit(24);
        } else {
            cout << "Child proc id: " << pid << endl;
            for (int i = 0; i != 5; ++i) {
                cout << "wait..." << endl;
                sleep(5);
            }
        }
    }
    return 0;
}