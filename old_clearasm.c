int clearAsm() {
    FILE* input = fopen("input.fasm", "r");
    FILE* inputJumps = fopen("input_jumps.fasm", "w");
 
    char line[100];
    char commandStek[100];
    int lineNum = 0;
 
    while (fscanf(input, "%[^\n]\n", line) != EOF) {
        int i = 0;
        //int j;
        while ((i < 100) && (line[i] != ';') && (line[i] != ':')) {
            commandStek[i] = line[i];
            i++;
        }
        commandStek[i] = '\0';
 
        //printf("%s %d\n", commandStek, i);
 
        if (i != 0) {
            if (line[i] == ':') {
                labels[labelsCount] = malloc(100 * sizeof(char));
                strcpy(labels[labelsCount], commandStek);
                if ((labelsCount % 10) == 0) {
                    labelsNums = (int*) realloc(labelsNums, (labelsCount + 10) * sizeof(int));
                }
                labelsNums[labelsCount] = lineNum - labelsCount;
                labelsCount++;
            }
            lineNum++;
        }
    }
   
    fseek(input, 0L, SEEK_SET);
 
    while (fscanf(input, "%[^\n]\n", line) != EOF) {
        int i = 0;
        int j;
        while ((i < 100) && (line[i] != ';') && (line[i] != ':')) {
            commandStek[i] = line[i];
            i++;
        }
        commandStek[i] = '\0';
        //printf("%s %d\n", commandStek, i);
 
        if (i != 0) {
            if (line[i] != ':') {
                char command[10];
                char second[10];
                sscanf(commandStek, "%s %s", command, second);
                int commandCode = getCommandCode(command);
                if (((commandCode >= 46) && (commandCode <= 52)) || !strcmp(command, "end") || !strcmp(command, "call") || !strcmp(command, "calli")) {
                    for (j = 0; j < labelsCount; j++) {
                        if (!strcmp(labels[j], second)) {
                            fprintf(inputJumps, "%s %d\n", command, labelsNums[j]);
                            if (!strcmp(command, "end"))
                                startCommand = labelsNums[j];
                            break;
                        }
                    }
                } else {
                    fprintf(inputJumps, "%s\n", commandStek);
                }
            }
        }
    }      
    fclose(input);
    fclose(inputJumps);
    return 0;
}