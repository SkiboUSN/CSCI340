#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <sys/queue.h>
//initialize queue
TAILQ_HEAD(tailhead, char) head;
//struct to support queue
struct entry {
char c;
TAILQ_ENTRY(entry) entries;
};

// struct __task{// this will be consumer struct i guess
// int taskid;
// } task_t;
// main acts as the producer in this case
int main(int argc, char *argv[]) {
TAILQ_INIT(&head); //initialize queue
if (strcmp(argv[1],"keyboard") || strcmp(argv[3],"k")) //check if keyboard or file
{
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
//how long the buffer currently is
while(!strcmp(read,"q"))
{
characters = fgets(&buffer,&bufsize,stdin);
printf("Input: '%s'\n",buffer);
TAILQ_INSERT_HEAD(&head, char, buffer);

}
}
else
{
FILE *fp;
int intArray[100];
int arraysize = 0;
//make sure in the readme to note filetype must be specified and cannot be k or keyboard
fp = fopen(argv[1],"r");
char[100] lineread;
int n = 100;
char *fgets(lineread, n, *fp);
TAILQ_INSERT_HEAD(&head, char, lineread);
}
for (int i = 0; i < *argv[2]; i++)
{
// task_t *tm = malloc(sizeof(task_t));// i think this is supposed to be the consumers
}

}
