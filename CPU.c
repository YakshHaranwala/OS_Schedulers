/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "task.h"

// run this task for the specified time slice
void run(Task *task, int slice, int cur_time, int tid) {
    printf("%d\t %s\t %d\t %d\n", cur_time, task->name, tid, slice);
}
