
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
 #include <time.h>

void insertionsort(int arr[], int n); 
void merge(int a[], int l1, int h1, int h2); 

void mergesort(int array[], int l, int h) 
{ 
	int i, leng=(h-l+1); 

	
	if (leng<=5) 
	{ 
		insertionsort(array+l, leng); 
		return; 
	} 

	pid_t left_pid,right_pid; 
	left_pid = fork(); 
	if (left_pid<0) 
	{ 
		
		perror("error in create left child\n"); 
		_exit(-1); 
	} 
	else if (left_pid==0) 
	{ 
		mergesort(array,l,l+leng/2-1); 
		_exit(0); 
	} 
	else
	{ 
		right_pid = fork(); 
		if (right_pid<0) 
		{ 
			
			perror("error in create right child\n"); 
			_exit(-1); 
		} 
		else if(right_pid==0) 
		{ 
			mergesort(array,l+leng/2,h); 
			_exit(0); 
		} 
	} 

	int status; 

	 
	waitpid(left_pid, &status, 0); 
	waitpid(right_pid, &status, 0); 

	 
	merge(array, l, l+leng/2-1, h); 
} 


void insertionsort(int array[], int n) 
{ 
int i, k, j; 
for (i = 1; i < n; i++) 
{ 
	k = array[i]; 
	j = i-1;

	
	while (j >= 0 && array[j] > k) 
	{ 
		array[j+1] = array[j]; 
		j = j-1; 
	} 
	array[j+1] = k; 
} 
} 


void merge(int arr[], int l1, int h1, int h2) 
{ 

	int c=h2-l1+1; 
	int sort[c]; 
	int i=l1, k=h1+1, m=0; 
	while (i<=h1 && k<=h2) 
	{ 
		if (arr[i]<arr[k]) 
			sort[m++]=arr[i++]; 
		else if (arr[k]<arr[i]) 
			sort[m++]=arr[k++]; 
		else if (arr[i]==arr[k]) 
		{ 
			sort[m++]=arr[i++]; 
			sort[m++]=arr[k++]; 
		} 
	} 

	while (i<=h1) 
		sort[m++]=arr[i++]; 

	while (k<=h2) 
		sort[m++]=arr[k++]; 

	int arr_c= l1; 
	for (i=0; i<c; i++,l1++) 
		arr[l1] = sort[i]; 
} 


void checksort(int arr[], int length) 
{ 
	if (length==1) 
	{ 
		printf("Sorting Done \n"); 
		return; 
	} 

	int i; 
	for (i=1; i<length; i++) 
	{ 
		if (arr[i]<arr[i-1]) 
		{ 
			printf("Sorting Not Done\n"); 
			return; 
		} 
	} 
	printf("Sorting Done\n"); 
	return; 
} 

 



void makedata(int array[], int length) 
{ 
	 
	int i; 
	for (i=0; i<length; i++) 
		array[i] = rand(); 
	return; 
} 





 
int main() 
{ 
 clock_t start, end;
     double cpu_time_used;
     
     start = clock();
	int sh_m_id; 
	key_t key = IPC_PRIVATE; 
	int *shared_memo_array; 


	 
	int length = 10000; 

	
	size_t shared_length = sizeof(int)*length; 

	
	if ((sh_m_id = shmget(key, shared_length, IPC_CREAT | 0666)) < 0) 
	{ 
		perror("shmget"); 
		_exit(1); 
	} 

	
	if ((shared_memo_array = shmat(sh_m_id, NULL, 0)) == (int *) -1) 
	{ 
		perror("shmat"); 
		_exit(1); 
	} 

	 
	srand(time(NULL)); 
	makedata(shared_memo_array, length); 
  
	
	mergesort(shared_memo_array, 0, length-1); 

	checksort(shared_memo_array, length); 


	if (shmdt(shared_memo_array) == -1) 
	{ 
		perror("shmdt"); 
		_exit(1); 
	} 

	if (shmctl(sh_m_id, IPC_RMID, NULL) == -1) 
	{ 
		perror("shmctl"); 
		_exit(1); 
	} 
  end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

     printf("cpu_time:\n");
     
        printf("%lf",cpu_time_used );
printf("\n");  


	return 0; 
} 

