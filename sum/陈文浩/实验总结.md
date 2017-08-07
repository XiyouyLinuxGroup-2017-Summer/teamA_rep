看了线程一个礼拜，我觉得最能考验知识就是这次实验题，让我知道了学习中很多的不足
一、进程原题
```c

/*  POSIX 下进程控制的实验程序残缺版 */ 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <signal.h>
#include <ctype.h> 
/* 允许建立的子进程个数最大值 */
#define MAX_CHILD_NUMBER 10 
/* 子进程睡眠时间 */
#define SLEEP_INTERVAL 2 
int proc_number=0; 
/* 子进程的自编号，从0开始 */
void do_something(); 

int main(int argc, char* argv[])
{   
    /* 子进程个数 */
    int child_proc_number = MAX_CHILD_NUMBER; 
    int i, ch; 
    pid_t  child_pid; 
    pid_t pid[10]={0}; /* 存放每个子进程的id */ 
    if (argc > 1) /* 命令行参数第一个参数表示子进程个数*/ 
    {
                child_proc_number = atoi(argv[1]); 
                child_proc_number= (child_proc_number > 10) ? 10 :child_proc_number;
            
    } 
    for (i=0; i<child_proc_number; i++) { 
            /* 填写代码，建立child_proc_number个子进程要执行
            * proc_number = i; 
            * do_something(); 
            * 父进程把子进程的id保存到pid[i] */ 
        }
    /* 让用户选择杀死进程，数字表示杀死该进程，q退出 */
    while ((ch = getchar()) != 'q')  
    { 
            if (isdigit(ch)) 
            { 
                        /*  填写代码，向pid[ch-'0']发信号SIGTERM， 
                        * 杀死该子进程 */ 
                    }
        } 
    /* 在这里填写代码，杀死本组的所有进程 */ 
    return;
} 
void do_something() 
{ 
    for(;;) 
    {  
            printf("This is process No.%d and its pid is %d\n",proc_number,  getpid());
            sleep(SLEEP_INTERVAL); // 主动阻塞两秒钟
        }
}
kill()函数用于删除执行中的程序或者任务。调用格式为： kill(int PID, int IID)；
其中：PID是要被杀死的进程号，IID为向将被杀死的进程发送的中断号。

实验过程

先猜想一下这个程序的运行结果。假如运行“./process 20”，输出会是什么样？然后按照注释里的要求把代码补充完整，运行程序。开另一个终端窗口，运行“ps aux|grep process”命令，看看process 究竟启动了多少个进程。回到程序执行窗口，按“数字键+回车”尝试杀掉一两个进程，再到另一个窗口看进程状况。按q 退出程序再看进程情况。

回答下列问题

1、你最初认为运行结果会怎么样？
最开始我觉得应该是num按顺序进行

2、实际的结果什么样？有什么特点？试对产生该现象的原因进行分析。
结果第一次出现了全是0的情况，因为我把父进程进入无限循环，导致只有num0，只要讲子进程进入无线循环就不会有问题了

3、proc_number 这个全局变量在各个子进程里的值相同吗？为什么？
不相同，因为进入子进程，子进程有独立的堆栈，只是复制了一份全局变量

4、kill 命令在程序中使用了几次？每次的作用是什么？执行后的现象是什么？
如果大于等于a.out后面大于等于10，执行10次，每次都会杀死一个子进程，杀死后就不会继续有对应的子进程循环出现

5、使用kill 命令可以在进程的外部杀死进程。进程怎样能主动退出？这两种退出方式哪种更好一些？
进程只要执行结束所有代码，就可以主动退出，这里是杀了所有进程，才可以退出，如果异常退出，使用kill你就会发现很多子进程都变成僵尸进程，因为父进程还没有回收，就已经杀死了
```
```c
我的改进
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include<stdlib.h>
/* 允许建立的子进程个数最大值 */
#define MAX_CHILD_NUMBER 10
/* 子进程睡眠时间 */
#define SLEEP_INTERVAL 2
int proc_number=0;
/* 子进程的自编号，从0开始 */
void do_something();

int main(int argc, char* argv[])
{
        /* 子进程个数 */
        int child_proc_number = MAX_CHILD_NUMBER;
        int i, ch;
        pid_t  child_pid;
        pid_t pid[10]={0}; /* 存放每个子进程的id */
        if (argc > 1) /* 命令行参数第一个参数表示子进程个数*/
    {
                child_proc_number = atoi(argv[1]);
                child_proc_number= (child_proc_number > 10) ? 10 :child_proc_number;
            
    }
        pid_t t;
    for (i=0; i<child_proc_number; i++) {
                /* 填写代码，建立child_proc_number个子进程要执行
                * proc_number = i;
                * do_something();
                * 父进程把子进程的id保存到pid[i] */
                t=fork();
                if(t!=0)
                    pid[i]=t;
                if(t==0)//让子进程去执行循环
        {
                        proc_number=i;
                        do_something();
                    
        }
            
    }
        /* 让用户选择杀死进程，数字表示杀死该进程，q退出 */
        while ((ch = getchar()) != 'q')
    {
                if (isdigit(ch))
        {
                        /*  填写代码，向pid[ch-'0']发信号SIGTERM，
                        * 杀死该子进程 */

                        kill(pid[ch-'0'],SIGTERM);
                        pid[ch-'0']=0;//一定套清零
                    
        }
            
    }
                kill(0,SIGTERM);//0就是杀死所有进程
        /* 在这里填写代码，杀死本组的所有进程 */
        return 0;

}
void do_something()
{
        for(;;)
    {
                printf("This is process No.%d and its pid is %d\n",proc_number,  getpid());
                sleep(SLEEP_INTERVAL); // 主动阻塞两秒钟
            
    }

}
```
###二、线程实验（重点！！！）
```c
/*  POSIX 下线程控制的实验程序残缺版 */ 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <ctype.h>
#include <pthread.h> 

#define MAX_THREAD 3 /* 线程的个数 */
unsigned long long main_counter, counter[MAX_THREAD]; 
/* unsigned long  long是比long还长的整数 */
void* thread_worker(void*); 
int main(int argc,char* argv[])
{     
    int i, rtn, ch;     
    pthread_t pthread_id[MAX_THREAD] = {0}; /* 存放线程id*/ 
    for (i=0; i<MAX_THREAD; i++)
    {       
            /* 在这里填写代码，用pthread_create建一个普通的线程，
            线程id存入pthread_id[i]，线程执行函数是thread_worker
            并i作为参数传递给线程 */
        }    
    do
    {
                /* 用户按一次回车执行下面的循环体一次。按q退出 */          
                unsigned long long sum = 0;    
                /* 求所有线程的counter的和 */
                for (i=0; i<MAX_THREAD; i++) 
        {
                        /* 求所有counter的和 */         
                        sum += counter[i];             
                        printf("%llu ", counter[i]);        
                    
        }
                printf("%llu/%llu", main_counter, sum);   
            
    }while ((ch = getchar()) != 'q'); 
    return 0;
} 
void* thread_worker(void* p)
{   
    int thread_num;  
    /* 在这里填写代码，把main中的i的值传递给thread_num */ 
    for(;;) 
    { /* 无限循环 */      
         counter[thread_num]++; /* 本线程的counter加一 */ 
         main_counter++; /* 主counter 加一 */   
        } 
}
实验过程 按照注释里的要求把代码补充完整，正确编译程序后，先预计一下这个程序的运行结果。具体的结果会是什么样？运行程序。开另一个终端窗口，运行“ps aux”命令，看看thread 的运行情况，注意查看thread 的CPU 占用率，并记录下这个结果。

```
回答下列问题

1、你最初认为前三列数会相等吗？最后一列斜杠两边的数字是相等，还是大于或者小于关系？
刚开始对三者不是特别确定，因为线程比较随意，所以觉得应该不会相等，开始没有运行的时候觉得应该是相等的。

**2、最后的结果如你所料吗？有什么特点？对原因进行分析。**
**最后发现前三个数字，有两个都会是0，斜杠前面的数字还比后面的小**
**①因为线程是共用的内存地址，i传过去的是地址，所以还没等线程**  
**运行继续，i的值已经变了，所以实际上i值没有很好的传过去。**
**解决方法可以看下面的改进代码，有三种方法。**
**②因为线程运行的时候是取出全局变量，但是很多线程取一个全局变量，都会把自己改变的全局变量放回原来的地方，无形之中全局变量就少加了很多，所以为什么有写锁，就是为了防止这种情况的发生**

3、thread 的CPU 占用率是多少？为什么会这样？
cpu占有率能达到300%多，因为线程可以多个处理器跑，所有使用也就比较多
4、thread_worker()内是死循环，它是怎么退出的？你认为这样退出好吗？
进程结束之后，所以线程也就完美结束，这样不好，因为没有处理临界资源

```c
完整版
#include<stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define MAX_THREAD 3 /* 线程的个数 */
unsigned long long main_counter, counter[MAX_THREAD];
/* unsigned long  long是比long还长的整数 */
void* thread_worker(void*);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int main(int argc,char* argv[])
{
        int i, rtn, ch;
        int k=0;
        pthread_t pthread_id[MAX_THREAD] = {0}; /* 存放线程id*/
        int *arr;
        for (i=0; i<MAX_THREAD; i++)
    {
              
                ①arr=(int *)malloc(sizeof(int));
                *arr=i;
                pthread_create(&pthread_id[i],NULL,(void *)thread_worker,arr);
                ②定义一个数组，每次传数组，因为数组每个地址就不一样
                ③从i开刀
                 pthread_create(&pthread_id[i],NULL,(void *)thread_worker,（void *)i);//把i的数值强制转换
            
    }
        do
    {
                char t;
                unsigned long long sum = 0; 
                for (i=0; i<MAX_THREAD; i++)
        {
                        sum += counter[i];
                        printf("%llu ", counter[i]);
                    
        }
                //注意！！！，这里printf之前全局变量还会在加，所以实际上斜杠左右不一定完全相等
                printf("%llu/%llu", main_counter, sum);//printf的时候maincounter还在++
            
    }while ((ch = getchar()) != 'q');
        return 0;

}
void* thread_worker(void* p)
{
        int thread_num;
       thread_num=*(int *)p;
        
        for(;;)
        { 
            :call <SNR>119_SparkupNext()
          pthread_mutex_lock(&mutex1);
             counter[thread_num]++; 
             main_counter++; 
             pthread_mutex_unlock(&mutex1);
            }

}
```
###三、互斥
```c
//* POSIX 下线程死锁的演示程序 */ 
#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <ctype.h>
#include <pthread.h> 

#define LOOP_TIMES 10000 

/*用宏PTHREAD_MUTEX_INITIALIZER来初始化 */
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_worker(void*);
void critical_section(int thread_num, int i); 

int main(void) 
{     
    int rtn, i;     
    pthread_t pthread_id = 0; /* 存放子线程的id */ 
    rtn = pthread_create(&pthread_id,NULL, thread_worker, NULL ); 
    if(rtn != 0)
    {            
            printf("pthread_create ERROR!\n"); 
            return -1; 
        } 
    for (i=0; i<LOOP_TIMES; i++) 
    { 
            pthread_mutex_lock(&mutex1);
            pthread_mutex_lock(&mutex2); 
            critical_section(1, i); 
            pthread_mutex_unlock(&mutex2);
            pthread_mutex_unlock(&mutex1);
        } 

    pthread_mutex_destroy(&mutex1); 
    pthread_mutex_destroy(&mutex2); 
    return 0;
} 
void* thread_worker(void* p) 
{ 
    int i; 
    for (i=0; i<LOOP_TIMES; i++)
    { 
            pthread_mutex_lock(&mutex2);
            pthread_mutex_lock(&mutex1); 
            critical_section(2, i); 
            pthread_mutex_unlock(&mutex2);
            pthread_mutex_unlock(&mutex1);
        }
} 
void critical_section(int thread_num, int i) 
{ 
    printf("Thread%d: %d\n", thread_num,i);
}
仔细阅读程序，编译程序后，先预计一下这个程序的运行结果。运行程序。若程序没有响应，按ctrl+c 中断程序运行，然后再重新运行，如此反复若干次，记录下每次的运行结果。若产生了死锁，请修改程序，使其不会死锁。

回答下列问题
1、你预想deadlock.c 的运行结果会如何？
正常运行
2、deadlock.c 的实际运行结果如何？多次运行每次的现象都一样吗？为什么会这样？
会出现只运行线程一或者只运行线程二，或者交替运行之后卡死，不一样，因为上下线程讲两个变量都锁了，造成死锁
``` 
```c
正确版本
//* POSIX 下线程死锁的演示程序 */
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define LOOP_TIMES 10000

/*用宏PTHREAD_MUTEX_INITIALIZER来初始化 */
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* thread_worker(void*);
void critical_section(int thread_num, int i);

int main(void)
{
        int rtn, i;
        pthread_t pthread_id = 0; /* 存放子线程的id */
        rtn = pthread_create(&pthread_id,NULL, thread_worker, NULL );
        if(rtn != 0)
    {
                printf("pthread_create ERROR!\n");
                return -1;
            
    }
        for (i=0; i<LOOP_TIMES; i++)
    {
                pthread_mutex_lock(&mutex1);//锁一
                pthread_mutex_lock(&mutex2);
                critical_section(1, i);
                pthread_mutex_unlock(&mutex2);
                pthread_mutex_unlock(&mutex1);
            
    }

        pthread_mutex_destroy(&mutex1);
        pthread_mutex_destroy(&mutex2);
        return 0;

}
void* thread_worker(void* p)
{
        int i;
        for (i=0; i<LOOP_TIMES; i++)
    {
                pthread_mutex_lock(&mutex1);//如果这一步锁2,上面的锁要锁2,这时候2已经锁了，于是无线等待
                pthread_mutex_lock(&mutex2);
                critical_section(2, i);
                pthread_mutex_unlock(&mutex2);
                pthread_mutex_unlock(&mutex1);
            
    }

}
void critical_section(int thread_num, int i)
{
        printf("Thread%d: %d\n", thread_num,i);

} 
```
