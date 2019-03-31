int main() 
{ 
	int shmid; 
	key_t key = IPC_PRIVATE; 
	int *shm_array; 
 
	int length = 10000; 

	size_t SHM_SIZE = sizeof(int)*length; 

	if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) 
	{ 
		perror("shmget"); 
		_exit(1); 
	} 
 
	if ((shm_array = shmat(shmid, NULL, 0)) == (int *) -1) 
	{ 
		perror("shmat"); 
		_exit(1); 
	} 

	srand(time(NULL)); 
	fillData(shm_array, length); 

	mergeSort(shm_array, 0, length-1); 
 
	isSorted(shm_array, length); 

	
	if (shmdt(shm_array) == -1) 
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
