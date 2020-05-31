#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<semaphore.h>

#define N 10

time_t end_time;
int count=N;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

void * barber(void *arg);
void * customer(void *arg);

int main(int argc,char *argv[])
{
    pthread_t p1,p2;
    end_time=time(NULL)+20;

    pthread_create(&p1, NULL, barber, NULL);
    pthread_create(&p2,NULL,customer,NULL);

    pthread_join(p2,NULL);
    pthread_join(p1,NULL);

    exit(0);
}

void * customer(void *arg)//producer
{
    while(time(NULL)<end_time)
    {
        if(count>0)
        {
            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&cv);
            pthread_mutex_unlock(&mutex);
        }
        else {
            sleep(1);
        }
    }
    return NULL;
}

void * barber(void *arg)//consumer
{
    while(time(NULL)<end_time)
    {
        if (count == 0){
            printf("Barber is sleeping\n");
            pthread_cond_wait(&cv, &mutex);
        }
        pthread_mutex_lock(&mutex);
        printf("Customer count is:%d.\n",count);
        count--;
        printf("Barber cut hair,count is:%d.\n",count);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    return NULL;
}

