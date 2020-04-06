#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

int done = 0; //keeps track of the amount of lines in the queue
pthread_mutex_t popmutex = PTHREAD_MUTEX_INITIALIZER; //used for geting lines from the queue
pthread_mutex_t writemutex = PTHREAD_MUTEX_INITIALIZER; //used for updating the total word count
int total_words = 0; //keeps track of the total amount of words in the txt file

//structs to support queue
struct entry {
    char *line;
    struct entry *next;
    struct entry *prev;
};

//arbitrarily large queue with head and tail nodes
struct entry *queue[100000];
struct entry head;
struct entry tail;

//defines an entry's variables as well as correctly alter the queue links
void make_entry(struct entry *obj, const char *line, struct entry *next, struct entry *prev){
    obj->line = strdup(line);
    obj->next = next;
    obj->prev = prev;
    next->prev = obj;
    prev->next = obj;
}

//allocates the memory for an entry then call make_entry to define its variables
struct entry *malloc_entry(const char *line, struct entry *next, struct entry *prev){
    struct entry *obj = malloc(sizeof(*obj));
    make_entry(obj, line, next, prev);
    return obj;
}

//returns and removes the first most entered entry in the queue
char* pop(){
    pthread_mutex_lock(&popmutex);
    struct entry tempEntry = *tail.prev;
    char *tempstr = tempEntry.line;
    tail.prev = tempEntry.prev;
    struct entry secondtempEntry = *tempEntry.prev;
    secondtempEntry.next = &tail;
    done--;
    pthread_mutex_unlock(&popmutex);
    return tempstr;
}

//the method threads run in order to count the number of words in a line and add that to the total word count
void *consumer(void *tid){

    while(done != 0){

        char *str = pop();
        int wordcount = 0;
        int j;

        //iterates throught the string
        for(j=0;str[j]!='\0';j++){
            //checks if the current character IS NOT a space or tab and the next character IS a space, tab, or newline
            if((str[j] != ' ' && str[j] != '\t') && (str[j+1] == ' ' || str[j+1] == '\t' || str[j+1] == '\n')){
                wordcount++;
            }
        }
        printf("tid: %p line: %swords: %d\n", tid, str, wordcount);

        //locks the write mutex to ensure no accidental overwrites then updates the total word count
        pthread_mutex_lock(&writemutex);
        total_words = total_words + wordcount;
        pthread_mutex_unlock(&writemutex);
    }
}

int main(int argc, char *argv[]) {

    //checks if the correct amount of arguments is inputed and tells the user how to use the program if not
    if(argc != 3){
        printf("Usage: ./340_proj_3 <input> <# of threads>\n");
        exit(1);
    }


    //initialize queue
    head.next = &tail;
    tail.prev = &head;

    //checks if the user wishes to input via the keyboard
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
        //how long the buffer currently is and get the user input
        characters = getline(&buffer, &bufsize, stdin);
        int i = 0;
        //continue gathering user inputs until the user inputs "quit"
        while(strcmp(buffer, "quit\n") != 0)
        {

            queue[i] = malloc_entry(buffer, head.next, &head); //create a new entry and add it to the queue
            printf("Input: ");
            i++; //increment spot in queue array
            done++; //increment number of lines
            characters = getline(&buffer, &bufsize, stdin); //how long the buffer currently is and get the user input

        }
        free(buffer); //free up the space used by the buffer
    }

    //use a file for input if the user did not specify to use the keyboard
    else
    {
        //open the input file or tell the user if the file does not exist
        FILE *fp;
        fp = fopen(argv[1],"r");
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
        int i = 0;
        //how long the buffer currently is and get the lines of the file
        while((characters = getline(&buffer, &bufsize, fp)) != -1)
        {
            queue[i] = malloc_entry(buffer, head.next, &head); //create a new entry and add it to the queue
            i++; //increment spot in queue array
            done++; //increment number of lines
        }
        free(buffer); //free up the space used by the buffer
        closedir(fp); //close the stream from the input file

    }


    //create an amount of threads equal to the user's input running the consumer function
    char *end;
    long numThread = strtol(argv[2], &end, 10);
    pthread_t threads[numThread];
    int rc;
    long t;
    for(t=0; t<numThread; t++){
       printf("In main: creating thread %ld\n", t);
       rc = pthread_create(&threads[t], NULL, consumer, (void *)t);
       if (rc){
          printf("ERROR; return code from pthread_create() is %d\n", rc);
          exit(-1);
       }
    }


    //wait for all threads to finish
    int k;
    for(k=0;k<(sizeof(threads)/sizeof(threads[0]));k++){
        pthread_join(threads[k], NULL);
    }

    printf("There are %d words in this text.\n", total_words); //print out the final total word count
}
