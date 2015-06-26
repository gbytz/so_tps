#include "tasks.h"
#include <iostream>
#include <stdlib.h>
using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void TaskConBloqueo(int pid, vector<int> params) {
	int tiempo = params[0];
	int inicioBloq = params[1];
	int finBloq = params[2];

	if(inicioBloq > tiempo){
		uso_CPU(pid, tiempo);
	}else{
		uso_CPU(pid,inicioBloq-2);
		uso_IO(pid,finBloq-inicioBloq+1);
		uso_CPU(pid,tiempo-finBloq);
	}
}

void TaskConsola(int pid, vector<int> params) { // params: n_llamadas bloqueantes, bmin, bmax
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];
	int timeBloq = 0;
	for(int i = 0; i < n; i++) {
		//srand(15);
		timeBloq = rand() % (bmax - bmin + 1) + bmin; //Lo que dura el bloqueo
		uso_IO(pid, timeBloq); // Se bloquea timeBloq miliseg
	}
}

void TaskBatch(int pid, vector<int> params){ //params: total_cpu, cant_bloqueos
	int total_cpu = params[0];
	int cant_bloqueos = params[1];
	int consumo;
	int falta_consumir = total_cpu - 1; //resto uno porque el exit me cuesta un ciclo.

	while(falta_consumir - cant_bloqueos > 0){
		//tiempo que consumo de cpu antes de hacer una llamada bloqueante.
		consumo = rand() % (falta_consumir - cant_bloqueos);
		uso_CPU(pid, consumo);
		falta_consumir -= consumo;
		
		if (cant_bloqueos > 0)
		{
			uso_IO(pid, 1); // llamada bloquante de 1 ciclo.
			falta_consumir--;
			cant_bloqueos--;
		}else{
			uso_CPU(pid, falta_consumir);
			break;
		}
	}
	
	if (cant_bloqueos > 0){
		for (int i = 0; i < cant_bloqueos; ++i)
		{
			uso_IO(pid, 1); // llamada bloquante de 1 ciclo.			
		}
	}

}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConBloqueo,3);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2)

}
