#include "lib/video.c"
#include "lib/multitasking.c"	

void task1(void){
    char upLetter='A';
    while(1){
        //if (!setjmp(arrayOfTasks[currentTaskIndex].registers)) yield();
        yield();
        vidmem[0]=upLetter;
        upLetter++;//vidmem[0]=='B' ? 'A':'B';
        //getchar();
        wait();

    }
}

void task2(void){
    char lowLetter='z';
    while(1){
       // if (!setjmp(arrayOfTasks[currentTaskIndex].registers)) yield();
        yield();
        vidmem[2]=lowLetter;//vidmem[2]=='B'? 'A':'B';
        lowLetter--;
        wait();

    }
}




int main( void )
{
    newLine();
    newLine();

    vidmem[1]=0x7;
    vidmem[3]=0x7;

    sched_add_task(task1);
    sched_add_task(task2);
    sched_run();
  for(;;);
}
