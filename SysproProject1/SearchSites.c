#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include "myLib.h"



void SearchSites(char *sample,char *path)
{
    int file,i,j,output_fd;
    
    char buffer[100000];
    char str[BUFSIZ];
    mode_t fd_mode=S_IRWXU;
    struct QueueN* head = NULL;

    char *s=(char *)malloc(100);

    strcpy(s,path);
    strcat(s,"/");
    strcat(s,sample);
   // printf("sample is %s and path is %s\n",sample,s);

    if ((file=open(s,O_RDONLY))==-1) {
                           perror("Error opening the file");
                           return;
    }

    read(file, buffer, 100000);
    printf("buffer is %s\n",buffer);
    //Psaxnw gramma gramma
    for(i=0;buffer[i]!='\0';i++){

        //Tsekarw gia http://
        if(buffer[i]=='h'){
            if(buffer[i+1]=='t'){
                i++;
                if(buffer[i+1]=='t'){
                    i++;
                    if(buffer[i+1]=='p'){
                         i++;
                         if(buffer[i+1]==':'){
                             i++;
                             if(buffer[i+1]=='/'){
                                  i++;
                                  if(buffer[i+1]=='/'){
                                         i=i+2;


                                         //Tsekarw gia www.
                                         if(buffer[i] == 'w'){
                                                    if(buffer[i+1] == 'w'){
                                                        if(buffer[i+2] == 'w'){
                                                             if(buffer[i+3] == '.'){
                                                                i=i+4;
                                                             }
                                                        }
                                                    }
                                         }


                                         j=0;
                                         /* Grafw to site se string */
                                         while((buffer[i]!=' ') && (buffer[i]!='\0') && (buffer[i]!='\n') && (buffer[i]!='\t'))
                                         {
                                                
                                                str[j]=buffer[i];
                                                j++;
                                                i++;
                                         }

                                         str[j]='\0';

                                         /* Bazw to string se oura, e3airwntas to (keno) site */
                                         if( (str[0] != ' ') && (str[0] != '\t') && (str[0] != '\0') && (str[0] != '\n') ){
                           

                                                if(searchQueue(head,str)==0){
                                                     pushString(&head,str);
                                               }
                                        }

                                         memset(str,0,strlen(str));
                                   }
                             }
                          }
                     }
                }
            }
        }
    }


    /* Open filename.out */
    strcat(sample,".out");
    char *output=(char *)malloc(100);

    strcpy(output,"output/");
    strcat(output,sample);

    if ((output_fd=open(output,O_WRONLY | O_CREAT,fd_mode))==-1) {
                        perror("Error opening the file");
    }


    /* Trexw thn oura, metatrepw ton ari8mo emfanishs se string, kai epeita kollaw ka8e site me ton ari8mo se ena string */

    char *buf=(char *)malloc(100);
    char *final=(char *)malloc(20000);

    while (head != NULL) {
   
        (void) sprintf(buf, " %d\n", head->count);

        strcat(head->str,buf);
        strcat(final,head->str);

        head = head->next;
    }

    printf("Extracting sites and writing them to %s\n",output);
    write(output_fd, final, strlen(final));
    close(file);

    deleteQueue(&head);
    free(buf);
    free(final);
    free(output);
    free(s);

}