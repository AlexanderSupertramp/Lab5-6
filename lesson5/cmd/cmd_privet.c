void cmd_privet(char args[5][40])
{
    int i;
    puts("Aloha");
    for (i=0; i<5; i++) {
        if (args[i][0]!=' ') {
            puts(", ");
            puts(args[i]);
        }
    }
    newLine();
    
}