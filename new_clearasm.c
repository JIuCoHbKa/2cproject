int clearAsm() {
    FILE* input = fopen("input.fasm", "r");
    FILE* inputCleared = fopen("input_cleared.fasm", "w");
    FILE* inputJumps = fopen("input_jumps.fasm", "w");
 
    char line[100];
    char commandStek[100];
    int lineNum = 0;
 	char c;
    char prevc = 'q';
	while (fscanf(input, "%c", &c) != EOF) {
		if ((c != '\t') && ((c == ' ') && (prevc == ' ')))
            fprintf(inputCleared, "%c", c);
		//printf("%c", c);
		if (c == ':') 
			fprintf(inputCleared, "\n");
	}

	fclose(inputCleared);
	inputCleared = fopen("input_cleared.fasm", "r");

    while (fgets(line, 100, inputCleared) != NULL) {
        int i = 0;
        
        while ((line[i] != '\n') && (line[i] != ';') && (line[i] != ':')) {
            commandStek[i] = line[i];
            i++;
        }
        commandStek[i] = '\0';
 
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
   
    fseek(inputCleared, 0L, SEEK_SET);
 
    while (fgets(line, 100, inputCleared) != NULL) {
        int i = 0;
        int j;
        while ((line[i] != '\n') && (line[i] != ';') && (line[i] != ':')) {
            commandStek[i] = line[i];
            i++;
        }
        commandStek[i] = '\0';
 
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
    fclose(inputCleared);
    fclose(inputJumps);
    free(labelsNums);
    return 0;
}