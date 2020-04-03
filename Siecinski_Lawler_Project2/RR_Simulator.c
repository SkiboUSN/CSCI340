#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

sem_t empty;
pthread_mutex_t popmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t writemutex = PTHREAD_MUTEX_INITIALIZER;

//structs to support queue
struct entry {
    char *line;
    struct entry *next;
    struct entry *prev;
};

struct queue {
    struct entry *queue[100];
    struct entry head;
    struct entry tail;
};

struct queue myqueue;

char* pop(){
    pthread_mutex_lock(&popmutex);
    struct entry tempEntry = *myqueue.tail.prev;
    char *tempstr = tempEntry.line;
    myqueue.tail.prev = tempEntry.prev;
    tempEntry = *tempEntry.prev;
    tempEntry.next = &myqueue.tail;
    pthread_mutex_unlock(&popmutex);
    return tempstr;
}

void *consumer(void *tid){

}

int main(int argc, char *argv[]) {

    /*pthread_t threads[(int)argv[2]];
    int rc;
    long t;
    for(t=0; t<argv[2]; t++){
       printf("In main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }
    }*/

    //initialize queue
    myqueue.head.next = &myqueue.tail;
    myqueue.tail.prev = &myqueue.head;

    if (strcmp(argv[1],"keyboard") == 0 || strcmp(argv[1],"k") == 0) //check if keyboard or file
    {
        char *buffer; //where the typed statement will be stored
        size_t bufsize = 64; //how long the string the user can input
        size_t characters;
        printf("not here\n");
        buffer = (char *)malloc(bufsize * sizeof(char));
        //check if malloc failed
            if( buffer == NULL)
            {
                perror("Unable to allocate buffer");
                exit(1);
            }
        //how long the buffer currently is
        struct entry tempEntry;
        characters = getline(&buffer, &bufsize, stdin);
        while(strcmp(buffer, "quit\n") != 0)
        {

            tempEntry.line = buffer;
            tempEntry.next = myqueue.head.next;
            tempEntry.prev = &myqueue.head;
            struct entry secondTempEntry = *myqueue.head.next;
            secondTempEntry.prev = &tempEntry;
            myqueue.head.next = &tempEntry;


            printf("Input: %s\n",buffer);
            characters = getline(&buffer, &bufsize, stdin);
            //sem_post(&empty);

        }
        free(buffer);
    }
    else
    {
        FILE *fp;
        printf("and here\n");
        //make sure in the readme to note filetype must be specified and cannot be k or keyboard
        fp = fopen(argv[1],"r");
        printf("and 1\n");
        if(fp == NULL){
            printf("not a valid txt file\n");
            exit(1);
        }
        char *buffer; //where the typed statement will be stored
        size_t bufsize = 64; //how long the string the user can input
        size_t characters;
        buffer = (char *)malloc(bufsize * sizeof(char));
        //check if malloc failed
            if( buffer == NULL)
            {
                perror("Unable to allocate buffer");
                exit(1);
            }
        printf("and 2\n");
        //how long the buffer currently is
        int i = 0;
        while((characters = getline(&buffer, &bufsize, fp)) != -1)
        {

            struct entry *tempEntry;
        printf("and 3\n");
            tempEntry = myqueue.queue[i];
        printf("and 4\n");
            tempEntry->line = (char *)malloc(bufsize*sizeof(char));
        printf("and 6\n");
            strcpy(tempEntry->line, buffer);
        printf("and 5\n");
            tempEntry->next = myqueue.head.next;
            tempEntry->prev = &myqueue.head;
            struct entry *secondTempEntry = myqueue.head.next;
            secondTempEntry->prev = tempEntry;
            myqueue.head.next = tempEntry;


            printf("Input: '%s'\n",buffer);
            i++;
            //sem_post(&empty);

        }
        struct entry tempEntry;
        for(int i=0;i<10;i++){
            tempEntry = *myqueue.head.next;
            printf("%d: %s",i,tempEntry.line);
        }
        free(buffer);
        //closedir(fp);

    }
}
