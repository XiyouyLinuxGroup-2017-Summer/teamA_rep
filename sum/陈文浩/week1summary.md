##第一周总结
####一、linux c学习函数的总结
1、文件权限改变
```c
int chmod(const char *path,mode_t mode);
//直接通过文件名改变权限
int fchmode(int files,mode_t mode);
//通过文件描述符号去改变
```
|参数|八进制|含义|
|:-:|:-:|:-:|
|S_IRUSR（S_IREAD）| 0400|文件所有者具可读|取权限
|S_IWUSR（S_IWRITE）|0200|文件所有者具可写入权限| 
|S_IXUSR（S_IEXEC）| 00100| 文件所有者具可执行权限|
|                               |
|                               |
|S_IRGRP |00040 |用户组具可读取权限|
|S_IWGRP |00020 |用户组具可写入权限|
|S_IXGRP |00010 |用户组具可执行权限|
|        |       |                |
|                                 |
|S_IROTH |00004 |其他用户具可读取权限|
|S_IWOTH |00002 |其他用户具可写入权限|
|S_IXOTH |00001 |其他用户具可执行权限|

2、文件的创建、打开和关闭
①.open函数
```c
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathanme, int flags, mode_t mode);
```
②.creat函数
```c
#include<sys/types.h>
#include<sys/stat.h>
int creat(const char *pathname,mode_t mode);
```
③.close函数
```c
#include<stdio.h>
int close(int fd);
```

- 成功调用返回一个文件描述符号，否则返回-1


|mode参数|含义|
|:-|:-|
|O_RDONLY|只读打开|
 |O_WRONLY|只写打开|
 |O_RDWR|读写打开| |O_EXEC|只执行打开| 
 |O_SEARCH|(仅对目录有意义)只搜索打开|
|O_APPEND|每次写文件时都追加到文件的尾部|
|O_CLOEXEC	 |将FD_CLOEXEC设置为文件描述符标志|
|O_CREAT	|若该文件不存在则创建|
|O_EXCL	|(指定了O_CREAT)若文件已存在则报错|
|O_NOFOLLOW	|若文件是一个符号链接则报错|
|O_SYNC|	对文件的修改将阻塞直到物理磁盘上同步为止|
|O_DIRECTORY	|若文件不是一个目录则失败|
|O_NONBLOCK	|以非阻塞的方式打开文件|
|O_NDELAY|以非阻塞方式打开文件|
|O_TRUNC	|当文件存在并以可写方式打开则截断文件至0|
3、文件的读写
```c
①read函数
#include<unistd.h>
ssize_t read(int fd,void *buf,size_t count);
//简单的说就是从文件fd读取到buf
②write函数
#include<unistd.h>
ssize_t write (fd,void *buf,size_t count);
//给文件写进buf
```
- 返回值：若执行成功则返回0，失败则返回-1。
- fd：要读取的文件的文件描述符。
- buf：存放读取出的内容的缓冲区。
- count：要读取多少字节。
4、文件读取指针的移动
```c
#include<unistd.h>
#include<sys/typed.h>
off_t lseek(int fildes, off_t offset, int whence);
```
- 返回值：若执行成功则返回移动后距离文件首的字节数，失败则返回-1。
- fildes：要移动读写指针的文件描述符。
- offset：将指针移动到对于参数whence表示的位置的偏移量。
- whence：表示要将指针移动到的位置的参考位置。

|参数|含义|
|:-:|:-:|
|SEEK_SET|将新位置指定成从文件起始处的一个偏移距离|
|SEEK_CUR|将新位置指定成从当前位置开始的一个偏移距离|
|SEEK_END|将新位置指定成从文件结尾开始的的一个偏移距离|

5、fcnt函数
```c
#include<unistd.h>
#include<fcntl.h>
int fcntl(int fd,int cmd,long arg);
int fcntl(int fd,int cmd,struct flock *lock)
```
|cmd参数|含义|
|:-|:-|
|F_DUPFD|此时fcntl函数的功能与dup函数的功能相同，返回值也相同。|
|F_GETFD|获取fildes表示的文件的close_on_exec标志值。若执行成功则返回该标志值，失败则返回-1。|
|F_SETFD|此时需要额外提供arg参数，设置fildes表示的文件的close_on_exec标志值，设为arg参数的最后一位。若执行成功则返回0，失败则返回-1。|
|F_GETFL|获得fildes表示的文件的打开方式（即open函数的oflag参数）。若执行成功则返回0，失败则返回-1。|
|F_SETFL|设置fildes表示的文件的打开方式，这里只可以设置O_APPEND、O_NONBLOCK和O_ASYNC三种属性。|
|F_SETLK|以下三个功能需要额外提供lock参数。对fildes表示的文件加锁或释放锁。若执行成功则返回0，失败则返回-1。|
|F_SETLKW|与F_SETLK功能基本相同，区别是若设置了这个标志，在文件被使用时将等待文件被释放。|
|F_GETLK|试探fildes表示的文件是否可以设置为lock，但不会真的设置为该值。|

6、获取文件属性
```c
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
int stat(const char *restrict path, struct stat *restrict buf);
int fstat(int fildes, struct *buf);
int lstat(const char *restrict path, struct stat *restrict buf);
```
- 注意：这个获取正确的相对路径，不能只是文件名
###二、坑点！！！
1、数组一定要开的大
2、尽量不用数组，用链表可以不用专门去看大小，动态去分配空间
3、尽量不要用swich语句去考虑参数的问题，一旦参数变多，case语句就会加很多
**4、最坑的就是一定要懂得opendir是打开目录，只是获取了文件名，没有真正改变工作目录，用stat的时候也要把目录看清！！！**

###三、心得体会

这个礼拜我觉得是过得特别充实，感觉自己很久没有这么认真的坐下来认真学习。首先感觉到自己对linux知道的太少了，不是几个命令就能懂得linux。这个礼拜写ls，才真正体会了这个小小不起眼的命令，隐含了这么多神奇的功能，写起来也非常困难。让我也真正体会到了linux这个系统的魅力。
学习了这么久的c语言，真正让我感觉到它的强大是在这一周，之前课设任务量很大，但是课设的文件操作远不及ls这个命令更有魅力。这一周过得确实很辛苦，小组很多人真是很厉害，不得不服，让我确实压力很大，ls做到后面感觉自己很急躁，非常的慌，但是慢慢只要沉下心来，很多问题都迎刃而解了，所以我觉得这周是一个非常好的开始，然后得到了很多锻炼，之后几周，我一定会保持更好的状态去迎接每一天。