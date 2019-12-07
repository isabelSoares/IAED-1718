#ifndef _HEADFILE_
#define _HEADFILE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXINPUT 100000
#define MAXCHAR 8003
#define MAXDEPENDENCIES 10000


typedef struct task * ptask;/*pointer for task */

/*struct that has all characteristics' task */
typedef struct task {
    unsigned long int id,duration,ndependencies;
    char description[MAXCHAR];
    ptask dependencies[MAXDEPENDENCIES];
} task_s;

/*struct that has double linked list with all tasks */
typedef struct node{
    struct task * task;
    struct node *prev, *next;
}listTask;


/*HEAD FILES*/
void addTask(char *string,struct node **head);
void Tasks_with_duration(unsigned long int  mode, struct node *listTask);
void dependentTasks(unsigned long int id, struct node **head);
void removeTask(char *string,struct node **head);
void ListTasks();

#endif