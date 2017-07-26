#include<stdio.h> 
#define swap(x, y) \
	(y) = (x) + (y); \
	(x) = (y) - (x); \
	(y) = (y) - (x);


//�����������������
int A[10]={5,4,6,1,3,2,9,7,8,0};
int temp[10]={0};

void print(int A[],int n){
	for(int i=0 ; i<n ; i++){
		printf((i==n-1)?"%d\n":"%d  ",A[i]);
	}
}









//ð�ݵ��Ż� 1
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






//ð�ݵ��Ż� 2
void bubble2_sort(int A[], int n){ 

    int index=0,len=n-1;//index������¼�������Ƚϵ��±꣬len�ڲ�ѭ������ 
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









//�Ľ���ð������--------��β������
void cocktail_sort(int A[],int n){
    
	int left=0,right=n-1;
    while (left < right){
        for (int i=left ; i<right ; ++i){  //�ҳ����Ԫ�أ�������� 
            if(A[i] > A[i+1]){
                swap(A[i],A[i+1]);
            }
        } 
        right--;
        for(int i=right ; i>left ; --i){  //�ҳ���СԪ�أ�������� 
            if (A[i - 1] > A[i]){
                swap(A[i-1],A[i]);
            }
    	}
		left++;
    }
} 









//ѡ�������Ż�1 
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
	
        if(min == right){//��Сֵ�պ������ұߣ����⴦�� 
            min = max;
        }
        
	    if(min != left){
    	    swap(A[min], A[left]);
		}
    }
}











//������   A-->�������Ķ�����    i-->�������Ľڵ�  n-->���鳤�� 
void adjust_heap(int A[],int i,int n) {
    int lchild=2*i+1;       //i�����ӽڵ���� 
    int rchild=2*(i+1);     //i���Һ��ӽڵ���� 
    int max=i;  

    if(i <= n/2-1){//ֻ������Ҷ�ڵ� 
        //�ҵ�������----�Һ��ӽڵ��븸�ڵ㽻�� 
		if(lchild<n && A[lchild]>A[max]){
            max=lchild;
        }    
        if(rchild<n && A[rchild]>A[max]){
            max=rchild;
        }
        
        if(max!=i){
            swap(A[i],A[max]);     //�������ƻ����ӽڵ�Ķѽṹ 
            adjust_heap(A,max,n);  //�ݹ���ûָ��ӽڵ� 
        }
    }        
}
//������   ���Ǵ������Ͻ�����  �������ǽ����������Ѿ�Ϊ�󶥶ѻ����ϡ�
void build_heap(int A[],int n){
	
    for(int i=n/2-1 ; i>=0 ; i--){    //��Ҷ�ڵ�������ֵΪn/2-1
        adjust_heap(A,i,n);    
    }    
} 
//�Ľ���ѡ������--------������ 
void heap_sort(int A[],int n){
    
	build_heap(A,n);
    for(int i=n-1 ; i>=0 ; i--){
        swap(A[0],A[i]);//�����Ѷ������һ��Ԫ�أ���ÿ�ν�ʣ��Ԫ���е�����߷ŵ������ 
        adjust_heap(A,0,i);      //���µ����Ѷ��ڵ��Ϊ�󶥶�
    }
} 








//���ֲ��Һ��������ز����±�  A--->���������� start,end--->��������  k--->�������Ԫ�� 
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
//���������Ż� :������Ҳ�������
void insert_sort(int A[], int n){
  
    for(int i=1; i<n; i++){
        if(A[i] < A[i-1]){     //�Ѿ���������� 
            int temp = A[i];
            int insertIndex = BinarySearch(A,0,i, A[i]);
            for (int j=i-1 ; j>=insertIndex ; j--){ //�ƶ����������в����±�---��β��Ԫ��
                A[j+1] = A[j];   
            }       
            A[insertIndex] = temp;    //����Ԫ��
        }
    }
}
 
 




//ϣ��������d�Ĳ������� 
void shell_insert(int A[], int step, int len){  
    for(int i=step,j; i < len; ++i){  //�ӵ�һ��ĵڶ���Ԫ�ؿ�ʼ 
	    int inser = A[i];//�������Ԫ�� 
	    for(j=i-step ; j>=0&&inser<A[j] ; j-=step) {//�Ծ���Ϊ step ��Ԫ�������ֱ�Ӳ�������
	        A[j+step] = A[j];
	    }
	    A[j+step] = inser;
  	}  
} 
//�Ľ��Ĳ�������-------ϣ������
void shell_sort(int A[],int n) {  
    int step = n/2;   //��ʼ���� 
    while(step >= 1){  
        shell_insert(A, step, n);  
        step/=2;    
    }  
}  








//partition  ���������ҳ����ᣬȻ�󷵻��� 
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
//����------�ص�����Ѱ������ 
void quick_sort(int A[],int left,int right){  
   
    while(left<right){  
        int pivot=partition(A,left,right);  
//        quick_sort(A,left,pivot-1); //ԭ�ݹ����  
//        quick_sort(A,pivot+1,right); 

        quick_sort(A, left,pivot - 1);  
        left = pivot+1; //β�ݹ� 
   		
    }  
   
} 
 
 
 






//�鲢����  A--->ԭ����     ������left---mid������mid+1---right   temp---->��ʱ�����źϲ���������   
void Merge(int A[],int left,int mid,int right,int temp[]){
    int i=left,j=mid+1,t=0;//���ң���ʱ����ָ��
    
	while(i<=mid && j<=right){
        if(A[i] <= A[j]){
            temp[t++] = A[i++];
        }else{
            temp[t++] = A[j++];
        }
    }
    while(i <= mid){     //����������ʣ��Ԫ��
        temp[t++] = A[i++];
    }
    while(j <= right){   //����������ʣ��Ԫ��
        temp[t++] = A[j++];
    }
    t = 0;
    while(left <= right){   //�ϲ�����������зŻ�ԭ���� 
        A[left++] = temp[t++];
    }
}
//��·�鲢  
void merge_sort(int A[],int left,int right,int temp[]){
    if(left < right){
        merge_sort(A, left, (left+right)/2,temp);
        merge_sort(A, (left+right)/2+1, right,temp);
        Merge(A,left, (left+right)/2, right,temp);
    }
}
 
 
//�������� 
void count_sort(int A[],int n){
	int c[n],ans[10];//c[i]�д����ֵΪi��Ԫ�صĸ���,comΪ����õ�����
	
    for(int i=0 ; i<n; i++) {//��ʼ����ʱ����c  
        c[i] = 0;  
    }  
    for(int i=0 ; i<n ; i++) { 
        c[A[i]]++;  
    }  
    for(int i=0 ; i<n ; i++) {  //����c[i]�õ�  c[i]��ֵ����С�ڵ���i=A[i]Ԫ�صĸ��� 
        c[i]=c[i]+c[i-1];  
    }  
    for(int i=n-1 ; i>=0 ; i--) {// ������õ���������Ӧ��Ԫ�� 
        ans[c[A[i]]-1] = A[i];   // ��A[i]�����C[i=A[i]]-1��λ���� 
        c[A[i]]--;               //С�ڵ���A[i]��Ԫ�ظ�����һ����Ϊ�Ѿ��ź���һ�� 
    }    
    for(int i=0 ; i<n ; i++){
    	A[i]=ans[i];
	}
}







//Ͱ����
void bucket_sort(int A[],int n){
	int tem[n],ans[n];      //tem[i]�д����ֵΪi��Ԫ�صĸ���,ansΪ����õ�����
   
    for(int i=0 ; i<n ; i++) {   //��ʼ����ʱ����tem 
        tem[i] = 0;  
    }  
   	for(int i=0 ; i<n ; i++) {   //ͳ�������У�ÿ��Ԫ�س��ֵĴ���  
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

