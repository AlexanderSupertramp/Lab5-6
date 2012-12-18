#include "lib/video.c"
#include "lib/longjmp.c"

#define TASK_STACK_SIZE 1000



typedef enum{ TS_RUNNABLE=1, TS_BLOCKED=2, TS_ZOMBIE=3} task_status_t;



typedef struct{
    jmp_buf registers;
    unsigned char stack[TASK_STACK_SIZE];
    task_status_t status;
}task_t;


////Commands:
//void cmd_clrscr(void);
//void cmd_privet(char** args);
//void cmd_senseOfLife(void);
//void cmd_whatIs(char *arg);
//
//
//#define MAX_LENGTH_OF_STRING_TO_READ 256
//#define MAX_NUMBER_OF_ARGS 5
//#define MAX_LENGTH_OF_ARG 40
//#define MAX_INDEX_FOR_FUNCTIONS_ARRAY 1000

task_t arrayOfTasks[4];
int numberOfTasks=0;
int currentTaskIndex=0;
	


void yield(void){
    //switch to the next task which is not TS_ZOMBIE
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

void wait(){
    long int i,j;
    long int n=10000;
    for (i=0; i<n; i++){
    for (j=0; j<n; j++ );
    }
}

void task1(void){
    char upLetter='A';
    while(1){
        if (!setjmp(arrayOfTasks[currentTaskIndex].registers)) yield();
        vidmem[0]=upLetter;
        upLetter++;//vidmem[0]=='B' ? 'A':'B';
        //getchar();
        wait();

    }
}

void task2(void){
    char lowLetter='z';
    while(1){
        if (!setjmp(arrayOfTasks[currentTaskIndex].registers)) yield();
        vidmem[2]=lowLetter;//vidmem[2]=='B'? 'A':'B';
        lowLetter--;
        wait();

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



////jmp_buf buf;
//
////returns an "unique" integer for a given string
//int getIndexForString(char *string);
//
////calls a function represented by a pointer, passing the corresponding arguments, stored in an array of words
////returns 1 if call successful, 0 otherwise
//int callFunctionWithPointerAndArgs(void (*pointer)(),char args[MAX_NUMBER_OF_ARGS][MAX_LENGTH_OF_ARG]);
//
////adds a pointer function to functions array, generating an unique indef for a given name
//void addCmdFunction(void (**functions)(),char* name,void (*function)());
//
////initialize all existent cmd functions calling addCmdFunction
//void initializeFunctions(void (**functions)());
//
////parse the string and detect all the words which come after the first one, store them into args
//int collectArguments(char* stringContainingArguments,char args[MAX_NUMBER_OF_ARGS][MAX_LENGTH_OF_ARG]);
//
////erease the existing arguments from the args
//void resetArguments(char args[MAX_NUMBER_OF_ARGS][MAX_LENGTH_OF_ARG]);


int main( void )
{
    newLine();
    newLine();

    vidmem[1]=0x7;
    vidmem[3]=0x7;

    sched_add_task(task1);
    sched_add_task(task2);
    sched_run();
    
    
    
    
//   puts("meme");
//    char s[MAX_LENGTH_OF_STRING_TO_READ];
//    int index,i;
//    void (*functions[MAX_INDEX_FOR_FUNCTIONS_ARRAY])(); 
//    char args[MAX_NUMBER_OF_ARGS][MAX_LENGTH_OF_ARG]; //An array of char* arguments
//
//    
//    initializeFunctions(functions);
//    
//    //longjmp(b,0);
//    
//   // puts("hola");
//    
//    while(1){
////        if (!setjmp(buf)) {
////            func1();
////        }
//        puts("main1");
//        newLine();
//        gets(s);
//    }
//    
    
  for(;;);
}

//void func1(){
//    func2();
//    puts("func1");
//    newLine();
//}
//
//void func2(){
//    puts("func2");
//    newLine();
//    longjmp(buf,1); //will not return to func1, will go to the place where setjmp() was called
//
//}
//
//
//void addCmdFunction(void (**functions)(),char* name,void (*function)()){
//    int index=getIndexForString(name);
//    functions[index]=function;
//}
//
//void initializeFunctions(void (**functions)()){
//    
//    addCmdFunction(functions,"privet",&cmd_privet);
//    addCmdFunction(functions,"clrscr",&cmd_clrscr);
//    addCmdFunction(functions,"senseOfLife",&cmd_senseOfLife);
//    addCmdFunction(functions,"whatIs",&cmd_whatIs);
//
//
//    
//}
//
//
//void resetArguments(char args[MAX_NUMBER_OF_ARGS][MAX_LENGTH_OF_ARG]){
//    int i;
//    
//    for (i=0; i<MAX_NUMBER_OF_ARGS; i++) {
//        strcpy(args[i]," ");
//    }
//}
//
//int collectArguments(char* string, char args[MAX_NUMBER_OF_ARGS][MAX_LENGTH_OF_ARG]){
//
//    int i=0,j=0,argNumber=0;
//    
//    resetArguments(args);
//    
//    while (string[i]!=' ') { //first word is the function name, skip that
//        if (string[i]=='\0') {
//            return 0; //no args
//        }
//        i++;
//    };
//    i++;
//    
//
//    
//    //we found a space
//    while (string[i]!='\0') {
//        
//        while (string[i]==' ') { //skip the spaces
//            i++;
//            if (string[i]=='\0') {
//                return 0; //no args
//            }
//        }
//        
//        //we found an arg
//        while (string[i]!=' ' && string[i]!='\0') {
//            //putc(string[i]);
//            args[argNumber][j]=string[i];
//            j++;
//            i++;
//        }
//        args[argNumber][j]='\0';
//        //puts("arg: ");
//
//       // puts(args[argNumber]);
//        newLine();
//        j=0;
//        argNumber++;
//    }
//    
//    return argNumber;
//}
//
//
//
//
//int getIndexForString(char *string){ //a character has a unique order in ASCII
//    int i=0;
//    int result=0;
//    while (string[i]!='\0' && string[i]!=' ') {
//        result=result+(string[i]-'a'+1)*(i+1);
//        i++;
//    }
//    return (result>0) ? result:(-result); //return abs(result)
//}
//
//
//
//
//
//int callFunctionWithPointerAndArgs(void (*pointer)(),char args[MAX_NUMBER_OF_ARGS][MAX_LENGTH_OF_ARG]){
//    if (pointer) {
//        (*pointer)(args);
//        newLine();
//        return 1;
//    }
//    newLine();
//    return 0;
//}
//
//
//
//
//
//
//
//
//
