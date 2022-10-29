#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 4096

typedef struct{
    int period;
    int original_period;
    int time_unit;
    int original_time_unit;
    char task_name[64];
    int completed_count;
    int lost_count;
}Task;


int main(int argc, char**argv) {
    /* Checks if the number of arguments is valid */
    if (argc != 2) {
        if (argc <= 1) {
            printf("Please provide at least one input file\nTerminating program...\n");
        } else {
            printf("Please, provide only one input file\nTerminating program...\n");
        }
        return 0;
    }

    /* Checks whether file exists */
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File %s not found\n", argv[1]);
        return 1;
    } 
    
    char line[MAX_LINE];    /* Will store the line */
    int total_exe_time = 0; /* Will store the total execution time */

    int total_lines = 0;    /* Will store the number of lines read in the file */
    while(fgets(line, MAX_LINE, file)) {
        total_lines++;
    }
    int total_tasks = total_lines - 1;
    fclose(file);

    /* Stores file's content into found_tasks */
    
    file = fopen(argv[1], "r");

    Task* found_tasks = (Task*)malloc(total_tasks*sizeof(Task*));

    int line_count = 0;     /* Will store the number of lines read in the file */
    
    while(fgets(line, MAX_LINE, file)){
        if (line_count == 0) {
            total_exe_time = atoi(line);
        } else {
            int c = 0;
            char* token = strtok(line, " ");
            while (token != NULL && c < 3) {
                if (c == 0) {
                    strcpy(found_tasks[line_count-1].task_name, token);
                } else if (c == 1) {
                    found_tasks[line_count-1].period = atoi(token);
                } else if (c == 2) {
                    found_tasks[line_count-1].time_unit = atoi(token);
                }
                token = strtok(NULL, " ");
                c++;
            }
        }
        line_count++;
    }
    fclose(file);
    
    for (int i = 0; i < total_tasks; i++) {
        printf("%s %d %d\n", found_tasks[i].task_name, found_tasks[i].period, found_tasks[i].time_unit);
    }

}