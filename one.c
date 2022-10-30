#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


void print_task(Task task) {
    printf("%s %d %d\n", task.task_name, task.period, task.time_unit);
}

int print_if_finished(Task* task, int* tc, FILE* file) {
    if (task->time_unit == 0) {
        /* If a determined task hits zero, then there is nothing left to process, therefore, it finished executing */
        fprintf(file, "[%s] for %d units - F\n", task->task_name, *tc);
        task->completed_count += 1;
        *tc = 0;

        return 1;
    }
    return 0;
}

int print_if_hold(Task previous, Task current, int* tc, FILE* file) {
    if (strcmp(previous.task_name, current.task_name) && previous.time_unit > 0) {
        //printf("[%s] was changed to %s\n", previous.task_name, current.task_name);
        fprintf(file, "[%s] for %d units - H\n", previous.task_name, *tc);
        *tc = 0;

        return 1;
    }
    return 0;
}

int print_if_lost(Task previous, Task current, int* tc, FILE* file) {
    // If the value of tc is 0, it means that the process was not lost since it did not execute and a new instance
    // of the same task is currently executing in the CPU
    if (strcmp(previous.task_name, current.task_name) == 0 && previous.time_unit < current.time_unit && *tc > 0) {
        fprintf(file, "[%s] for %d units - L\n", previous.task_name, *tc);
        *tc = 0;

        return 1;
    }
    return 0;
}

int print_if_killed(int count, int total, Task current, int* tc, FILE* file) {
    if (count + 1 == total && current.time_unit > 0) {
        fprintf(file, "[%s] for %d units - K\n", current.task_name, *tc);
        *tc = 0;

        return 1;
    }
    return 0;
}

void print_task_array(Task* array, int size) {
    for (int i = 0; i < size; i++) {
        print_task(array[i]);
    }
}

void order_tasks(Task* array, int size) {
    // Bubblesort code reutilized from a personal repository
    Task temp;
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1; j++) {
            if (array[j].period > array[j + 1].period) {
                temp = array[j + 1];
                array[j + 1] = array[j];
                array[j] = temp;
            }
        }
    }
}

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
    
    int line_count = 0;   
    
    fgets(line, MAX_LINE, file);
    total_exe_time = atoi(line); /* Somar 1 e depois subtrair */
    
    for (int i = 0; i < total_tasks; i++) {
        fscanf(file, "%s%d%d", found_tasks[i].task_name, &found_tasks[i].period, &found_tasks[i].time_unit);
        found_tasks[i].original_period = found_tasks[i].period;
        found_tasks[i].original_time_unit = found_tasks[i].time_unit;
        found_tasks[i].completed_count = 0;
        found_tasks[i].lost_count = 0;
    }
    print_task_array(found_tasks, total_tasks);

    fclose(file);
    printf("%d", line_count);
    Task ordered_tasks[total_tasks];
    printf("%d", total_tasks);

    // Copy found_tasks to ordered_tasks
    for (int i = 0; i < total_tasks; i++) {
        ordered_tasks[i] = found_tasks[i];
    }

    order_tasks(ordered_tasks, total_tasks);

    int count = 0;
    int task_count = 0;
    int last_print = 0;
    int idle_count = 0;

    /* Initialize empty previous task */
    Task previous;
    previous.period = 0;
    previous.original_period = previous.period;
    previous.time_unit = 0;
    previous.original_time_unit = previous.time_unit;
    memset(previous.task_name, '\0', 64*sizeof(char));
    print_task_array(ordered_tasks, total_tasks);

    file = fopen("rate_jvvc.out", "w");
    fprintf(file, "EXECUTION BY RATE\n");
    while (count < total_exe_time) {
        
        //Updates periods
        for (int i = 0; i < total_tasks; i++) {
            if (ordered_tasks[i].period <= count) {
                //printf("At count %d\n", count);
                //printf("%d += %d\n", ordered_tasks[i].period, ordered_tasks[i].original_period);
                if (ordered_tasks[i].time_unit > 0) {
                    ordered_tasks[i].lost_count += 1;
                }
                ordered_tasks[i].period += ordered_tasks[i].original_period;
                ordered_tasks[i].time_unit = ordered_tasks[i].original_time_unit;
                //printf("Update period\n");
            }
        }

        int switched = 0;
        int found = 0;
        for (int i = 0; i < total_tasks; i++) {
            if (ordered_tasks[i].time_unit > 0) {

                /* If this if statement is entered, it means that a task with execution to be made was found. */
                if (idle_count > 0) {
                    /* An idle_count higher than zero means that for some moment in a previous loop, the
                    CPU was idle. Therefore, it must be printed here*/
                    fprintf(file, "idle for %d units\n", idle_count);
                }
                
                ordered_tasks[i].time_unit--;
                print_if_hold(previous, ordered_tasks[i], &task_count, file);                  /* If a task is switched to another task */
                print_if_lost(previous, ordered_tasks[i], &task_count, file);                  /* If a task is lost */
                task_count++;                                                            /* Task count needs to be here, or else the count would go to the previous task*/

                //printf("[%s] %d\n", ordered_tasks[i].task_name, ordered_tasks[i].time_unit); /* Debug print */
                print_if_finished(&ordered_tasks[i], &task_count, file);            /* If a task finishes, print to the console */
                print_if_killed(count, total_exe_time, ordered_tasks[i], &task_count, file);   /* If a task is killed */                   /* Updates the task_count for the current execution */

                previous = ordered_tasks[i];                                /* Finally, the current task becomes the previous task */
                found = 1;
                break;
            }
        }

        /* If no task was found, the idle_count is incremented, meaning that the CPU was idle */
        if (!found) {
            idle_count++;
            //printf("idle, %d\n", idle_count);
        } else {
            /* If a task is found, the idle_count must be resetted, since CPU is no longer idle */
            idle_count = 0;
        }

        /* If the execution finishes while the CPU was idle, print */
        if (count + 1 == total_exe_time && idle_count > 0) {
            fprintf(file, "idle for %d units\n", idle_count);
        }
        count++; /* Another clock finishes */
    }
    fprintf(file, "\n");

    fprintf(file, "LOST DEADLINES\n");
    for (int i = 0; i < total_tasks; i++) {
        fprintf(file, "[%s] %d\n", ordered_tasks[i].task_name, ordered_tasks[i].lost_count);
    }
    fprintf(file, "\n");

    fprintf(file, "COMPLETE EXECUTION\n");
    for (int i = 0; i < total_tasks; i++) {
        fprintf(file, "[%s] %d\n", ordered_tasks[i].task_name, ordered_tasks[i].completed_count);
    }
    fprintf(file, "\n");

    fprintf(file, "KILLED");
    for (int i = 0; i < total_tasks; i++) {
        fprintf(file, "\n");
        /* Uncomment depending if it counts when the tasks are instance at the same time of total_exe_time */
        if (ordered_tasks[i].time_unit > 0 /*|| ordered_tasks[i].period == total_exe_time*/) {
            fprintf(file, "[%s] %d", ordered_tasks[i].task_name, 1);
        } else {
            fprintf(file, "[%s] %d", ordered_tasks[i].task_name, 0);
        }
    }
    fclose(file);

    //printf("%d", idle_count); // Should be zero
    print_task_array(ordered_tasks, total_tasks);
    
    return 0;
}