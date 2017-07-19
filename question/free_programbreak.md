>在探究一下free()对progeram break的影响时，根据资料显示，free函数通常情况下不影响program break的大小，但是当在堆顶部释放完整的一组连续内存块时，会影响堆顶的大小。而我尝试实现后的结果却发现堆顶大小不变，和预期有出入。

下面有段程序，在分配了多块内存后，根据命令行参数来释放其中的部分或者全部。程序前两个命令行参数指定分配内存块的数量，大小，第三个参数表示释放内存的循环步长，有两个状态，为1释放每块内存，为2隔一块释放一块内存，第4、5个参数设定需要释放内存块的范围。步长默认设定1。


```
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_ALLOCS 1000000
#define GN_GT_0 2
#define GN_ANY_BASE 0100
#define GN_BASE_8 0200
#define GN_BASE_16 0400

static long getNum(const char *fname, const char *arg, int flags, const char* name){
    long res;
    char *endptr;
    int base;
    if (arg == NULL || *arg == '\0'){
        printf("error getnum\n");
        return 0;
    }

    base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? :(flags & GN_BASE_16) ? 16 : 10;
    res = strtol(arg, &endptr, base);
    return res;
}

int getInt(const char *arg, int flags, const char *name){
    long res;
    res = getNum("getInt", arg, flags, name);
    if(res > INT_MAX || res < INT_MIN){
        printf("error getnum\n");
        return 0;
    }

    return (int)res;
}

//分配内存块的数量，大小，释放内存的循环步长，需要释放内存块的范围

int main(int argc, char *argv[]){
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;
    printf("\n");

    numAllocs = getInt(argv[1], GN_GT_0, "num-allocs");
    blockSize = getInt(argv[2], GN_GT_0 | GN_ANY_BASE, "block-size");

    freeStep = (argc > 3) ? getInt(argv[3], GN_GT_0, "step") : 1;
    freeMin = (argc > 4) ? getInt(argv[4], GN_GT_0, "min") : 1;
    freeMax = (argc > 5) ? getInt(argv[5], GN_GT_0, "mac") : numAllocs;

    printf("Initial program break:    %10p\n", sbrk(0));

    printf("Allocating %d*%d bytes\n", numAllocs, blockSize);
    for(j = 0; j < numAllocs; j++){
        ptr[j] = malloc(blockSize);
        if(ptr[j] == NULL){
            exit(1);
        }
    }

    printf("Program break is now:    %10d\n", sbrk(0));
    printf("Freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);
    for(j = freeMin - 1; j < freeMax; j += freeStep){
        free(ptr[j]);
    }

    printf("After free(), program break is:    %10d\n", sbrk(0));

    return 0;
}

```

![这里写图片描述](http://img.blog.csdn.net/20170717113413264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

首先我分配1000个内存块，然后每隔一块内存释放一块内存。在释放所有内存块后，堆顶仍然为malloc后的大小。


![这里写图片描述](http://img.blog.csdn.net/20170717113716616?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后尝试释放部分内存块的情况，只留最后一块内存不释放，发现堆顶不变。

>这里有出现了一个问题，根据资料来看，如果在堆顶释放完整的一组连续内存块的话，free会调用sbrk使堆顶降下来，而我运行现实并没有任何变化。
这种情况下，free函数的glibc实现会在释放内存时将相邻的空间内存合并为一整块更大的内存，因而也有能力辨识出堆顶的整个空闲区域。所以实际情况下堆顶应该是降低的。

![这里写图片描述](http://img.blog.csdn.net/20170717120539733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
