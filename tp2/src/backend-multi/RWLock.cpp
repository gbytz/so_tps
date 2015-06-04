#include "RWLock.h"

RWLock :: RWLock() {
	pthread_rwlock_init(&rwlock, NULL);
}

void RWLock :: rlock() {
	pthread_rwlock_rdlock(&rwlock);
}

void RWLock :: wlock() {
	pthread_rwlock_wrlock(&rwlock);
}

void RWLock :: runlock() {
	pthread_rwlock_unlock(&rwlock);
}

void RWLock :: wunlock() {
	pthread_rwlock_unlock(&rwlock);
}
