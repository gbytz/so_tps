#include "sched_fixed.h"
#include <iostream>

using namespace std;

SchedFixed::SchedFixed(vector<int> argn) {
}

SchedFixed::~SchedFixed() {
}

void SchedFixed::initialize() {
}

void SchedFixed::load(int pid) {
	datos_proc nuevo;
	nuevo.pid = pid;
	nuevo.periodo = period(pid);
	this->procesos.push_back(nuevo);
}

void SchedFixed::unblock(int pid) {
}

int SchedFixed::tick(int cpu, const enum Motivo m) {
	int next_pid = IDLE_TASK;
	int current = current_pid(cpu);
	switch(m)
	{
		case TICK:
			// Si se está en IDLE y hay algun proceso esperando para ser ejecutado...
			next_pid = next(cpu); // Elegimos el próximo proceso a ejecutar.
			break;
		case BLOCK:
			break;
		case EXIT:
			for (unsigned int i = 0; i < procesos.size(); ++i)
			{
				if (procesos[i].pid == current)
				{
					procesos.erase(procesos.begin()+i);
				}

			}
			next_pid = next(cpu); // Elegimos el próximo proceso a ejecutar.
			break;
	}
	return next_pid;
}

int SchedFixed::next(int cpu) {
	int next_pid = IDLE_TASK; // Por defecto devolvemos la tarea IDLE...
	int periodo = 99999;
	for (unsigned int i = 0; i < procesos.size(); ++i)
	{
		if (procesos[i].periodo < periodo)
		{
			next_pid = procesos[i].pid;
			periodo = procesos[i].periodo;
		}
	}
	return next_pid;
}
