#include "longjmp.c"

#define TASK_STACK_SIZE 1000



typedef enum{ TS_RUNNABLE=1, TS_BLOCKED=2, TS_ZOMBIE=3} task_status_t;



typedef struct{
    jmp_buf registers;
    unsigned char stack[TASK_STACK_SIZE];
    task_status_t status;
}task_t;



task_t arrayOfTasks[4];
int numberOfTasks=0;
int currentTaskIndex=0;

void yield(void){
    //switch to the next task which is not TS_ZOMBIE
    if (!setjmp(arrayOfTasks[currentTaskIndex].registers))
    {
        while(1){
            currentTaskIndex++;
            if (currentTaskIndex==numberOfTasks) {
                currentTaskIndex=0;
            }
            if (arrayOfTasks[currentTaskIndex].status==TS_BLOCKED ||
                arrayOfTasks[currentTaskIndex].status==TS_RUNNABLE) {
                longjmp(arrayOfTasks[currentTaskIndex].registers,1);
            }
        }
    }
    
    
}

void wait(){
    long int i,j;
    long int n=10000;
    for (i=0; i<n; i++){
        for (j=0; j<n; j++ );
    }
}

void sched_save_state(task_t *task){
    setjmp(task->registers);
}

void sched_add_task(void *function){
    task_t *task=&arrayOfTasks[numberOfTasks++];
    unsigned* initialStackPointer;
    
    sched_save_state(task);
    initialStackPointer=(unsigned)task->stack+TASK_STACK_SIZE;
    
    task->registers[0].esp=initialStackPointer;
    task->registers[0].eip=function;
    task->registers[0].eflags=0x200;
    task->status=TS_RUNNABLE;
    
    
}

void sched_run(void){
    longjmp(arrayOfTasks[currentTaskIndex].registers,1);
}
