#ifndef RWLock_h
#define RWLock_h
#include <iostream>

class RWLock {
    public:
        RWLock();
        void rlock();
        void wlock();
        void runlock();
        void wunlock();

    private:
        pthread_rwlock_t rwlock;
        pthread_cond_t roomEmpty;
        pthread_mutex_t mutex;
        pthread_mutex_t mutexRoom;
        pthread_mutex_t turnstile;
        int readers;
};

#endif
