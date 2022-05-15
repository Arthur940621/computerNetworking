#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

using namespace std;

void* thread_main(void* arg) {
    int cnt = *(reinterpret_cast<int*>(arg));
    string* msg = new string("Hello, I am thread~ \n");
    for (int i = 0; i != cnt; ++i) {
        sleep(1);
        cout << "running thread" << endl;
    }
    return msg;
}

int main() {
    pthread_t t_id;
    int thread_param = 5;
    if (pthread_create(&t_id, NULL, thread_main, &thread_param) != 0) {
        cerr << "pthread_create() error" << endl;
        return -1;
    }

    void* thr_ret;
    if (pthread_join(t_id, &thr_ret) != 0) {
        cerr << "pthread_join() error" << endl;
        return -1;
    }
    string* tmp = reinterpret_cast<string*>(thr_ret);
    cout << *tmp;
    delete tmp;
    cout << "end of main" << endl;
    return 0;
}