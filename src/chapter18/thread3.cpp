#include <iostream>
#include <pthread.h>

using namespace std;

int sum = 0;
void* thread_summation(void* arg) {
    int start = reinterpret_cast<int*>(arg)[0];
    int end = reinterpret_cast<int*>(arg)[1];

    while (start <= end) {
        sum += start;
        ++start;
    }
    return NULL;
}

int main() {
    pthread_t id_t1, id_t2;
    int range1[] = {1, 5};
    int range2[] = {6, 10};

    pthread_create(&id_t1, NULL, thread_summation, range1);
    pthread_create(&id_t2, NULL, thread_summation, range2);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);
    
    cout << "result: " << sum << endl;
    return 0;
}