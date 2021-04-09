/**
 * <schedule_rr.c>
 * Name: Yaksh J Haranwala
 * Student ID: 201907532
 * 
 * This file has the FCFS schdeuler for the project. The purpose of the scheduler is 
 * to schedule the tasks based on Round - Robin basis. Scheduling metrics like Average 
 * Response time, Average wait time and Average turnaround time is also calculated.
 **/
#define TRUE 1

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
void reload_process(Task *);

int get_num_tasks();
double average_response_time();
double average_wait_time();
double average_turnaround_time();

int *burst_times;
int *exit_times;
int *start_times;

struct node *task_list = NULL;  // A linked list for holding the tasks.
struct node *to_pick;

int cur_time;   // For calculating the current tiem of the system and then prinitng.
int num_tasks;

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
 * according to the RR policy. The time quantum is 10.
 * 
 * @return the task which has been picked.
 **/
Task *pickNextTask(){
    struct node *iter = task_list;
    while(iter->next != NULL){
        iter = iter->next;
    }
    return iter->task;
}

/**
 * Schedule the tasks and print the output.
 **/
void schedule(){
    num_tasks = get_num_tasks();

    burst_times = calloc(num_tasks, sizeof(int));
    exit_times = calloc(num_tasks, sizeof(int));
    start_times = calloc(num_tasks, sizeof(int));

    struct node *iter = task_list;
    int i = 0;
    while(iter != NULL){
        burst_times[i] = iter->task->burst;
        i++;
        iter = iter->next;
    }

    printf("Time\t Name\t tid\t Burst Duration\n");
    int exit_index = 0;
    int start_index = 0;
    while(task_list != NULL){
        /* Pick the next time and schedule it to run. */
        Task *task = pickNextTask();
        
        int slice = 0;
        if(QUANTUM < task->burst){
            slice = QUANTUM;
        }else{
            slice = task->burst;
        }

        run(task, slice, cur_time, task->tid);
        task->burst = task->burst - slice;

        /* Filling the burst_time, exit_time and start_time array for calculating of various times.*/        
        if(start_times[start_index] == 0) {
            start_times[start_index] = cur_time;
            start_index++;
        }       

        if(task->burst <= 0){
            exit_times[exit_index] = cur_time + slice;
            delete(&task_list, task);
            release_pid(task->tid);
            exit_index++;
        }else{
            delete(&task_list, task);
            reload_process(task);
        }
        cur_time += slice;   
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

/**
 * Reload the task with the same tid and he the new remaining burst time
 **/
void reload_process(Task *temp){
    Task *to_load = malloc(sizeof(Task));

    to_load->name = malloc(sizeof(char) * (strlen(temp->name) + 1));
    strcpy(to_load->name, temp->name);

    to_load->tid = temp->tid;
    to_load->burst = temp->burst;
    to_load->priority = temp->priority;

    insert(&task_list, to_load);
}