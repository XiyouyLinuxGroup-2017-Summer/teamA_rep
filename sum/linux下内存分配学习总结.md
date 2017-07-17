内存分配
==========

在堆上分配内存
------
所谓堆是一段长度可变的虚拟内存，始于进程的未初始化数据段末尾（BSS）。通常将堆的当前内存边界称为“program break”。

我们依然结合进程内存布局看来。

![这里写图片描述](http://img.blog.csdn.net/20170717100735727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

>改变堆的大小其实就像命令内核改变进程的program break位置一样，最初，program break正好位于未初始化数据段末尾之后。在program break的位置抬升后，程序可以访问新分配区域内的任何内存地址，而此时物理内存页尚未分配。内核会在进程首次试图访问这些虚拟内存地址时自动分配新的物理内存页。

![这里写图片描述](http://img.blog.csdn.net/20170619183805524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

系统调用brk()会将program break设置为参数addr所指定的位置。由于内存以页为单位进行分配，addr实际为下一个页的边界处。

**当试图将program break设置为一个低于其初初始值的位置时，也就是低于&end位置时，可能会导致无法预知的问题。比如说我们常见的分段内存访问错误（segmention fault）**

调用sbrk()将program break在原有地址上增加incer参数的大小，调用成功返回 **前** 一个program break的地址，返回的是新增大小的起始地址。（可以想到，你可以调用sbrk(0)去跟踪一个堆的情况）


malloc函数族详解
------
对于malloc函数我们并不陌生，学习c语言时学过malloc的用法。

![这里写图片描述](http://img.blog.csdn.net/20170717101842302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

malloc函数在堆上分配size字节大小的内存，返回新内存起始处的地址，⚠️所分配的内存未经初始化，若无法分配内存，malloc返回null。由于malloc返回内存块采用内存对齐的方式，在大多数硬件架构上，malloc是基于8或16字节边界来分配内存。

函数calloc()用于给一组相同对象分配内存，参数指定分配对象的数量，size指定每个对象的大小，与malloc不同，calloc会将已分配的内存初始为0。

realloc()函数通常用来调整一块内存的大小。参数ptr用来指向调整的内存，size指定所需调整的大小，函数返回指向大小调整后内存块的指针。若realloc增加了已分配内存块的大小，它不会对额外分配的内存进行初始化。

>通常情况下，当增大已分配的内存时，realloc()会尝试合并在空闲列表中紧随其后且大小满足要求的内存块，若原内存位于堆的顶部，那么函数将对堆空间进行扩展。若这块内存位于堆中部并且紧邻其后的内存空间大小不足，realloc会新分配一块内存，并将原数据复制到新内存块中，总的来说就是realloc()函数能够移动内存，这会大量占用资源，因此，尽量避免使用realloc。

当然，你也可以用realloc()去重新定位一块内存
```
new = realloc(ptr, newsize);
if(new != NULL){        //最好进行错误判断，不然有可能会丢失现有内存块
  ptr = new;
}
```

free函数释放ptr所指向的内存块，**一般情况下**，free并不降低program break的位置，而是将这块内存添加到空闲内存列表中供后续使用。原因如下：
 1. 被释放的内存一般位于堆的中间而不是顶端。
 2. 这样会减少sbrk系统调用的使用次数
 3. 降低program break的收益不大，因为对于分配大量内存的程序来说，它们通常倾向于持有已分配内存或者是反复释放和重新分配内存，而不是释放所有内存后在持续运行。


------------------

在堆栈上分配内存
--------

alloca()通过增加栈帧的大小从堆栈上分配内存。根据定义，当前调用函数的栈帧位于堆栈的顶部，帧的上方存在扩展空间，只需修改堆栈指针值就行。参数size指定在堆栈上分配的字节数，函数返回指向已分配内存块的指针。

![这里写图片描述](http://img.blog.csdn.net/20170717173301143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

要⚠️ 的是，不能在函数的参数列表中调用alloca()，会使alloca分配的堆栈空间出现在当前函数参数的空间内，而函数参数是有固定的位置的。

>alloca()函数相对malloc来说具有一定的优势，编译器将alloca作为内联代码处理，并通过直接调整堆栈指针来实现，此外，alloca也不需要维护空闲内存块列表。另外，alloca()函数分配的内存随栈帧的移除而自动释放，也就是调用调用alloca的函数返回则释放。
