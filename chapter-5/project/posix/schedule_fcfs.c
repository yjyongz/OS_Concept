#include <stdio.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "cpu.h"
#include "stdlib.h"

struct node *head = NULL;
void run(Task *task, int slice);

int add(char* task_name, int priority, int burst) {
    Task *t = (Task*)calloc(1, sizeof(Task));
    t->name = strdup(task_name);
    t->tid = -1;
    t->priority = priority;
    t->burst = burst;
    printf("added name:%s priority:%d burst:%d\n", task_name, priority, burst);
    insert(&head, t);
    return 0;
}

void fcfs_schedule(struct node *tn) {
    if (!tn) return;
    fcfs_schedule(tn->next);
    run(tn->task, 0);
    delete(&head, tn->task); 
    free(tn->task->name);
    free(tn);

}

void schedule() {
    fcfs_schedule(head);
}
