
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
