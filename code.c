
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 


void insertion_sort(int array[], int n); 
void merge(int a[], int l1, int h1, int h2); 

void merge_sort(int a[], int l1, int l2) 
{ 
    int i, length=(l2-l1+1); 
  
    
    if (length<=5) 
    { 
        insertion_sort(a+l1, length); 
        return; 
    } 
  
    pid_t left_parent_id,right_parent_id; 
    left_parent_id = fork(); 
    if (left_parent_id<0) 
    { 
        
        perror("Left Child:error in create\n"); 
        _exit(-1); 
    } 
    else if (left_parent_id==0) 
    { 
        merge_sort(a,l1,l1+length/2-1); 
        _exit(0); 
    } 
    else
    { 
        right_parent_id = fork(); 
        if (right_parent_id<0) 
        { 
             
            perror("Right Child:error in create\n"); 
            _exit(-1); 
        } 
        else if(right_parent_id==0) 
        { 
            merge_sort(a,l1+length/2,l2); 
            _exit(0); 
        } 
    } 
  
    int status; 
  
    
    waitpid(left_parent_id, &status, 0); 
    waitpid(right_parent_id, &status, 0); 
  
    
    merge(a, l1, l1+length/2-1, l2); 
}



void merge(int ar[], int l1, int h1, int h2) 
{ 
	int c=h2-l1+1; 
	int sort[c]; 
	int i=l1, k=h1+1, m=0; 
	while (i<=h1 && k<=h2) 
	{ 
		if (ar[i]<ar[k]) 
			sort[m++]=a[i++]; 
		else if (ar[k]<ar[i]) 
			sort[m++]=ar[k++]; 
		else if (ar[i]==ar[k]) 
		{ 
			sort[m++]=ar[i++]; 
			sort[m++]=ar[k++]; 
		} 
	} 

	while (i<=h1) 
		sort[m++]=ar[i++]; 

	while (k<=h2) 
		sort[m++]=ar[k++]; 

	int ar_count = l1; 
	for (i=0; i<c; i++,l1++) 
		ar[l1] = sort[i]; 
}







void makedata(int array[], int length) 
{ 
	
	int i1; 
	for (i1=0; i1<length; i1++) 
		array[i1] = rand(); 
	return; 
} 






void check_sort(int array[], int length) 
{ 
	if (length==1) 
	{ 
		printf("Sorting Done \n"); 
		return; 
	} 
else{

	int j; 
	for (j=1; j<length; j++) 
	{ 
		if (array[j]<array[j-1]) 
		{ 
			printf("checking of sorting return false\n"); 
			return; 
		} 
	} 
	printf("Sorting of array is done \n"); 
}
	return; 
} 





void insertion_sort(int array[], int n) 
{ 
int i,j,k; 
for (i = 1; i < n; i++) 
{ 
	k = array[i]; 
	j = i-1; 

	
	while (j >= 0 && arr[j] > k) 
	{ 
		array[j+1] = array[j]; 
		j = j-1; 
	} 
	array[j+1] = k; 
} 
} 





int main() 
{ 
	int shmid; 
	key_t key = IPC_PRIVATE; 
	int *sharedmemo_array; 


	int length = 10000; 

	
	size_t segmentation_size = sizeof(int)*length; 

	if ((shmid = shmget(key, segmentation_size, IPC_CREAT | 0666)) < 0) 
	{ 
		perror("shmget"); 
		_exit(1); 
	} 

	
	if ((sharedmemo_array = shmat(shmid, NULL, 0)) == (int *) -1) 
	{ 
		perror("shmat"); 
		_exit(1); 
	} 

	
	srand(time(NULL)); 
	makedata(sharedmemo_array, length); 
 
	merge_sort(sharedmemo_array, 0, length-1); 

	 
	check_sort(sharedmemo_array, length); 

	
	if (shmdt(sharedmemo_array) == -1) 
	{ 
		perror("shmdt"); 
		_exit(1); 
	} 

	
	if (shmctl(shmid, IPC_RMID, NULL) == -1) 
	{ 
		perror("shmctl"); 
		_exit(1); 
	} 

	return 0; 
}
