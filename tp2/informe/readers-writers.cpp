readers = 0;
roomEmpty = Semaphore(1);
turnstile = Semaphore(1);
mutex = Semaphore(1);

//writer
turnstile.wait();
roomEmpty.wait();

// critical section for writers

turnstile.signal();
roomEmpty.signal();

//reader
turnstile.wait();
turnstile.signal();

mutex.wait();
readers ++;
if (readers == 1){
	roomEmpty.wait();
}
mutex.signal();

// critical section for readers

mutex.wait();
readers --;
if (readers == 0){
	roomEmpty.signal();
}
mutex.signal();