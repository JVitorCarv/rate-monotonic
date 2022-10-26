#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int period;
    int original_period;
    int time_unit;
    char* task_name;
}Task;


void print_task(Task task) {
    printf("%s %d %d\n", task.task_name, task.period, task.time_unit);
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
    Task* found_tasks = (Task*)malloc(2*sizeof(Task));  //Will store all unique tasks

    found_tasks[0].period = 50;
    found_tasks[0].original_period = 50;
    found_tasks[0].time_unit = 25;
    found_tasks[0].task_name = "T1";

    found_tasks[1].period = 80;
    found_tasks[1].original_period = 80;
    found_tasks[1].time_unit = 35;
    found_tasks[1].task_name = "T2";
    
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

    int insertion_timestamps[total_tasks][total_exe_time];
    //Initialize matrix as 0
    for (int i = 0; i < total_tasks; i++) {
        for (int j = 0; j < total_exe_time; j++) {
            insertion_timestamps[i][j] = 0;
        }
    }
    // Populates insertion_timestamps
    for (int i = 0; i < total_tasks; i++) {
        for (int j = 1; j < total_exe_time; j++) {
            if (ordered_tasks[i].period * j > total_exe_time) {
                break; 
            }
            else {
                insertion_timestamps[i][j] = ordered_tasks[i].period * j;
            }
        }
    }
    // Checking print
    /*
    for (int i = 0; i < total_tasks; i++) {
        for (int j = 0; j < total_exe_time; j++) {
            printf("%d ", insertion_timestamps[i][j]);
        }
        printf("\n");
    }*/

    int time_passed = 0;
    int original_exe_time = total_exe_time;
    while (total_exe_time > 0) {
        int task_exe_time = 0;
        time_passed = original_exe_time - total_exe_time;
        
        // Tries to find a task which has not finished yet
        for (int i = 0; i < total_tasks; i++) {
            if (ordered_tasks[i].time_unit > 0) {
                task_exe_time = ordered_tasks[i].time_unit;
                /* Is there any other higher priority task that will arrive
                during execution? */
                int new_task_exe_time = -1;
                if (i > 0) {
                    for (int j = 0; j < i; j ++) {
                        for (int k = 0; k < original_exe_time; k++) {
                            if (insertion_timestamps[j][k] >= time_passed && insertion_timestamps[j][k] <= time_passed + task_exe_time) {
                                new_task_exe_time = insertion_timestamps[j][k] - time_passed;
                                printf("%d", new_task_exe_time);
                            }
                        }
                    }
                }
                //if (new_task_exe_time > -1) task_exe_time = new_task_exe_time;
                /* Will the task go over its own period of execution? */ 
                /* Will the task go overt the total deadline? */
                /*for (int j = 0; j < total_tasks; j++) {
                    if (j != i) {
                        for (int k = 0; k < original_exe_time; k++) {
                            if (insertion_timestamps[j][k] >= time_passed && insertion_timestamps[j][k] <= task_exe_time + time_passed) {//Verify later
                                ordered_tasks[i].time_unit = insertion_timestamps[j][1]; 
                            }
                        }
                    }
                }*/

                total_exe_time -= task_exe_time;
                printf("%s executed for %d u.t.\n", ordered_tasks[i].task_name, task_exe_time);
                ordered_tasks[i].time_unit -= task_exe_time;
                break;
            }
        }
        int finished = 0;
        for (int i = 0; i < total_tasks; i++) {
            if (ordered_tasks[i].time_unit > 0) {
                //printf("There is still something to be executed\n");
                finished = 1;
            }
        }
        if (finished == 0) {
            printf("CPU Idle for %d\n", total_exe_time);
            total_exe_time = 0;
        }
    }
    
    return 0;
}