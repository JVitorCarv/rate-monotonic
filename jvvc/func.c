#include "func.h"

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
    if (strcmp(previous.task_name, current.task_name) != 0 && previous.time_unit > 0) {
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

int check_argc(int argc) {
    if (argc != 2) {
        if (argc <= 1) {
            printf("Please provide at least one input file\nTerminating program...\n");
        } else {
            printf("Please, provide only one input file\nTerminating program...\n");
        }
        return 1;
    }
    return 0;
}