#include "RWLock.h"

RWLock :: RWLock() {
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&roomEmpty, NULL);
	readers = 0;
}

void RWLock :: rlock() {
	// Espero al semaforo y una vez que lo tengo aviso a otro reader.
	pthread_mutex_lock(&turnstile);
	pthread_mutex_unlock(&turnstile);
	
	//pido el mutex para modificar cant de readers
	pthread_mutex_lock(&mutex);
	readers++;
	// si soy el primero, bloqueo a los writers
	// if (readers == 1)
	// {
	// 	pthread_mutex_lock(roomEmpty);
	// }
	pthread_mutex_unlock(&mutex);
}

void RWLock :: wlock() {
	// si paso el turnstile ya no pueden entrar readers
	pthread_mutex_lock(&turnstile);
	//chequeo que estoy en condiciones de escribir
	pthread_mutex_lock(&mutexRoom);
	//espero a que no haya ningun reader para avanzar
	while(readers != 0){
		pthread_cond_wait(&roomEmpty, &mutexRoom);
	}
	pthread_mutex_unlock(&mutexRoom);
}

void RWLock :: runlock() {
	//pido el mutex para modificar la cant de readers
	pthread_mutex_lock(&mutex);
	readers--;
	//si soy el ultimo, habilito a los writers
	if (readers == 0)
	{
		pthread_cond_signal(&roomEmpty);
	}
	pthread_mutex_unlock(&mutex);
}

void RWLock :: wunlock() {
	pthread_mutex_unlock(&turnstile);
}

