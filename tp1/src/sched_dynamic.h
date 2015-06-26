#ifndef __SCHED_DYNAMIC_
#define __SCHED_DYNAMIC_

#include <vector>
#include <set>
#include <queue>
#include "basesched.h"

using namespace std;

class SchedDynamic : public SchedBase {
	public:
		SchedDynamic(std::vector<int> argn);
        	~SchedDynamic();
		virtual void initialize();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int next(int cpu);
		struct datos_pid{
			int pid;
			int deadline;
		};

		set<int> task_blocked;
		vector<datos_pid> procesos;
};

#endif
