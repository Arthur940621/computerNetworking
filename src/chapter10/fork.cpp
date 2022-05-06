#include <iostream>
#include <unistd.h>

using namespace std;

int gval = 10;

int main() {
    int lval = 20;
    gval++, lval += 5; // gval = 11, lval = 25
    pid_t pid = fork();
    if (pid == 0) {
        gval += 2, lval += 2; // gval = 13, lval = 27
    } else {
        gval -= 2, lval -= 2; // gval = 9, lval = 23
    }
    if (pid == 0) {
        cout << "Child Proc: " << gval << ", " << lval << endl;
    } else {
        cout << "Parent Proc: " << gval << ", " << lval << endl;
    }
    return 0;
}