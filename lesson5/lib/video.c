#include "io.c"
#include "kbd.c"

//define the maximal number of rows and columns which can be displayed on that screen
#define MAX_NUMB_OF_ROWS 25
#define MAX_NUMB_OF_COLUMNS 80


char* vidmem = (char *) 0xb8000;
//global x and y for our current position for read/write
int cursorColumn=0;
int cursorRow=0;



// puts a char at the current cursor position and updates this position
//scrolls the content if needed
void putc(char c){
    vidmem[cursorRow*2*MAX_NUMB_OF_COLUMNS+cursorColumn*2]=c;
    vidmem[cursorRow*2*MAX_NUMB_OF_COLUMNS+cursorColumn*2+1]=0x7;
    cursorColumn++;
    scrollIfNeeded();
}

//checks the current cursor position:
//if y>boundOfY go to new line
//if x>boundOfX "scroll" everything one line up
void scrollIfNeeded()
{
    if (cursorColumn==MAX_NUMB_OF_COLUMNS){
        cursorRow++;
        cursorColumn=0;
    }
    if (cursorRow==MAX_NUMB_OF_ROWS){
        int i=0, j=0;
        cursorRow--;
        for (i; i<=cursorRow; i++) {
            for (j=0; j<MAX_NUMB_OF_COLUMNS; j++) {
                vidmem[i*2*MAX_NUMB_OF_COLUMNS+j*2]=vidmem[(i+1)*2*MAX_NUMB_OF_COLUMNS+j*2];
                vidmem[i*2*MAX_NUMB_OF_COLUMNS+j*2+1]=vidmem[(i+1)*2*MAX_NUMB_OF_COLUMNS+j*2+1];
                
                if (i==cursorRow){ //last line should be blank
                    vidmem[cursorRow*2*MAX_NUMB_OF_COLUMNS+cursorColumn*2]=' ';
                    vidmem[cursorRow*2*MAX_NUMB_OF_COLUMNS+cursorColumn*2+1]=0x0;
                }
            }
        }
    }
}

//just a loop with putc gives us:
void puts(char* string){
    int i=0;
    while (string[i]!='\0') {
        putc(string[i]);
        i++;
    }
}

void newLine(){
    setCursorPosition(cursorRow+1,0);//go to next line
}



//transforms all the screen in "black spaces"
//resets the cursor position to (0,0)
void clrscr(){
    int i=0,j=0;
    for (i; i<=cursorRow; i++) {
        for (j=0; j<=MAX_NUMB_OF_COLUMNS; j++) {
            vidmem[i*2*MAX_NUMB_OF_COLUMNS+j*2]=' ';
            vidmem[i*2*MAX_NUMB_OF_COLUMNS+j*2+1]=0x0;//black colour
        }
    }
    cursorColumn=0;
    cursorRow=0;
}





//moves the cursor position to a new one
//scrolls the content if needed
void setCursorPosition(int newRow ,int newColumn)
{
    if (newRow>cursorRow || newColumn>=cursorColumn) cursorColumn=newColumn;
    if (newRow>=cursorRow) cursorRow=newRow;
    scrollIfNeeded(cursorRow,cursorColumn);
    
}

//clears the character at the current position
void clrchar(){
    cursorColumn--;
    if (cursorColumn==-1) {
        cursorColumn=MAX_NUMB_OF_COLUMNS;
        cursorRow--;
        if (cursorRow==-1) {
            cursorRow=0;
        }
    }
    vidmem[cursorRow*2*MAX_NUMB_OF_COLUMNS+cursorColumn*2]=' ';
    vidmem[cursorRow*2*MAX_NUMB_OF_COLUMNS+cursorColumn*2+1]=0x0;
    
}


int strcmp(char* str1, char* str2)
{
    int i=0;
    while (str1[i]!='\0' && str2[i]!='\0') {
        if (str1[i]>str2[i]) return 1;
        else if (str1[i]<str2[i]) return -1;
        i++;
    }
    if (str1[i]=='\0' && str2[i]=='\0') return 0;
}

char* strcpy(char* str1, char* str2){
    int i=-1;
    do {
        i++;
        str1[i]=str2[i];
        
    }
    while (str2[i]!='\0');
    return str1;
}





char* gets(char *string){
    int i=0;
    char c;
    while ((c =getchar())!='\n') {
        if (c ==127){ //DEL ASCII code (backspace on macBook)
            if (i>0){ //we do not want to erease a char if there is none
                i--;
                clrchar();
            }
            continue;
        }
        string[i]=c;
        putc(string[i]);
        i++;
    }
    string[i]='\0'; //make this char array a string
    return string;
}

