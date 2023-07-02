#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#include "myLib.h"

static int flag = 0;
static void sig_handler(int sig_num) {

        /*  Change global variable on ^C so that the loop ends   */
        printf("\nTerminating Program\n");
        flag = 1;
}




int main(int argc, char* argv[]) {
    
    char *path;

    if(argc==3){

                if(strcmp(argv[1],"-p")!=0){
                    printf("Wrong input, second arguement is -p\n");
                    return 0;
                }

                path = malloc((strlen(argv[2])+1) * sizeof (char));
                strcpy(path, argv[2]);
                
    }
    else if(argc==1)
    {
        path=(char *)malloc(1);
        path=".";
    }
    else{
        printf("Wrong number of inputs\n");
        return 1;
    }
    



    pid_t listener,worker,x;
    int fd[2],i=0,j,count=0,av;
    int readfd, writefd,fifo_fd;


    char *r=(char *)malloc(300);
    char *c=(char *)malloc(100);
    char *fifoname=(char *)malloc(30);

    struct QueueN* newhead = NULL;
    struct QueueN* newh = NULL;
    struct QueueN* curr=NULL;

    struct stat st = {0};

    strcpy(fifoname,"/tmp/fifoname");


   if ((pipe(fd) == -1) || ((listener = fork()) == -1)) {
       perror("Failed to setup pipeline between listener and manager");
       return 1;
   }






           if(listener==0) {
                    /* Listener sends output to Manager */

                    close (fd[0]);
                    if (dup2(fd[1], 1) == -1) {
                            perror("Failed to redirect stdout of execl");
                            return 1;
                    }
                    close(fd[1]);
                

                    if (execl("/usr/bin/inotifywait", "inotifywait","-m","-e","moved_to","-e","create",path, NULL) < 0) {
                            perror("Failed to execl");
                            return 1;          
                    }
           }
           else
           {


                    /* Redirecting stdin */
                                           
                    close(fd[1]);
                    if (dup2(fd[0], 0) == -1) {
                       perror("Failed to redirect stdin");
                       return 1;
                    }
                    close(fd[0]);

                    

                    if (stat("output", &st) == -1) {
                            mkdir("output", 0777);
                    }
                    if (stat(path, &st) == -1) {
                            mkdir(path, 0777);
                    }
                    

                        /* Checking for ^C */   
                        signal(SIGINT,sig_handler);


                        char *buffer=(char *)malloc(3000);
                        char *copy=(char *)malloc(3000);
                        char *token=(char *)malloc(1000);

                        while(flag==0){
                                    

                                   /* Manager receives input from listener */

                                   strcpy(buffer,"");
                                   read(STDIN_FILENO, buffer, 300);


                                   strcpy(copy,buffer);
                                   token = strtok(copy, " ");
                                   i=0;



                                   /* Check for ^C and get out of loop */
                                   signal(SIGINT,sig_handler);        
                                   if(flag==1)
                                        break;



                                    count=0;

                                    /* Time to split text from inotifywait into words */
                                    while( token != NULL ) {
                                              i++;
                                              
                                              /* Gia  i==1 briskw Directory */
                                              /* Gia  i==2 briskw Action */
                                              /* Gia  i==3 briskw to arxeio */

                                              if(i==2){
                                                printf( "\nAction: %s ", token );
                                              }
                                              else if(i==3){  

                                                    //Dhmiourgountai \n sth le3h ta opoia apomakrunw
                                                    for(j=0;token[j]!='\0';j++){

                                                            if(token[j]=='\n'){
                                                                token[j]='\0';
                                                            }
                                                    }

                                                    strcpy(copy, token); 
                                                    
                                              }
                                              token = strtok(NULL, " ");
                                    }

                                    printf("%s\n",copy);
                                    token='\0';








                                        /* Checking for available workers */  
                                         printf("Searching for available workers\n");
                                         curr = newhead;


                                        while ((curr != NULL) && (curr->av!=1)) 
                                                    curr = curr->next;
                                                

                                        /* If no workers */
                                          if (curr==NULL){

                                                    printf("No available workers\n");

                                                    count++;            //Number of children                                    
                                                    
                                                    /*  Create a unique name for FIFO*/
                                                    (void) sprintf(c, "%d", count);
                                                    strcat(fifoname,c);

                                                    mkfifo(fifoname, 0666);
                                                    fifo_fd = open(fifoname, O_RDWR | O_NONBLOCK);



                                                    /* Create worker and add him to the queue */
                                                    if ( ((worker = fork()) == -1)) {
                                                           perror("Failed to setup worker");
                                                           return 1;
                                                    }

                                                    if(worker>0)
                                                        pushPid(&newhead,worker,fifoname,fifo_fd);
                                                        
                                                    av=1;


                                        }
                                        else{
                                                    av=0;
                                        }


                                      
                                                    /*  If a child is created enter here    */
                                                    if( (worker==0) && (av==1) ){

                                                                    while(flag==0){

                                                                          /* Worker receives the name of the file via named pipe */
                                                                            read(fifo_fd,r, 100);
                                                                            
                                                                            /* Extract sites */
                                                                            printf("Worker %d received %s\n",getpid(), r);

                                                                            SearchSites(r,path);

                                                                            printf("Worker %d stopping\n",getpid());
                                                                            raise(SIGSTOP);
                                                                   }
                                                                   return 0;
                                                                   

                                                    }
                                                    else{
                                                                  /* Manager sends the name of the file via named pipe */

                                                                    /*  If there are available workers then enter here  */
                                                                    if(av==0){

                                                                           /* Assign the worker's values to the pipe we are working on */
                                                                            worker=curr->count;
                                                                            fifo_fd=curr->fd;
                                                                            fifoname=curr->str;

                                                                            fifo_fd = open(fifoname, O_RDWR | O_NONBLOCK);

                                                                            /*  State worker unavaiable and assign him to job   */
                                                                            changeAvailability(newhead,worker);
                                                                            kill(worker, SIGCONT);
                                                                    }

                                                                    write(fifo_fd, copy, strlen(copy)+1);
                                                                
                                                                    waitpid(worker, NULL, WUNTRACED);

                                                                    changeAvailability(newhead,worker);
                                                                    close(fifo_fd); 
                                                                    
                                                                                         
                                                    }  
                                              
                           } 
                            
                            printf("Waiting for processes to end\n");

                            curr = newhead;
                            while (curr != NULL) {                                
                                    unlink(curr->str);
                                    wait(NULL);
                                    curr = curr->next;
                            }

                            printf("Freeing memory\n");
                            deleteQueue(&newhead);
                            free(buffer);
                            free(copy);
                            free(token);
                            free(path);
                            free(c);
                            free(r);
                                          
            }             
   return 0;
}