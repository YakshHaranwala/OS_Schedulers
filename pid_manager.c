/** <q2_pid_manager.c>
 * Submitted by: Yaksh J Haranwala
 * 
 * This code is the solution to Assignment 2 Question 2.
 * The purpose of the Code is to create a PID manager that allocates and removes PID when the functions
 * are called to do so. Note that the PIDs assigned are within a specified range and the PIDs cannot 
 * exceed the range. Also note that all the PIDs, are stored in a linked list in this particular implementation
 * of the PID manager. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "pid_manager.h"

#define MIN 300
#define MAX 5000
#define False 0
#define True 1

tid_node *head = NULL;
tid_node *tail = NULL;
pthread_mutex_t lock;

/**
 * Initiate the data structure (Linked list) in our case for representing all the pids.
 * Return 1 if the initiation of the data structure was successful or -1 otherwise. 
 *
 * @return the status indicating whether the initiation was successful or not.
 */
int allocate_map(void){
    pthread_mutex_init(&lock, NULL);
    head = (tid_node *)malloc(sizeof(tid_node));
    tail = (tid_node *)malloc(sizeof(tid_node));
    if ((head == NULL) || (tail == NULL)){
        return -1;
    }else{
        return 1;
    }
}

/**
 * Allocate and return the allocated PID.
 * Return -1 if the PID allocation was unsuccessful.
 * 
 * @return The PID allocated or -1 if the allocation was unsuccessful.
 */
int allocate_pid(void){
    pthread_mutex_lock(&lock);
    tid_node *temp = (tid_node *)malloc(sizeof(tid_node));
    int to_allocate = MIN;

    if (head == NULL){      //Check whether a LL has been initiated for storing the PIDs.
        printf("No Data Structure found for storing PIDs.\nPlease initiate a PID Map before allocating a PID.\n");
        return -1;
    }else if(head->pid == 0){   //Check whether the head is declared but not initiated and if so, then initiate it. 
        head->pid = MIN;
        head->next = NULL;
        tail = head;
        return head->pid;
    }else if(head->pid != MIN){   // Check whether the head pid has been released and if so, then allocate it again.
        temp->pid = MIN;
        temp->next = head;
        head = temp;
        return temp->pid;
    }else{
        tid_node *curr = head;
        tid_node *prev = NULL;
        int found = False;  // Sentinel value for loop termination.
        
        /* Iterate over the LL and check if there is any space left in the LL for allocating another PID. */
        while(curr != NULL && (found != True)){
            if(curr->pid >= MAX){   // Check if all the PIDs have been allocated or not.
                printf("No Memory Left\n");
                return -1;
            }else if(prev == NULL){  // If it is the first iteration of the Loop, then initiate prev and continue execution.
                prev = curr;         // This is to ensure that we dont get a segmentation fault and initiate both prev and
                curr = curr->next;   // curr appropriately in the process of finding an empty spot.
                ++to_allocate;
            }else if(curr->pid != (prev->pid + 1)){ // Check whether the difference between 2 PIDs linked to each other is 
                to_allocate = prev->pid + 1;        // exactly 1, and if not, then we have found an empty spot.
                found = True;                       
            }else{
                prev = curr;
                curr = curr->next;
                ++to_allocate;
            }
        }
    }
    
    
    int iterator = MIN;     // Iterator sentinel.
    tid_node *iter = head;
    tid_node *previous = NULL;
    /* Iterate till the "iter" node has not reached the position before which the new PID node is to be inserted. */
    while(iterator < to_allocate){
        previous = iter;
        iter = iter->next;
        iterator++;
    }
    
    temp->pid = to_allocate;    // Allocate and insert the PID in the LL.
    temp->next = iter;
    previous->next = temp;
    return temp->pid;
    pthread_mutex_unlock(&lock);
}

/**
 * Release the PID with the given PID.
 * If the PID to be released does not exit, then print an error to the console.
 *
 * @param pid is the PID that is to be released.
 */
void release_pid(int pid){
    pthread_mutex_lock(&lock);
    tid_node *temp = head;
    tid_node *prev = NULL;
    /* Check whether the head Node of the LL is to be released and if so, then move the head to the next node */
    if(head->pid == pid){
        head = temp->next;
        free(temp);
        return;
    }else{
        while((temp != NULL) && (temp->pid != pid)){
            prev = temp;
            temp = temp->next;
        }
    }
    
    if (temp == NULL){
        printf("The pid does not exist\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
    pthread_mutex_unlock(&lock);
}
