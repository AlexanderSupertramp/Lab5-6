void cmd_whatIs(char *arg){
    if (!strcmp(arg,"life")) {
        puts("42");
    }
    else if (!strcmp(arg,"4+1")){
        puts("5");
    }
    else if (!strcmp(arg,"FAF")){
        puts("Filiera Anglofona");
    }
    newLine();
}