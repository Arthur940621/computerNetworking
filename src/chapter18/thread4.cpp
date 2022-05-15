#include <iostream>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100

long long num = 0;

using namespace std;

void* thread_inc(void* arg) {
    for  (int i = 0; i != 5000; ++i) {
        num += 1;
    }
    return NULL;
}

void* thread_des(void* arg) {
    for  (int i = 0; i != 5000; ++i) {
        num -= 1;
    }
    return NULL;
}

int main() {
    pthread_t thread_id[NUM_THREAD];
    cout << "sizeof long long: " << sizeof(long long) << endl;
    for (int i = 0; i != NUM_THREAD; ++i) {
        if (i % 2) {
            pthread_create(&thread_id[i], NULL, thread_inc, NULL);
        } else {
            pthread_create(&thread_id[i], NULL, thread_des, NULL);
        }
    }
    for (int i = 0; i != NUM_THREAD; ++i) {
        pthread_join(thread_id[i], NULL);
    }
    cout << "result: " << num << endl;
    return 0;
}