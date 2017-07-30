###  第二周学习总结
#### 一、重要知识点概括
```c
int execl(const char *path, const char *arg, ... /* (char  *) NULL */);
//代表把每个参数罗列
//execl("/usr/bin/ls", "-l", "-R", "/", (char *)NULL);
int execlp(const char *file, const char *arg, ... );
int execle(const char *path, const char *arg, ... /*, (char *)NULL, char * const envp[] */);
// char* const envp[] = {"AA=aaa", "XX=abcd", NULL};
    execle("./hello", "hello", "-l", "-a", NULL, envp);
    int execv(const char *path, char *const argv[]);
    //char *argv[] = { "-l", "-R", "/"  }；execv("/usr/bin/ls", argv);
    int execvp(const char *file, char *const argv[]);
    int execvpe(const char *file, char *const argv[], char *const envp[]);
    int fexecve(int fd, char *const argv[], char *const envp[]);
    ```
    ![这里写图片描述](http://img.blog.csdn.net/20170730135620919?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbTBfMzc3ODcyMjI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
    我觉得最重要的就是这个改变函数，没有这个改变函数，我就觉得程序就五大继续运行

    二、本周心得
    经历了上周ls的摧残之后，这周又有了进程，虽然这周代码实现比ls较为轻松，
    但是概念性的东西还是太多了，关键我觉得对计算机的操作系统概念不是特别清晰，所以到时看这一章比较吃力，
    进程只是了解了一个大概，但是我相信到时学操作系统应该就更轻松一些了。
