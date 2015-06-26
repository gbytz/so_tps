#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
    int cantidad_cores = argn[0];
    for(int i = 0; i < cantidad_cores; ++i){
        cpu_data cpu;
        cpu.initial_quantum = argn[i+1];
        cpu.quantum_left = argn[i+1];
        this->cpus.push_back(cpu);
    }
}

SchedRR2::~SchedRR2() {
}

void SchedRR2::load(int pid) {
    int cpu = get_unloaded_cpu(); // Buscamos el CPU con carga mínima.
    queue_pid(cpu, pid); // Encolamos el nuevo proceso.
    set_cpu(pid, cpu); // Registramos a cual CPU pertenece el proceso.
}

void SchedRR2::unblock(int pid) {
    int cpu = get_cpu(pid); // Buscamos a cual CPU está asociado el proceso.
    unblock_pid(cpu, pid); // Lo desbloqueamos en ese CPU.
    queue_pid(cpu, pid); // Encolamos nuevamente el proceso para ser atendido por su CPU.
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
    int next_pid;
    switch(m)
    {
        case TICK:
            if( is_idle(cpu) && processes_waiting(cpu) )
            {
                reset_quantum(cpu);     // Reseteamos el quantum.
                next_pid = next(cpu);   // Elegimos el próximo proceso a ejecutar.
            }
            else
            {
                decrease_quantum(cpu);  // Decrementamos el quantum.
                if( quantum_finished(cpu) )
                {
                    queue_pid(cpu, current_pid(cpu));   // Reencolamos el proceso al que se le terminó el quantum.
                    reset_quantum(cpu);
                    next_pid = next(cpu);
                }
                else
                {
                    next_pid = current_pid(cpu);    // Sigue ejecutando el proceso que ya tenía el CPU.
                }
            }
            break;
        case BLOCK:
            block_pid(cpu, current_pid(cpu));   // Bloqueamos el proceso que estaba ejecutando.
            reset_quantum(cpu);
            next_pid = next(cpu);
            break;

        case EXIT:
            remove_pid(current_pid(cpu));   // Desasocio el proceso del CPU que le fue asignado al ser cargado.
            reset_quantum(cpu);
            next_pid = next(cpu);
            break;
    }
    return next_pid;
}

int SchedRR2::next(int cpu) {
    int next_pid = IDLE_TASK;
    if( processes_waiting(cpu) )
    {
        next_pid = this->cpus[cpu].ready_queue.front();
        this->cpus[cpu].ready_queue.pop();
    }
    return next_pid;
}

int SchedRR2::get_unloaded_cpu(){
    int next_cpu = 0;
    for(unsigned int i = 0; i < this->cpus.size(); ++i){
        if(cpu_load(next_cpu) > cpu_load(i)) next_cpu = i;
    }
    return next_cpu;
}

int SchedRR2::cpu_load(int cpu) {
    int load = 0;
    load = (current_pid(cpu) != IDLE_TASK) + this->cpus[cpu].blocked_set.size() + this->cpus[cpu].ready_queue.size(); // Carga = Running + Blocked + Ready
    return load;
}

int SchedRR2::get_cpu(int pid) {
    return this->pids_cpu[pid];
}

void SchedRR2::set_cpu(int pid, int cpu){
    this->pids_cpu[pid] = cpu;
}

void SchedRR2::remove_pid(int pid){
    this->pids_cpu.erase(pid);
}

void SchedRR2::reset_quantum(int cpu){
    this->cpus[cpu].quantum_left = this->cpus[cpu].initial_quantum;
}

void SchedRR2::decrease_quantum(int cpu){
    this->cpus[cpu].quantum_left--;
}

bool SchedRR2::quantum_finished(int cpu){
    return this->cpus[cpu].quantum_left == 0;
}

void SchedRR2::queue_pid(int cpu, int pid){
    this->cpus[cpu].ready_queue.push(pid);
}
void SchedRR2::block_pid(int cpu, int pid){
    this->cpus[cpu].blocked_set.insert(pid);
}

void SchedRR2::unblock_pid(int cpu, int pid){
    this->cpus[cpu].blocked_set.erase(pid);
}

bool SchedRR2::is_idle(int cpu){
    return current_pid(cpu) == IDLE_TASK;
}

bool SchedRR2::processes_waiting(int cpu){
    return !this->cpus[cpu].ready_queue.empty();
}