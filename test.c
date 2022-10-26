#include <stdio.h>
#include <stdlib.h>

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
    found_tasks[0].original_period = found_tasks[0].period;
    found_tasks[0].time_unit = 25;
    found_tasks[0].original_time_unit = found_tasks[0].time_unit;
    found_tasks[0].task_name = "T1";

    found_tasks[1].period = 80;
    found_tasks[1].original_period = found_tasks[1].period;
    found_tasks[1].time_unit = 35;
    found_tasks[1].original_time_unit = found_tasks[1].time_unit;
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

    int original_exe_time = total_exe_time;
    int count = 0;
    while (count < total_exe_time) {
        int task_exe_time = 0;

        for (int i = 0; i < total_tasks; i++) {
            if (ordered_tasks[i].period < count) {
                ordered_tasks[i].period += ordered_tasks[i].original_period;
            }
        }
        
        // Tries to find a task which has not finished yet
        for (int i = 0; i < total_tasks; i++) {
            if (ordered_tasks[i].time_unit > 0) {
                task_exe_time = ordered_tasks[i].time_unit;

                /* Will the task go over the total deadline? */
                if (task_exe_time + count > total_exe_time) {
                    task_exe_time = total_exe_time - count;
                    // This means that the process was killed
                    // and others different from zero should die too
                }
                /* Will the task go over its own period of execution? */ 
                if (task_exe_time + count > ordered_tasks[i].period) {
                    task_exe_time = ordered_tasks[i].period - count;
                    // This means that the process was lost
                }

                if (task_exe_time + count > ordered_tasks[0].period) {
                    task_exe_time = ordered_tasks[0].period - count;
                }

                /* Is there any other higher priority task that will arrive
                during execution? */


                count += task_exe_time;
                printf("%s executed for %d u.t.\n", ordered_tasks[i].task_name, task_exe_time);
                ordered_tasks[i].time_unit -= task_exe_time;
                break;
            }
        }
        
        
    }
    print_task_array(ordered_tasks, total_tasks);
    
    return 0;
}