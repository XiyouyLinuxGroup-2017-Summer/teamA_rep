#include<stdio.h> 
#define swap(x, y) \
	(y) = (x) + (y); \
	(x) = (y) - (x); \
	(y) = (y) - (x);


//测试数组与输出函数
int A[10]={5,4,6,1,3,2,9,7,8,0};
int temp[10]={0};

void print(int A[],int n){
	for(int i=0 ; i<n ; i++){
		printf((i==n-1)?"%d\n":"%d  ",A[i]);
	}
}









//冒泡的优化 1
void bubble1_sort (int A[], int n){ 
	
  	for(int i=0,flag=1 ; i<n-1&&flag ; ++i) {  
        flag=0;
    	for(int j=0 ; j<n-1-i ; ++j) {  
        	if(A[j] > A[j+1]) {  
            	flag = 1;  
            	swap(A[j],A[j+1])
        	}	  
    	}  
    } 
}






//冒泡的优化 2
void bubble2_sort(int A[], int n){ 

    int index=0,len=n-1;//index用来记录此躺最后比较的下标，len内层循环次数 
	for(int i=0 ; i<n; ++i) {  
        index=0; 
		for(int j=0 ; j<len ; ++j) {  
            if(A[j] > A[j+1]) {  
                index = j;  
               	swap(A[j],A[j+1])
            }  
        } 
		len=index; 
    }
}









//改进的冒泡排序--------鸡尾酒排序
void cocktail_sort(int A[],int n){
    
	int left=0,right=n-1;
    while (left < right){
        for (int i=left ; i<right ; ++i){  //找出最大元素，放在最后 
            if(A[i] > A[i+1]){
                swap(A[i],A[i+1]);
            }
        } 
        right--;
        for(int i=right ; i>left ; --i){  //找出最小元素，放在最后 
            if (A[i - 1] > A[i]){
                swap(A[i-1],A[i]);
            }
    	}
		left++;
    }
} 









//选择排序优化1 
void select_sort(int A[],int n){
   for(int left=0,right=n-1 ; left<right ; ++left,--right){
        int min = left;
        int max = right;
        for (int i=left; i<=right ; i++){
            if(A[i] < A[min]){
                min = i;
            }else if(A[i] > A[max]){
                max = i;
			}
        }
		if(max != right){
			swap(A[max], A[right]);
		}
	
        if(min == right){//最小值刚好在最右边，特殊处理 
            min = max;
        }
        
	    if(min != left){
    	    swap(A[min], A[left]);
		}
    }
}











//调整堆   A-->待调整的堆数组    i-->待调整的节点  n-->数组长度 
void adjust_heap(int A[],int i,int n) {
    int lchild=2*i+1;       //i的左孩子节点序号 
    int rchild=2*(i+1);     //i的右孩子节点序号 
    int max=i;  

    if(i <= n/2-1){//只调整非叶节点 
        //找到最大的左----右孩子节点与父节点交换 
		if(lchild<n && A[lchild]>A[max]){
            max=lchild;
        }    
        if(rchild<n && A[rchild]>A[max]){
            max=rchild;
        }
        
        if(max!=i){
            swap(A[i],A[max]);     //交换后破环了子节点的堆结构 
            adjust_heap(A,max,n);  //递归调用恢复子节点 
        }
    }        
}
//建立堆   堆是从下往上建立的  调整堆是建立在子树已经为大顶堆基础上。
void build_heap(int A[],int n){
	
    for(int i=n/2-1 ; i>=0 ; i--){    //非叶节点最大序号值为n/2-1
        adjust_heap(A,i,n);    
    }    
} 
//改进的选择排序--------堆排序 
void heap_sort(int A[],int n){
    
	build_heap(A,n);
    for(int i=n-1 ; i>=0 ; i--){
        swap(A[0],A[i]);//交换堆顶和最后一个元素，即每次将剩余元素中的最大者放到最后面 
        adjust_heap(A,0,i);      //重新调整堆顶节点成为大顶堆
    }
} 








//二分查找函数，返回插入下标  A--->待排序数组 start,end--->查找区间  k--->待插入的元素 
int BinarySearch(int A[], int start, int end,int k){
    while(start <= end){
        int middle = (start + end)/2;
        if(A[middle] > k){
            end = middle - 1;
        }else{
            start = middle + 1;
		}
    }
    return start;
}
//插入排序优化 :二叉查找插入排序
void insert_sort(int A[], int n){
  
    for(int i=1; i<n; i++){
        if(A[i] < A[i-1]){     //已经有序就跳过 
            int temp = A[i];
            int insertIndex = BinarySearch(A,0,i, A[i]);
            for (int j=i-1 ; j>=insertIndex ; j--){ //移动有序区间中插入下标---结尾的元素
                A[j+1] = A[j];   
            }       
            A[insertIndex] = temp;    //插入元素
        }
    }
}
 
 




//希尔按增量d的插入排序 
void shell_insert(int A[], int step, int len){  
    for(int i=step,j; i < len; ++i){  //从第一组的第二个元素开始 
	    int inser = A[i];//待插入的元素 
	    for(j=i-step ; j>=0&&inser<A[j] ; j-=step) {//对距离为 step 的元素组进行直接插入排序
	        A[j+step] = A[j];
	    }
	    A[j+step] = inser;
  	}  
} 
//改进的插入排序-------希尔排序
void shell_sort(int A[],int n) {  
    int step = n/2;   //初始增量 
    while(step >= 1){  
        shell_insert(A, step, n);  
        step/=2;    
    }  
}  








//partition  从数组中找出枢轴，然后返回它 
int partition(int A[],int left , int right){
    int pivot;
    pivot = A[left];
    while (left < right){
        while(left<right && A[right]>=pivot){
            right--;
		}
        A[left] = A[right];
        while(left<right && A[left]<=pivot){
            left++;
		}
        A[right] = A[left];
    }
    A[left] = pivot;
    return left;
}
//快排------重点在于寻找枢轴 
void quick_sort(int A[],int left,int right){  
   
    while(left<right){  
        int pivot=partition(A,left,right);  
//        quick_sort(A,left,pivot-1); //原递归调用  
//        quick_sort(A,pivot+1,right); 

        quick_sort(A, left,pivot - 1);  
        left = pivot+1; //尾递归 
   		
    }  
   
} 
 
 
 






//归并操作  A--->原数组     左序列left---mid右序列mid+1---right   temp---->临时数组存放合并的有序列   
void Merge(int A[],int left,int mid,int right,int temp[]){
    int i=left,j=mid+1,t=0;//左，右，临时数组指针
    
	while(i<=mid && j<=right){
        if(A[i] <= A[j]){
            temp[t++] = A[i++];
        }else{
            temp[t++] = A[j++];
        }
    }
    while(i <= mid){     //处理左序列剩余元素
        temp[t++] = A[i++];
    }
    while(j <= right){   //处理右序列剩余元素
        temp[t++] = A[j++];
    }
    t = 0;
    while(left <= right){   //合并后的有序序列放回原数组 
        A[left++] = temp[t++];
    }
}
//二路归并  
void merge_sort(int A[],int left,int right,int temp[]){
    if(left < right){
        merge_sort(A, left, (left+right)/2,temp);
        merge_sort(A, (left+right)/2+1, right,temp);
        Merge(A,left, (left+right)/2, right,temp);
    }
}
 
 
//计数排序 
void count_sort(int A[],int n){
	int c[n],ans[10];//c[i]中存放了值为i的元素的个数,com为排序好的数组
	
    for(int i=0 ; i<n; i++) {//初始化临时数组c  
        c[i] = 0;  
    }  
    for(int i=0 ; i<n ; i++) { 
        c[A[i]]++;  
    }  
    for(int i=0 ; i<n ; i++) {  //迭代c[i]得到  c[i]的值就是小于等于i=A[i]元素的个数 
        c[i]=c[i]+c[i-1];  
    }  
    for(int i=n-1 ; i>=0 ; i--) {// 给排序好的数组放入对应的元素 
        ans[c[A[i]]-1] = A[i];   // 将A[i]放入第C[i=A[i]]-1个位置上 
        c[A[i]]--;               //小于等于A[i]的元素个数减一，因为已经放好了一个 
    }    
    for(int i=0 ; i<n ; i++){
    	A[i]=ans[i];
	}
}







//桶排序
void bucket_sort(int A[],int n){
	int tem[n],ans[n];      //tem[i]中存放了值为i的元素的个数,ans为排序好的数组
   
    for(int i=0 ; i<n ; i++) {   //初始化临时数组tem 
        tem[i] = 0;  
    }  
   	for(int i=0 ; i<n ; i++) {   //统计数组中，每个元素出现的次数  
        tem[A[i]]++;
   	}  
   	for(int i=0,k=0 ; i<n ; i++) { 
   		if(tem[i]){
		   for(int j=0 ; j<tem[i] ; j++){
		   		ans[k++]=i;
		   }
		}
   	}
   	for(int i=0 ; i<n ; i++) {
        A[i] = ans[i];  
    }
}






int main(void){

	quick_sort(A,0,9);
	
	print(A,10);

	return 0;
}

