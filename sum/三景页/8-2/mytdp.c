#include<stdi.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<pthread.h>
#include<assert.h>

typedef struct worker
{
    void* (*process)(void* arg);
    void* arg;
    struct worker* next;
}CThread_worker;

typedef struct
{
    pthread_mutex_t queue_lock;
	pthread_cond_t queue_ready;
	CThread_worker* queue_head;  //链
    int shutdown;                //是否销毁线程池
    pthread_t* threadid;
    int max_thread_num;          //线程池中允许的活动线程数目
    int cur_queue_size;          //当前等待队列的数目
}CThread_pool;

int pool_add_worker(void*(*process)(void* arg),void* arg);
void* thread_routine(void* arg);

static CThread_pool* pool=NULL;

void pool_init(int max_thread_num)
{
    pool=(CThread_pool*)malloc(sizeof(CThread_pool));
    pthread_mutex_init(&(pool->queue_lock),NULL);
    pthread_cond_init(&(pool->queue_ready),NULL);
    pool->queue_head=NULL;
    pool->max_thread_num=max_thread_num;
    pool->cur_queue_size=0;
    pool->shutdown=0;
    pool->threadid=(pthread_t*)malloc(max_thread_num*sizeof(pthread_t));
    int i=0;
    for(i=0;i<max_thread_num;i++)
    {
        pthread_create(&(pool->threadid[i],NULL,thread_routine,NULL));
    }
}




int main()
{
    poll_init(3);  //线程池初始化，设置容量为3
    sleep(1);

    int *workingnum=(int*)malloc(sizeof(int)*10);
    int i;
    for(i=0;i<10;i++)
    {
        workingnum[i]=i;
        pool_add_worker(myprocess,&workingnum[i]);
    }
    sleep(5);      //等待任务执行完
    pool_destory();//摧毁线程池
    free(workingnum);
    return 0;
}
