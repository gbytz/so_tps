#include "sched_dynamic.h"
#include <iostream>

using namespace std;

SchedDynamic::SchedDynamic(vector<int> argn) {
}

SchedDynamic::~SchedDynamic() {
}

void SchedDynamic::initialize() {
}

void SchedDynamic::load(int pid) {
	datos_pid nuevo;
	nuevo.pid = pid;
	nuevo.deadline = period(pid);
	this->procesos.push_back(nuevo);
}

void SchedDynamic::unblock(int pid) {
}

int SchedDynamic::tick(int cpu, const enum Motivo m) {
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

int SchedDynamic::next(int cpu) {
	int next_pid = IDLE_TASK; // Por defecto devolvemos la tarea IDLE...
	int deadline = 999999999;
	for (unsigned int i = 0; i < procesos.size(); ++i)
	{
		procesos[i].deadline--;
		if (procesos[i].deadline < deadline)
		{
			next_pid = procesos[i].pid;
			deadline = procesos[i].deadline;
		}
	}
	return next_pid;
}
