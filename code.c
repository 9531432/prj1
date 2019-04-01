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
	fillData(array, length); 

	mergeSort(array, 0, length-1); 
 
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
