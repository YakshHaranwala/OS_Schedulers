/**
 * Struct for representing the nodes for the Linked List.
 */
typedef struct linked_list{
    struct linked_list *next;       // Pointer to the next PID in the list.
    int pid;                        // PID .
}tid_node;

int allocate_map(void);
int allocate_pid(void);
void release_pid(int);