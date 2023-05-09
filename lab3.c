#include <stdio.h>
#include <stdlib.h>
#include<semaphore.h>
#include<threads.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#define size 5
#define N1 5
int count=0;
int buffer[size];
sem_t m,s,n,e;
int checkbuffer=0 ;
int readbuffer;
int front = -1, rear = -1;

void criticalsectionofmonitor(int value) {
  if (rear == size - 1)
   printf("\n monitor thread : Buffer full!!\n");
  else {
    if (front == -1)
      front = 0;
    rear++;
    buffer[rear] = value;
    printf("\n monitor thread : writing to buffer at position %d \n ",rear);
  }
}

void criticalsectionofcollector() {
  if (front == -1)
   printf("\n collector thread : nothing is in the buffer!\n");
  else {
  readbuffer=buffer[front];
    printf("\n collector thread : reading from the buffer at position %d\n",front);
    front++;
    if (front > rear)
      front = rear = -1;
  }
}



void *mcounter(void *n)
{
int k = *((int*)n);
while(1)
{

//int randtime = rand()%11+1;
int randtime = rand()%10;
sleep(randtime);
printf("\n counter thread %d : received a message\n",k);
printf("\n counter thread %d : waiting to write\n",k);
sem_wait(&m);
count++;
printf("\n counter thread %d : now adding to counter , counter value = %d \n",k,count);
sem_post(&m);


}

}


void *mmonitor(void *arg )
{
while(1)
{int index=0;
//int randtime = rand()%11+1;
int randtime = rand()%10;
sleep(randtime);
sem_wait(&m);
printf("\n monitor thread : waiting to read counter \n");
int value = count;
//count =0 ;
printf("\n monitor thread : reading a count value of %d \n",value);
sem_post(&m);
count=0;
//sem_getvalue(&e,checkbuffer);
//if(checkbuffer==0)


sem_wait(&e);
sem_wait(&s);

criticalsectionofmonitor(value);

sem_post(&s);
sem_post(&n);

}
//int randtime = rand()%7+1;
//sleep(randtime);
//count=0;
}
void *mcollector(void *arg)
{//int index=0;
while(1)
{
//int randtime = rand()%11+1;
int randtime = rand()%10;
sleep(randtime);
//int rvalue;
sem_wait(&n);
sem_wait(&s);

criticalsectionofcollector();

sem_post(&s);
sem_post(&e);


}
//int randtime = rand()%7+1;
//sleep(randtime);
}


int main()
{

//int N1=5;
pthread_t mcounterthread[N1],mmonitorthread,mcollectorthread;
sem_init(&s,0,1);
sem_init(&n,0,0);
sem_init(&e,0,size);
sem_init(&m,0,1);
for(int i=0;i<N1;i++)
{int *data =malloc(sizeof(*data));
*data=i;

pthread_create(&mcounterthread[i],NULL,mcounter,data);

}

pthread_create(&mmonitorthread,NULL,mmonitor,NULL);
pthread_create(&mcollectorthread,NULL,mcollector,NULL);

for(int i=0;i<N1;i++)
{

pthread_join(mcounterthread[i],NULL);
}
pthread_join(mmonitorthread,NULL);
pthread_join(mcollectorthread,NULL);
}
