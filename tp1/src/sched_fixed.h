#ifndef __SCHED_FIXED_
#define __SCHED_FIXED_

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;

class SchedFixed : public SchedBase {
	public:
		SchedFixed(std::vector<int> argn);
        	~SchedFixed();
		virtual void initialize();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		int next(int cpu);
		struct datos_proc{
			int pid;
			int periodo;
		};

		vector<datos_proc> procesos;
};

#endif
