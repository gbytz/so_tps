#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include <set>
#include <map>
#include "basesched.h"

using namespace std;

class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
    	~SchedRR2();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
        struct cpu_data
        {
            int initial_quantum;
            int quantum_left;
            queue<int> ready_queue;
            set<int> blocked_set;
        };

        vector<cpu_data> cpus;
        map<int, int> pids_cpu;

		int next(int cpu);

        int get_unloaded_cpu();
        int cpu_load(int cpu) ;
        int get_cpu(int pid) ;
        void set_cpu(int pid, int cpu);
        void remove_pid(int pid);
        void reset_quantum(int cpu);
        void decrease_quantum(int cpu);
        bool quantum_finished(int cpu)  ;
        void queue_pid(int cpu, int pid);
        void block_pid(int cpu, int pid);
        void unblock_pid(int cpu, int pid);
        bool is_idle(int cpu);
        bool processes_waiting(int cpu);
        // vector<int> quantums;
        // vector<int> cuota_actual;
        // vector<queue<int> > cola_procesos;
        // vector<set<int> > bloqueadas;
};

#endif
