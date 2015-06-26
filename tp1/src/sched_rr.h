#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include <set>
#include "basesched.h"

using namespace std;

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		//Codigo para calcular turnaround.
		// struct turnaround{
		// 	int pid;
		// 	int time;
		// 	bool exit;
		// };
		// vector<turnaround> t_time;

		int next(int cpu);
		vector<int> quantums;
		vector<int> cuota_actual;
		queue<int> cola_procesos;
		set<int> bloqueadas;



};

#endif
