#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int period;
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
    found_tasks[0].time_unit = 25;
    found_tasks[0].task_name = "T1";

    found_tasks[1].period = 80;
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
                /* Will the task go over its own period of execution? */ 

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
        printf("CPU Switch\n");
    }
    
    return 0;
}