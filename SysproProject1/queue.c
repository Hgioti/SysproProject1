#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>


struct QueueN {

    char str[BUFSIZ];
    int count;
    int av;
    int fd;
    struct QueueN* next;
};
 



/* Functions for workers */

void pushPid(struct QueueN** head, int pid,char *fifoname,int fd)
{
        printf("Creating worker %d\n",pid);
        struct QueueN* N = (struct QueueN*)malloc(sizeof(struct QueueN));

        N->count = pid;
        N->av=0;
        strcpy(N->str,fifoname);
        N->fd=fd;
        N->next = (*head);
        (*head) = N;
}



void changeAvailability(struct QueueN* head, int pid)
{
    /* Search for worker and swap his availability */
    struct QueueN* curr = head;

    while (curr != NULL) {

        if(curr->count == pid){

                if(curr->av==0){
                    curr->av=1;
                    printf("Worker %d is now available\n",pid);
                }
                else if(curr->av==1){
                    curr->av=0;
                    printf("Worker %d is now occupied\n",pid);
                }

                //printf("Availability of  changed to %d\n",curr->av);
                return;

        }
        curr = curr->next;
    }

    printf("Didn't find worker\n");
}






/* Functions for searching sites */

void pushString(struct QueueN** head, char *str)
{

    struct QueueN* N = (struct QueueN*)malloc(sizeof(struct QueueN));

    N->count = 1;
    strcpy(N->str,str);

    N->next = (*head);
    (*head) = N;
}




int searchQueue(struct QueueN* head,char *key)
{

    struct QueueN* curr = head;
    while (curr != NULL) {

            if(strcmp(curr->str,key) ==0 )
            {

                 curr->count++;
                 return 1;         
            }

            curr = curr->next;
    }
    return 0;
}



/* Free memory */
void deleteQueue(struct QueueN** head)
{

   struct QueueN* curr = *head;
   struct QueueN* next;
 
   while (curr != NULL)
   {
           next = curr->next;
           free(curr);
           curr = next;
   }
   
   *head = NULL;
}