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
