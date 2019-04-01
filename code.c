#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>


void check_sort(int array[], int length) 
{ 
	if (length==1) 
	{ 
		printf("array is sorted\n"); 
		return; 
	} 

	int i; 
	for (i=1; i<length; i++) 
	{ 
		if (array[i]<array[i-1]) 
		{ 
			printf("array is not sorted\n"); 
			return; 
		} 
	} 
	printf("Sorting Done Successfully\n"); 
	return; 
} 



void makedata(int a[], int len) 
{ 
  
    int i; 
    for (i=0; i<len; i++) 
        a[i] = rand(); 
    return; 
} 





void insertionsort(int arr[], int n); 
void merge(int a[], int d, int h1, int h2); 

  
void mergesort(int a[], int l, int le) 
{ 
    int i, len=(le-l+1); 
  
 
    if (len<=5) 
    { 
        insertionsort(a+l, len); 
        return; 
    } 
  
    pid_t l_p,r_p; 
    l_p = fork(); 
    if (l_p<0) 
    { 
     
        perror("not created_left child\n"); 
        _exit(-1); 
    } 
    else if (l_p==0) 
    { 
        mergesort(a,l,l+len/2-1); 
        _exit(0); 
    } 
    else
    { 
        r_p = fork(); 
        if (r_p<0) 
        { 
            
            perror("not created_right child\n"); 
            _exit(-1); 
        } 
        else if(r_p==0) 
        { 
            mergesort(a,l+len/2,le); 
            _exit(0); 
        } 
    } 
  
    int status; 
  
    waitpid(l_p, &status, 0); 
    waitpid(r_p, &status, 0); 
    merge(a, l, l+len/2-1, le); 
} 




void insertionsort(int arr[], int n) 
{ 
   int i, g, j; 
   for (i = 1; i < n; i++) 
   { 
       g= arr[i]; 
       j = i-1; 
  
       while (j >= 0 && arr[j] > g) 
       { 
           arr[j+1] = arr[j]; 
           j = j-1; 
       } 
       arr[j+1] = g; 
   } 
} 








void merge(int a[], int d, int h1, int h2) 
{ 
   
    int c=h2-d+1; 
    int s[c]; 
    int i=d, k=h1+1, m=0; 
    while (i<=h1 && k<=h2) 
    { 
        if (a[i]<a[k]) 
            s[m++]=a[i++]; 
        else if (a[k]<a[i]) 
            s[m++]=a[k++]; 
        else if (a[i]==a[k]) 
        { 
            s[m++]=a[i++]; 
            s[m++]=a[k++]; 
        } 
    } 
  
    while (i<=h1) 
        s[m++]=a[i++]; 
  
    while (k<=h2) 
        s[m++]=a[k++]; 
  
    int arr_count = d; 
    for (i=0; i<c; i++,d++) 
        a[d] = s[i]; 
} 
  







int main() 
{ 
	int shmid; 
	key_t key = IPC_PRIVATE; 
	int *array; 
 
	int length = 10000; 

	size_t SIZE = sizeof(int)*length; 

	if ((mid = shmget(key, SIZE, IPC_CREAT | 0666)) < 0) 
	{ 
		perror("shmget"); 
		_exit(1); 
	} 
 
	if ((array = shmat(mid, NULL, 0)) == (int *) -1) 
	{ 
		perror("shmat"); 
		_exit(1); 
	} 

	srand(time(NULL)); 
	makedata(array, length); 

	mergesort(array, 0, length-1); 
 
	check_sort(array, length); 

	
	if (shmdt(array) == -1) 
	{ 
		perror("shmdt"); 
		_exit(1); 
	} 

	if (shmctl(mid, IPC_RMID, NULL) == -1) 
	{ 
		perror("shmctl"); 
		_exit(1); 
	} 

	return 0; 
} 
