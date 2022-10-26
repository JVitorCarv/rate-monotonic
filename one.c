#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int period;
    int original_period;
    int time_unit;
    int original_time_unit;
    char* task_name;
}Task;


void print_task(Task task) {
    printf("%s %d %d\n", task.task_name, task.period, task.time_unit);
}

int print_if_finished(Task task, int* tc) {
    if (task.time_unit == 0) {
        /* If a determined task hits zero, then there is nothing left to process, therefore, it finished executing */
        printf("[%s] for %d units - F\n", task.task_name, *tc);
        *tc = 0;

        return 1;
    }
    return 0;
}

int print_if_hold(Task previous, Task current, int* tc) {
    if (strcmp(previous.task_name, current.task_name) && previous.time_unit > 0) {
        //printf("[%s] was changed to %s\n", previous.task_name, current.task_name);
        printf("[%s] for %d units - H\n", previous.task_name, *tc);
        *tc = 0;

        return 1;
    }
    return 0;
}

int print_if_lost(Task previous, Task current, int* tc) {
    if (strcmp(previous.task_name, current.task_name) == 0 && previous.time_unit < current.time_unit) {
        printf("[%s] for %d units - L\n", previous.task_name, *tc);
        *tc = 0;

        return 1;
    }
    return 0;
}

int print_if_killed(int count, int total, Task current, int* tc) {
    if (count + 1 == total && current.time_unit > 0) {
        printf("[%s] for %d units - K\n", current.task_name, *tc);
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
    printf("File name provided: %s\n", argv[1]);        
    int total_exe_time = 165;                           
    int total_tasks = 2;                                
    Task* found_tasks = (Task*)malloc(total_tasks*sizeof(Task));  //Will store all unique tasks

    /*  CASO DO ARQUIVO  */
    found_tasks[0].period = 50;
    found_tasks[0].original_period = found_tasks[0].period;
    found_tasks[0].time_unit = 25;
    found_tasks[0].original_time_unit = found_tasks[0].time_unit;
    found_tasks[0].task_name = "T1";

    found_tasks[1].period = 80;
    found_tasks[1].original_period = found_tasks[1].period;
    found_tasks[1].time_unit = 35;
    found_tasks[1].original_time_unit = found_tasks[1].time_unit;
    found_tasks[1].task_name = "T2";

    /*  CASO DO SLIDE  */
    /*
    found_tasks[0].period = 50;
    found_tasks[0].original_period = found_tasks[0].period;
    found_tasks[0].time_unit = 20;
    found_tasks[0].original_time_unit = found_tasks[0].time_unit;
    found_tasks[0].task_name = "T1";

    found_tasks[1].period = 100;
    found_tasks[1].original_period = found_tasks[1].period;
    found_tasks[1].time_unit = 35;
    found_tasks[1].original_time_unit = found_tasks[1].time_unit;
    found_tasks[1].task_name = "T2";
    */

    /*
    found_tasks[2].period = 30;
    found_tasks[2].original_period = found_tasks[2].period;
    found_tasks[2].time_unit = 15;
    found_tasks[2].original_time_unit = found_tasks[2].time_unit;
    found_tasks[2].task_name = "T3";
    */
    
    Task ordered_tasks[total_tasks];

    // Copy found_tasks to ordered_tasks
    for (int i = 0; i < total_tasks; i++) {
        ordered_tasks[i] = found_tasks[i];
    }

    printf("%d\n", total_exe_time);

    printf("Found tasks\n");
    print_task_array(found_tasks, total_tasks);
    order_tasks(ordered_tasks, total_tasks);
    printf("Ordered tasks\n");
    print_task_array(ordered_tasks, total_tasks);

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
    previous.task_name = "\0";

    while (count < total_exe_time) {
        
        //Updates periods
        for (int i = 0; i < total_tasks; i++) {
            if (ordered_tasks[i].period <= count) {
                //printf("At count %d\n", count);
                //printf("%d += %d\n", ordered_tasks[i].period, ordered_tasks[i].original_period);
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
                    printf("idle for %d units\n", idle_count);
                }
                
                ordered_tasks[i].time_unit--;
                print_if_hold(previous, ordered_tasks[i], &task_count);                  /* If a task is switched to another task */
                print_if_lost(previous, ordered_tasks[i], &task_count);                  /* If a task is lost */
                task_count++;                                                            /* Task count needs to be here, or else the count would go to the previous task*/

                //printf("[%s] %d\n", ordered_tasks[i].task_name, ordered_tasks[i].time_unit); /* Debug print */
                print_if_finished(ordered_tasks[i], &task_count);            /* If a task finishes, print to the console */
                print_if_killed(count, total_exe_time, ordered_tasks[i], &task_count);   /* If a task is killed */                   /* Updates the task_count for the current execution */

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
            printf("idle for %d units\n", idle_count);
        }
        count++; /* Another clock finishes */
    }
    //printf("%d", idle_count); // Should be zero
    print_task_array(ordered_tasks, total_tasks);
    
    return 0;
}