 #include<stdio.h>
#include<math.h>
#include<pthread.h>
int x[100], size, N;
int min, max;
int grno;
float grsize;
int histo[50];
int start=0;
pthread_mutex_t mutex;
pthread_cond_t cond;
void * running()
{
        int i, temp;
           pthread_mutex_lock(&mutex);
                if(start==0)
                        pthread_cond_wait(&cond, &mutex);    
        pthread_mutex_unlock(&mutex);

        for(;;)
        {
                pthread_mutex_lock(&mutex);
                i=size-1;
                size--;
                pthread_mutex_unlock(&mutex);

                if(size<0)
                {
                        pthread_mutex_lock(&mutex);
                          pthread_cond_wait(&cond, &mutex);
                        pthread_mutex_unlock(&mutex);
                }

                if(i<0)
                        break;

                if(min>x[i])
                        min=x[i];
                if(max<x[i])
                        max=x[i];                   
        }

        for(;;)
        {
                pthread_mutex_lock(&mutex);
                i=size-1;
                size--;
                pthread_mutex_unlock(&mutex);

                if(i<0)
                        break;

                temp=(float)((x[i]-min))/grsize;
                if(temp >(grno-1))
                        temp= grno-1;
                histo[temp]++;
        }
}

int main()
{
        pthread_t id;
        void * status;            
        int i, nthread;
        int temp1, temp2;

        pthread_mutex_init(&mutex,0);
        pthread_cond_init(&cond,0);

        printf("Enter the size of the array :: ");
        scanf("%d",&size);
        N=size;

        printf("\nEnter the %d elements of array\n",N);
        for(i=0;i<N;i++)
                scanf("%d",&x[i]);
        min=max=x[0];

        printf("\nEnter the total number of groups required in histogram::");
        scanf("%d",&grno);

        for(i=0;i<grno;i++)
                histo[i]=0;

        printf("\nEnter the total number of threads :: ");
        scanf("%d",&nthread);                 
        for(i=0;i<nthread;i++)
        {
                if(0==pthread_create(&id,0,running,0))
                        continue;
                else
                        printf("\nError in creating threads");
        }

        pthread_mutex_lock(&mutex);
        start=1;
        if(0!=pthread_cond_broadcast(&cond))
                printf("\nError in broadcasting");
        pthread_mutex_unlock(&mutex);

        while(size>=0)
                sleep(1);

        grsize=(float)(max-min)/(float)grno;
        size=N;

        pthread_mutex_lock(&mutex);
                pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
                                           
                                           
         while(size>=0)
                sleep(1);

        pthread_join(id,status);

        printf("\nThe histogram is as follows::\n");
        temp1=min;
        temp2=ceil(grsize);

        for(i=0;i<grno;i++)
        {
                if(i!=grno-1)
                {
                        printf("The number of elements in the bin of range %d to %d = %d\n",temp1, temp2,histo[i]);
                        temp1=temp2+1;
                        temp2+=ceil(grsize);
                }
                else
                        printf("For the last bin starting from %d  = %d ",temp1, histo[i]);
        }

        pthread_mutex_destroy(&mutex);                    

        printf("\n");
        return 0;
}                    



   
