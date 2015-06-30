#include <vector>
#include <queue>
#include <set>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>
#include <cstdio>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	int cantidad_cores = argn[0];
	for (int i = 0; i < cantidad_cores; ++i)
	{
		this->quantums.push_back(argn[i+1]);
		this->cuota_actual.push_back(argn[i+1]);
	}
}

SchedRR::~SchedRR() {
}

void SchedRR::load(int pid) {
	this->cola_procesos.push(pid); // El llegar un nuevo proceso lo agregamos a la cola.
}

void SchedRR::unblock(int pid) {
	this->bloqueadas.erase(pid);	// Desbloqueamos el proceso.
	this->cola_procesos.push(pid);	// Lo agregamos nuevamente a la cola.
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int next_pid = IDLE_TASK;
	switch(m)
	{
		case TICK:
			// Si se está en IDLE y hay algun proceso esperando para ser ejecutado...
			if( current_pid(cpu) == IDLE_TASK && !cola_procesos.empty() ){
				cuota_actual[cpu] = quantums[cpu];	// Reseteamos el quantum.
				next_pid = next(cpu);				// Elegimos el próximo proceso a ejecutar.
			}
			else { // Si se está ejecutando una tarea que NO es IDLE...
				this->cuota_actual[cpu]--;	// Decrementamos el quantum.
				if( cuota_actual[cpu] == 0 ){ // ...si se terminó el quantum...
					this->cola_procesos.push(current_pid(cpu)); // Reencolamos el proceso.
					cuota_actual[cpu] = quantums[cpu];			// Reseteamos el quantum.
					next_pid = next(cpu);					// Devolvemos el siguiente proceso a ejecutar.
				}
				else { // ...si el quantum no terminó...
					next_pid = current_pid(cpu);	// Sigue ejecutando el proceso que ya tenía el CPU.
				}
			}
			break;
		case BLOCK:
			this->bloqueadas.insert(current_pid(cpu));	// Bloqueamos al proceso.
			cuota_actual[cpu] = quantums[cpu];
			next_pid = next(cpu);
			break;
		case EXIT:
			cuota_actual[cpu] = quantums[cpu];	// Reseteamos el quantum.
			next_pid = next(cpu);				// Elegimos el próximo proceso a ejecutar.
			break;
	}
	return next_pid;
}

int SchedRR::next(int cpu) {
	int next_pid = IDLE_TASK; // Por defecto devolvemos la tarea IDLE...
	if( !this->cola_procesos.empty() ) {//...si hay procesos en espera,
		next_pid = this->cola_procesos.front(); // Devolvemos el siguiente de la cola.
		this->cola_procesos.pop();
	}
	return next_pid;
}
