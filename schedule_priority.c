/**
 * <schedule_priority.c>
 * Name: Yaksh J Haranwala
 * 
 * This file has the Highest Priority scheduler for the project. The purpose of the scheduler is 
 * to schedule the tasks based on Highest Priority First basis. Scheduling metrics like 
 * Average Response time, Average wait time and Average turnaround time is also
 * calculated.
 **/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "schedulers.h"
#include "task.h"
#include "cpu.h"
#include "list.h"
#include "pid_manager.h"

void add(char *, int, int);
Task * pickNextTask();
void schedule();

int get_num_tasks();
double average_response_time();
double average_wait_time();
double average_turnaround_time();

int *burst_times;
int *exit_times;
int *start_times;

struct node *task_list = NULL;  // A linked list for holding the tasks.

int cur_time;   // For calculating the current tiem of the system and then prinitng.
int num_tasks;  // Total number of tasks in the queue.

/**
 * Add a task to the linked list containing all the tasks.
 **/
void add(char *taskName, int taskPriority, int burstTime){
    Task * temp_task = malloc(sizeof(Task)); //Creating a temporary task.

    /* Copying the name of the task from file to actual task.*/
    temp_task->name = malloc(sizeof(char) * (strlen(taskName) + 1));
    strcpy(temp_task->name, taskName);

    temp_task->tid = allocate_pid();
    temp_task->burst = burstTime;
    temp_task->priority = taskPriority;

    insert(&task_list, temp_task);
}


/**
 * Pick the next task available from the list. This task is picked
 * according to the Highest Priority first policy.
 * 
 * @return the task which has been picked.
 **/
Task *pickNextTask(){
    struct node *iter = task_list;
    Task *highest_priority = iter->task;
    while(iter != NULL){
        if(iter->task->priority >= highest_priority->priority){
            highest_priority = iter->task;
        }
        iter = iter->next;
    }

    return highest_priority;
}


/**
 * Schedule the tasks and print the output.
 **/
void schedule(){
    num_tasks = get_num_tasks();

    burst_times = calloc(num_tasks, sizeof(int));
    exit_times = calloc(num_tasks, sizeof(int));
    start_times = calloc(num_tasks, sizeof(int));

    int index = 0;
    printf("Time\t Name\t tid\t Burst Duration\n");
    while(task_list != NULL){
        /* Pick the next time and schedule it to run. */
        Task *task = pickNextTask();
        run(task, task->burst, cur_time, task->tid);
        
        /* Filling the burst_time, exit_time and start_time array for calculating of various times.*/
        burst_times[index] = task->burst;
        exit_times[index] = cur_time + task->burst;
        start_times[index] = cur_time;
        cur_time += task->burst;

        release_pid(task->tid);
        delete(&task_list, task);
        index++;
    }
    printf("Average turnaround time: %.2f\n", average_turnaround_time());
    printf("Average wait time: %.2f\n", average_wait_time());
    printf("Average response time: %.2f\n", average_response_time());
}


/**
 * Calculate the average Turn around time of the algorithm.
 **/
double average_turnaround_time(){
    double to_return = 0.0;
    for(int i = 0; i < num_tasks; ++i){
        to_return += exit_times[i];
    }

    return to_return/num_tasks;
}

/**
 * Calculate the average waiting time of the algorithm.
 **/
double average_wait_time(){
    double to_return = 0.0;
    for(int i = 0; i < num_tasks; ++i){
        to_return += exit_times[i] - burst_times[i];
    }

    return to_return/num_tasks;
}

/**
 * Calculate the average response time of the algorithm.
 **/
double average_response_time(){
    double to_return = 0.0;
    for(int i = 0; i < num_tasks; i++){
        to_return += start_times[i];
    }

    return to_return/num_tasks;
}


/**
 * Find the number of tasks that are to be scheduled.
 **/
int get_num_tasks(){
    struct node *iter = task_list;
    int to_return = 0;
    while(iter != NULL){
        to_return++;
        iter = iter->next;
    }

    return to_return;
}
