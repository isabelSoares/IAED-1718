/*Project about development's projects
Isabel Soares 89466*/

#include "task.h"

int main(int argc,char *argv[]){

    char string[MAXINPUT];
    char command[MAXINPUT];
    char *p;
    int offset;
    unsigned long int value, id;

    struct node * listTask = NULL;

    do{
        if(fgets(string,MAXINPUT,stdin)==NULL){
                        break;
                }
        sscanf(string,"%s%n",command, &offset);
        p = string + offset;
        /*remove the number of characters that
        have been read, save this value in offset
        and offset adds with string */

        if (!strcmp(command,"add")) {
            addTask(p,&listTask);
        }
        else if (!strcmp(command,"duration")){
            value = 0;
            sscanf(p,"%lu",&value);
            Tasks_with_duration(value,listTask);
        }
        else if (!strcmp(command, "depend")){
            sscanf(p,"%lu",&id);
            dependentTasks(id, &listTask);
        }
        else if (!strcmp(command, "remove")){
            removeTask(p,&listTask);
        }
        else if (!strcmp(command, "path")){

        }
    }while (strcmp(command, "exit"));

    return 0;
}


void addTask(char *string,struct node **head){

    unsigned long int id, duration,id_dependencies;
    char description[MAXCHAR];
    int offset,find,n_arg,count = 0;
    struct node * new_node, * x;
    task_s * task;
    struct node *temp_listTask;
    size_t size;
    int si;


    /*see all the information given by user*/
    n_arg = sscanf(string,"%lu%n",&id,&offset);

    if (n_arg != 1 || id <= 0){
      printf("illegal arguments\n");
      return ;
    }
    string += offset+2; /*offset used for spaces between information*/


    sscanf(string,"%[^\"]%n",description,&offset);

    size = strlen(description);
        si = (int) size;


    if (n_arg != 1 || si >= MAXCHAR){
      printf("illegal arguments\n");
      return ;
    }

    string += offset+1;

    n_arg=sscanf(string,"%lu%n",&duration,&offset);

    if (n_arg != 1 || duration <= 0){
      printf("illegal arguments\n");
      return ;
    }
    string += offset;


    temp_listTask= *head;
    while(temp_listTask !=NULL){
        if(temp_listTask->task->id == id){
            printf("id already exists\n");
            return ;
        }
        temp_listTask=temp_listTask->next;
    }

    task = malloc(sizeof(task_s));
    new_node = malloc(sizeof(struct node));

    task->id = id;
    strcpy(task -> description, description);
    task->duration = duration;
    new_node->task = task;
    new_node->next = NULL;

    /*create dependencies*/

    while(*string != '\n'){
        sscanf(string,"%lu%n",&id_dependencies,&offset);
                if (id_dependencies <= 0){
                        printf("illegal arguments\n");
                        return ;
                }
        string += offset;


        temp_listTask = *head;
        find = 0;
        while(temp_listTask !=NULL){
            if(temp_listTask->task->id == id_dependencies){
                task->dependencies[count]=temp_listTask->task;
                count++;
                find = 1;
                /*found id equal to id of dependencies given by user*/
            }
            temp_listTask = temp_listTask->next;
        }
        if (find == 0){
            printf("no such task\n");
            return ;
        }
    }
    task->dependencies[count] = NULL;

    /*add new task to listTask*/
    if (*head == NULL) {
        *head = new_node;
        (*head)->prev = NULL;
      }
    else {
        for(x = *head; x->next != NULL; x = x->next);
        x->next = new_node;
        new_node->prev = x;
    }
}

void Tasks_with_duration(unsigned long int  mode, struct node *listTask){
    int count;

    while(listTask!=NULL){
        if (listTask->task->duration >= mode){
            count = 0;
                        if (mode < 0){
                                printf("illegal arguments\n");
                                return ;
                        }
            printf("%lu \"%s\" %lu",listTask->task->id, listTask->task->description, listTask->task->duration);
            while(listTask->task->dependencies[count]!= NULL){
                printf(" %lu",listTask->task->dependencies[count]->id);
                count++;
            }
            printf("\n");
        }
        listTask=listTask->next;
    }
}

void dependentTasks(unsigned long int id, struct node **head){
    struct node *temp_listTask = *head;
    int i, found;
    if (id <= 0){
      printf("illegal arguments\n");
      return ;
    }

    found = 0;
    temp_listTask= *head;
    while(temp_listTask != NULL){
        if (temp_listTask->task ->id == id){
            found = 1;
            /*found id equal to id given by user */
            break;
        }
        temp_listTask = temp_listTask->next;
    }
    if (found == 0){
        printf("no such task\n");
        return ;
    }
    printf("%lu:",id);

    found = 0;
    while(temp_listTask != NULL){
        i = 0;
        while(temp_listTask->task-> dependencies[i]!= NULL){
            if (temp_listTask->task-> dependencies[i]->id == id){
                printf(" %lu",temp_listTask->task->id);
                found = 1;
                break;
            }
            i++;
        }
        temp_listTask = temp_listTask->next;
    }

    if (found == 0){
        printf(" no dependencies");
    }
    printf("\n");
}

void removeTask(char *string,struct node **head){
    unsigned long int id_remove;
    int find, i;
    struct node *temp_listTask, *lstTask;

    sscanf(string,"%lu",&id_remove);
    if (id_remove <= 0){
      printf("illegal arguments\n");
      return ;
    }

    find = 0;
    temp_listTask= *head;
    while(temp_listTask != NULL){
        if (temp_listTask->task ->id == id_remove){
            find = 1;
            lstTask = temp_listTask;
        }
        temp_listTask = temp_listTask->next;
    }
    if (find == 0){
        printf("no such task\n");
        return ;
    }

    temp_listTask = lstTask->next;

    while(temp_listTask != NULL){
        i = 0;
        while(temp_listTask->task-> dependencies[i]!= NULL){
            if (temp_listTask->task-> dependencies[i]->id == id_remove){
                printf("task with dependencies\n");
                return ;
            }
            i++;
        }
        temp_listTask = temp_listTask->next;
    }


    if (lstTask->prev != NULL){
        lstTask->prev->next = lstTask->next;
    }
    if (lstTask->next != NULL){
        lstTask->next->prev = lstTask ->prev;
    }

    if (*head == lstTask){
      *head = lstTask->next;
    }

    free(lstTask->task);
    free(lstTask);

}