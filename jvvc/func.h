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

void print_task(Task task);

int print_if_finished(Task* task, int* tc, FILE* file);

int print_if_hold(Task previous, Task current, int* tc, FILE* file);

int print_if_lost(Task previous, Task current, int* tc, FILE* file);

int print_if_killed(int count, int total, Task current, int* tc, FILE* file);

void print_task_array(Task* array, int size);

void order_tasks(Task* array, int size);

int check_argc(int argc);
