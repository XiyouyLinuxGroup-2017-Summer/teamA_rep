  # malloc族函数实现

  * 1 介绍
  * 2 内存管理
    * 2.1 内存地址
      * 2.1.1 虚拟内存地址与物理内存地址
      * 2.1.2 内存布局
      * 2.1.3 堆内存模型
    * 2.2 brk()与sbrk()
  * 3 malloc实现
    * 3.1 原理介绍
      * 3.1.1 工作模式
      * 3.1.2 first_fit & best_fito
    * 3.2 数据结构
    * 3.3 实现
      * 3.3.1 内存对齐
      * 3.3.2 遍历块
      * 3.3.3 扩展堆
      * 3.3.4 分离块
      * 3.3.5 malloc()
  * 4 其他函数
    * 4.1 calloc()
    * 4.2 free()
    * 4.3 realloc()
  * 5 总结
    * 5.1 代码
    * 5.2 碎片问题
  ------------------  

  ## 1.介绍


  >What is malloc ? If you don’t even know the name, you might begin to learn C in the Unix
  environment prior to read this tutorial. For a programmer, malloc is the function to allocate
  memory blocks in a C program, most people don’t know what is really behind, some even
  thinks its a syscall or language keyword. In fact malloc is nothing more than a simple function
  and can be understood with a little C skills and almost no system knowledge.
  The purpose of this tutorial is to code a simple malloc function in order to understand the
  underlying concepts. We will not code an efficient malloc, just a basic one, but the concept
  behind can be useful to understand how memory is managed in every day processes and how-to
  deal with blocks allocation, reallocation and freeing.
  From a pedagogical standpoint, this is a good C practice. It is also a good document to
  understand where your pointers come from and how things are organized in the heap.

  ![这里写图片描述](http://img.blog.csdn.net/20170718120242227?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  malloc函数在堆上分配size字节大小的内存，返回新内存起始处的地址，⚠️所分配的内存未经初始化，若无法分配内存，malloc返回null。由于malloc返回内存块采用内存对齐的方式，在大多数硬件架构上，malloc是基于8或16字节边界来分配内存。

  ## 2.内存管理

  要实现malloc首先要了解以下几个知识点

    ### 2.1 内存地址
    #### 2.1.1 虚拟内存地址与物理内存地址

    用户编制程序时使用的地址称为虚地址或逻辑地址，其对应的存储空间称为虚存空间或逻辑地址空间；而计算机物理内存的访问地址则称为实地址或物理地址，其对应的存储空间称为物理存储空间或主存空间。

    对于虚拟内存来说，每个进程看似都有各自独立的2^N(N为机器位数)字节的虚拟地址空间,真实中的进程不太可能（也用不到）如此大的内存空间，实际能用到的内存取决于物理内存大小。
    用户程序中使用的都是虚拟地址空间中的地址，永远无法直接访问实际物理地址。
    这种虚拟地址空间的作用主要是简化程序的编写及方便操作系统对进程间内存的隔离管理，虚拟内存到物理内存的映射由操作系统动态维护。
    虚拟内存一方面保护了操作系统的安全，另一方面允许应用程序使用比实际物理内存更大的地址空间。



    #### 2.1.2 内存布局

    我们以Linux 64位系统为例。理论上，64bit内存地址可用空间为0x0000000000000000 ~ 0xFFFFFFFFFFFFFFFF，
    根据Linux内核相关文档描述，Linux64位操作系统仅使用低47位，高17位做扩展（只能是全0或全1）。所以，实际用到的地址为空间为0x0000000000000000 ~ 0x00007FFFFFFFFFFF和0xFFFF800000000000 ~ 0xFFFFFFFFFFFFFFFF，其中前面为用户空间（User Space），后者为内核空间（Kernel Space）。图示如下

    ![](http://blog-codinglabs-org.qiniudn.com/image/a-malloc-tutorial-04.png)

    >Each process has its own virtual adress space dynamically translated into physical memory
    adress space by the MMU (and the kernel.) This space is devided in sevral part, all that we
    have to know is that we found at least some space for the code, a stack where local and volatile
    data are stored, some space for constant and global variables and an unorganized space for
    program’s data called the heap.

    每个进程都有它独立的虚拟内存空间，虚拟内存地址由MMU（Memory Management Unit动态的转化为物理内存地址对真实数据进行操作。这个空间被划分以下几部分

    * code - 存放函数体的二进制代码 。
    2. data - 常量字符串就是放在这里的，全局变量和静态变量的存储是放在一块的。初始化的全局变量和静态变量在一块区域，程序结束后由系统释放(rodata—read only data)。
    3. bss - 未初始化的全局变量和未初始化的静态变量(.bss)，程序结束后由系统释放。
    4. heap - 一般由程序员分配释放(new/malloc/calloc delete/free)，若程序员不释放，程序结束时可能由 OS 回收。
    ⚠️它与数据结构中的堆是两回事，但分配方式倒类似于链表。
    * Mapping Area - 这里是与mmap系统调用相关的区域。大多数实际的malloc实现会考虑通过mmap分配较大块的内存区域。
    5. stack - 由编译器自动分配释放，存放函数的参数值，局部变量的值等。其操作方式类似于数据结构中的栈。

  我们主要关注heap内存布局。

  #### 2.1.3 堆内存模型

  进程所面对的虚拟内存地址空间，只有按页映射到物理内存地址，才能真正使用。受物理存储容量限制，整个堆虚拟内存空间不可能全部映射到实际的物理内存
  >The heap is a continuous (in terme of virtual adresses) space of memory with three bounds:
  a starting point, a maximum limit (managed through sys/ressource.h’s functions getrlimit(2)
  and setrlimit(2)) and an end point called the break. The break marks the end of the mapped
  memory space, that is, the part of the virtual adress space that has correspondance into real
  memory.

  >堆是由三段连续内存组成，分别为开始部分，最大限制，堆顶。堆顶指针指向映射区域的边界，从堆起始地址到break之间的地址空间为映射好的，可以供进程访问；而从break往上，是未映射的地址空间，如果访问这段空间则程序会报错。

  Linux对堆的管理示意如下：


  ![](http://blog-codinglabs-org.qiniudn.com/image/a-malloc-tutorial-05.png)

  一般来说，malloc所申请的内存主要从Heap区域分配。改变堆的大小其实就像命令内核改变进程的break位置一样，最初，break正好位于未初始化数据段末尾之后。在break的位置抬升后，程序可以访问新分配区域内的任何内存地址，而此时物理内存页尚未分配。内核会在进程首次试图访问这些虚拟内存地址时自动分配新的物理内存页。

  ### 2.2 brk()与sbrk()

  Linux通过brk和sbrk系统调用操作break指针。

  ![这里写图片描述](http://img.blog.csdn.net/20170619183805524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU2VxdWluX1lG/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

  系统调用brk()会将program break设置为参数addr所指定的位置。由于内存以页为单位进行分配，addr实际为下一个页的边界处。

  **当试图将program break设置为一个低于其初初始值的位置时，也就是低于&end位置时，可能会导致无法预知的问题。比如说我们常见的分段内存访问错误（segmention fault）**

  调用sbrk()将program break在原有地址上增加incer参数的大小，调用成功返回 **前** 一个program break的地址，返回的是新增大小的起始地址。（可以想到，你可以调用sbrk(0)去跟踪一个堆的情况）

  >由于Linux是按页进行内存映射的，所以如果break被设置为没有按页大小对齐，则系统实际上会在最后映射一个完整的页，从而实际已映射的内存空间比break指向的地方要大一些。但是使用break之后的地址是很危险的（尽管也许break之后确实有一小块可用内存地址）。

  ## 3 malloc实现
    ### 3.1 原理介绍
    #### 3.1.1 工作模式
    malloc的实现很简单，它首先会扫描之前由free()所释放的空闲内存块列表，以求找到尺寸大于或等于要求的一块空闲内存。如果这一内存块的尺寸正好与要求相当，就将它返回给调用者，如果是一块较大的内存，那么将对其进行分割，在将一块大小相当的内存返回给调用者的同时，把较小的那块空闲内存块保留在空闲列表中。

    #### 3.1.2 first_fit & best_fito
    block链中查找合适的blocks时，一般来说有两种查找算法：
    * First fit：从头开始，使用第一个数据区大小大于要求size的块所谓此次分配的块
    * Best fit：从头开始，遍历所有块，使用数据区大小大于size且差值最小的块作为此次分配的块

  两种方法各有千秋，best fit具有较高的内存使用率（payload较高），而first fit具有更好的运行效率。

    ### 3.2 数据结构
    >what we need is a small block at the begining of each chunk containing the extra-information,
    called meta-data. This block contains at least a pointer to the next chunk, a flag to mark free chunks and the size of the data of the chunk. Of course, this block of information is before the pointer returned by malloc.

    >我们需要将堆内存空间以块（Block）的形式组织起来，每个块由meta区和数据区组成，meta区记录数据块的元信息（数据区大小、空闲标志位、指针等等），数据区是真实分配的内存区域，并且数据区的第一个字节地址即为malloc返回的地址。

    ![](http://blog-codinglabs-org.qiniudn.com/image/a-malloc-tutorial-06.png)

    ```
    typedef struct s_block *t_block;

    struct s_block {
      size_t size; //数据区域大小
      t_block next; //指向下一区域的指针
      int free; //是否为空闲区域（因为内存对齐所以用int）
      char data[1]  // 这是一个虚拟字段，表示数据块的第一个字节，长度不应计入meta
      int padding;  // ⚠️填充4字节，保证meta块长度为8的倍数 */
    };
    ```

    C禁止零长度数组，所以我们定义了一个字节长数组，这就是为什么之后我们需要为结构块的大小指定一个宏。

    ### 3.3 实现
    #### 3.3.1 内存对齐

    通常需要指针与整数大小（也就是指针大小）对齐。我们只考虑64位情况，因此，我们的指针必须是8的倍数（当然是64位 = 8字节），因为我们的元数据块已经对齐，我们唯一需要的是对齐数据块的大小。我们如何做到这一点？有几种方法，最有效的方法是使用算术技巧添加预处理器宏。

    首先我们知道，给定任何正整数除以八，然后再乘以八，得到的是最接近此数且小于次数的八的倍数，因此，只要再加八就能得到最接近次数且大于此数的八的倍数。但是这样有一个问题，当本身就为八的倍数时会造成答案偏大。

    那么假设，设X是一个整数

        X = 8 × p + q 0 ≤ q ≤ 7
    如果X是八的倍数:

      q = 0
      X - 1 = 8 × (P − 1）+ 7
    所以

      ((X − 1) / 8）× 8 + 8 = 8 × p = X
    如果X不是八的倍数：

      q != 0
      X − 1 = 8 × p + (q − 1), 0 ≤ q − 1 ≤ 6  

    所以

      (X − 1) / 8 × 8 + 8 = 8 × p + 8 = X / 8 × 8 + 8

    因此，我们可以得到公式：

      (X − 1)/8 × 8 + 8

    转化为c语言
    ```
    (((((X)-1)>>3)<<3)+8)
    ```

  #### 3.3.2 遍历块
  >We begin at the base address of the heap test the current chunk, if it fit our need we
  just return its address, otherwise we continue to the next chunk until we find a fitting one or the
  end of the head. The only trick is to keep the last visited chunk, so the malloc function can
  easily extends the end of the heap if we found no fitting chunk. The code is straightforward,
  base is a global pointer to the starting point of our heap

  find_block从frist_block开始，查找第一个符合要求的block并返回block起始地址，如果找不到这返回NULL。这里在遍历时会更新一个叫last的指针，这个指针始终指向当前遍历的block。这是为了如果找不到合适的block而开辟新block使用的。

  ```
  t_block find_block(t_block *last, size_t size) {
      t_block b = first_block;
      while(b && !(b->free && b->size >= size)) {
          *last = b;
          b = b->next;
      }
      return b;
  }
  ```
  #### 3.3.3 扩展堆

  当找不到合适的block时，就需要开辟新的空间，只需要移动break初始化一个新块，同时保存新块的信息。

  先定义一个持有块大小的宏，由于存在虚拟的data字段，sizeof不能正确计算meta长度，这里手工设置
  ```
  #define BLOCK_SIZE 24 //sizeof(struct s_block)
  ```

  ```
  t_block extend_heap(t_block last, size_t s) {
      t_block b;
      b = sbrk(0);
      if(sbrk(BLOCK_SIZE + s) == (void *)-1)
          return NULL;
      b->size = s;
      b->next = NULL;
      if(last)
          last->next = b;
      b->free = 0;
      return b;
  }
  ```
  #### 3.3.4 分离块

  First fit有一个比较致命的缺点，就是可能会让很小的size占据很大的一块block,当一个块足够宽以容纳被请求的大小加上一个新的块（至少是块大小+ 4）时，为了提高payload，我们在列表中插入一个新的块,将其分裂为一个新的block，示意如下：

  ![](http://blog-codinglabs-org.qiniudn.com/image/a-malloc-tutorial-07.png)

  ```
  void split_block(t_block b, size_t s) {
      t_block new;
      new = b->data + s; //由于字段数据是类型char[]，所以总和是按字节精度完成的。
      new->size = b->size - s - BLOCK_SIZE ;
      new->next = b->next;
      new->free = 1;
      b->size = s;
      b->next = new;
  }
  ```
  #### 3.3.5 malloc()

  malloc代码流程：
  * 首先对齐请求的大小
  * 如果初始化了base：
   - 搜索足够大的空闲块
   - 如果我们发现了一大块：
      * 尝试分割块
        （请求的大小和分配大小之间的差块要足以存储元数据和最小块（8字节)）
      * 作为使用的块（b->free =  0）
   - 否则：扩展堆。
    （ ⚠️查找块是将指针指向最后访问的块，这样我们可以在扩展期间不遍历整个列表直接访问。）
  * 否则：扩展堆（此时指针为空）。


  ```
  size_t align8(size_t s) {
      return (((s-1)>>3)<<3)+8
  }
  ```

  ```
  #define BLOCK_SIZE 24
  void *first_block=NULL;

  /* other functions... */

  void *malloc(size_t size) {
      t_block b, last;
      size_t s;
      /* 对齐地址 */
      s = align8(size);
      if(first_block) {
          /* 查找合适的block */
          last = first_block;
          b = find_block(&last, s);
          if(b) {
              /* 如果可以，则分裂 */
              if ((b->size - s) >= ( BLOCK_SIZE + 8))
                  split_block(b, s);
              b->free = 0;
          } else {
              /* 没有合适的block，开辟一个新的 */
              b = extend_heap(last, s);
              if(!b)
                  return NULL;
          }
      } else {
          b = extend_heap(NULL, s);
          if(!b)
              return NULL;
          first_block = b;
      }
      return b->data;
  }
  ```
  ## 4 其他函数
  ### 4.1 calloc()
  * 使用malloc得到一块内存
  * 将块内容赋0

  因为块的大小是8的倍数，所以可以8个字节一赋，我们将新指针强制看作无符号整数数组。
  ```
  void *calloc(size_t number, size_t size) {
      size_t *new;
      size_t s8, i;
      new = malloc(number * size);
      if(new) {
          s8 = align8(number * size) >> 3;
          for(i = 0; i < s8; i++)
              new[i] = 0;
      }
      return new;
  }
  ```
  ### 4.2 free()

  ### 4.3 realloc()
  ## 5 总结
  ### 5.1 代码
  ### 5.2 碎片问题

  参考：[Malloc_tutorial.pdf](http://www.inf.udec.cl/~leo/Malloc_tutorial.pdf) 、[实现一个malloc](http://blog.codinglabs.org/articles/a-malloc-tutorial.html)（推荐）  
  内存模型参考
  ：[Anatomy of a Program in Memory](http://duartes.org/gustavo/blog/post/anatomy-of-a-program-in-memory/)、
  [How the Kernel Manages Your Memory](http://duartes.org/gustavo/blog/post/how-the-kernel-manages-your-memory/)

  源码：[glibc](http://repo.or.cz/w/glibc.git/blob/HEAD:/malloc/malloc.c)
