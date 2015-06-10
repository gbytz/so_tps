#include "RWLock.h"

RWLock :: RWLock() {
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&turnstile, NULL);
	pthread_cond_init(&roomEmpty, NULL);
	readers = 0;
	writers = 0;
}

RWLock :: ~RWLock() {
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&turnstile);
	pthread_cond_destroy(&roomEmpty);
}

void RWLock :: rlock() {
	// Espero al semaforo y una vez que lo tengo aviso a otro reader.
	pthread_mutex_lock(&turnstile);
	pthread_mutex_unlock(&turnstile);
	
	//pido el mutex para modificar cant de readers
	pthread_mutex_lock(&mutex);
	// me fijo que no haya entrado ningun writer
	while(writers != 0){
		pthread_cond_wait(&roomEmpty, &mutex);
	}
	// al aumentar readers bloqueo a los writers
	readers++;
	pthread_mutex_unlock(&mutex);
}

void RWLock :: wlock() {
	// si paso el turnstile ya no pueden entrar readers ni writers
	pthread_mutex_lock(&turnstile);
	//chequeo que estoy en condiciones de escribir
	pthread_mutex_lock(&mutex);
	//espero a que no haya ningun reader para avanzar
	while(readers != 0){
		pthread_cond_wait(&roomEmpty, &mutex);
	}
	writers++;
	pthread_mutex_unlock(&mutex);
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
	pthread_mutex_lock(&mutex);
	// siempre soy el ultimo
	writers--;
	// despierto a todos los readers que esten esperando
	pthread_cond_broadcast(&roomEmpty);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&turnstile);
}

