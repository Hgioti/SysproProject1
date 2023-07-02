#ifndef myLib_h
#define myLib_h


void SearchSites(char *,char *);
struct QueueN {

    char str[BUFSIZ];
    int count;
    int av;
    int fd;
    struct QueueN* next;
};

void pushString(struct QueueN**, char *);

int searchQueue(struct QueueN*,char *);

void pushPid(struct QueueN**, int,char *,int);

void changeAvailability(struct QueueN*, int);

void deleteQueue(struct QueueN**);


#endif

