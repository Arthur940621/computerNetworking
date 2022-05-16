#include <iostream>
#include <pthread.h>
#include <semaphore.h>

static sem_t sem_one;
static sem_t sem_two;
static int num;

using namespace std;

void* read(void* arg) {
    for (int i = 0; i != 5; ++i) {
        cout << "Input num: " << endl;
        sem_wait(&sem_two);
        cin >> num;
        sem_post(&sem_one);
    }
    return NULL;
}

void* accu(void* arg) {
    int sum = 0;
    for (int i = 0; i != 5; ++i) {
        sem_wait(&sem_one);
        sum += num;
        sem_post(&sem_two);
    }
    cout << "Result: " << sum << endl;
    return NULL;
}

int main() {
    sem_init(&sem_one, 0, 0);
    sem_init(&sem_two, 0, 1);
    
    pthread_t id_t1, id_t2;
    pthread_create(&id_t1, NULL, read, NULL);
    pthread_create(&id_t2, NULL, accu, NULL);

    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}