#include "CreateRBT.hpp"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "blake_ref.hpp"
#include <climits>				//CHAR_BIT
#include "RedFun.hpp"
#include <sys/types.h>
#include <pthread.h>
#include "llist.hpp"
//#include <ctime>		//Unix sleep and time variables
#include <Windows.h>	//Windows Sleep


#include "hashOperations.hpp"

extern const char* chractersToUse;

#define charTableLength 64
#define THREADSNUM 4

int* nextTable;
int chainLength;
int passLength;
int passBitLength;
char* currPass;
queue* hashQueue;
int localCollisions;
std::unordered_map <std::string, char*>* chash;


typedef struct {
	pthread_mutex_t* mtx;
	int chains;
	unsigned long long int* wtbw;
} threadArguments;



void checkSizes(int* table, int base, int max) {
	if (table[base] == charTableLength) {
		table[base] = 0;
		table[base+1]++;
		if (base+1 < max)
			checkSizes(table, base+1, max);
	}
}


void nextString(char* currentString) {
	int counter;
	for (counter = 0; counter < passLength; counter++) {
		currentString[counter] = chractersToUse[nextTable[counter]];
	}
	nextTable[0]++;
	checkSizes(nextTable, 0, passLength);
}





static void* chainCreatorthread(void* argsptr) {
	threadArguments* args = (threadArguments*)argsptr;
	pthread_mutex_t* mutex = args->mtx;
	int passPlusOne = passLength+1;
	int myChains = args->chains;
	char* myPass = (char*)malloc(sizeof(char)*passPlusOne);
	char* chainFirstPass =  (char*)malloc(sizeof(char)*passPlusOne);
	int currLocalChain;
	unsigned char* out = (unsigned char*)malloc(sizeof(char)*32);
	unsigned char* combined;


	for(currLocalChain = 0; currLocalChain < myChains; currLocalChain++) {
		pthread_mutex_lock(mutex);
		nextString(currPass);
		strncpy(myPass, currPass, passPlusOne);
		pthread_mutex_unlock(mutex);
		strncpy(chainFirstPass, myPass, passPlusOne);


		combined = (unsigned char*)malloc(sizeof(unsigned char)*(passLength+34));
		Hash(256, (BitSequence*)myPass, passBitLength, (BitSequence*)out);
		int counter;
		for(counter = 0; counter < chainLength; counter++) {
			reduct(out, myPass);
			Hash(256, (BitSequence*)myPass, passBitLength, (BitSequence*)out);
		}

		for (counter = 0; counter < passLength; counter++)
			combined[counter] = chainFirstPass[counter];
		combined[counter] = ' ';
		counter++;


		for (counter = passLength+1; counter < passLength+33; counter++)
			combined[counter] = out[counter-passPlusOne];
		combined[counter] = '\n';


		pthread_mutex_lock(mutex);
		if (addHash(chash, combined)) {
			args->wtbw++;
			queueAddLast(hashQueue, (void*)combined);
		}
		else {
			localCollisions++;
			currLocalChain--;
		}
		pthread_mutex_unlock(mutex);
	}

	free(out);
	free(myPass);
	free(chainFirstPass);
	return (void*) NULL;
}


/*Main creation function.*/
int createRBT(int passLen, int chainLen, int chains, char* fileName) {
	/*Declaration and initialization phase.*/
	localCollisions = 0;
	std::unordered_map <std::string, char*> hashInstance;
	chash = &hashInstance;
	clock_t begin, end;
	double time_spent;
	int writenChains = 0;
	int charsToBewWriten = 34+passLen;
	unsigned char* currentChainOutput;
	FILE* fd = NULL;
	fd = fopen(fileName,"w+b");
//	struct timespec sleepTime, rem;
    if(NULL == fd) {
    	printf("Fopen() error!!!\n");
    	return 1;
    }
	int counter;
	passLength = passLen;
	chainLength = chainLen;
	passBitLength = passLen*CHAR_BIT;
	nextTable = (int*)malloc(sizeof(int)*passLen);
	for(counter = 0; counter < passLen; counter++)
		nextTable[counter] = 0;
	currPass = (char*)malloc(sizeof(char)*(passLen+1));
	currPass[passLen] = '\0';
	int i;
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	pthread_attr_t attr;
	if (pthread_attr_init(&attr) != 0)
		printf("Thread attribute initialization not completed\n");
	pthread_t threadId[THREADSNUM];
	threadArguments arguments[THREADSNUM];
	unsigned long long int watingToBeWriten = 0;
	int estimatedSize = (34+passLen)*chains;
	int alreadyFlushed = 0;
	int percentage = 0;
	queue queueInst;
	hashQueue = &queueInst;
	queueInit(hashQueue);
//	sleepTime.tv_sec = 0;
//	sleepTime.tv_nsec = 5000000;


	/*Initialize arguments for the threads to be spawned.*/
	for (i = 0; i < THREADSNUM; i++) {
		//threadId[i] = 0;
		arguments[i].mtx = &mutex;
		arguments[i].chains = chains/THREADSNUM;
		if (i+1 == THREADSNUM)
			arguments[i].chains += (chains%THREADSNUM);
		arguments[i].wtbw = &watingToBeWriten;
	}


	/*Start time lapsing.*/
	begin = clock();


	/*Thread creation process.*/
	for (i = 0; i < THREADSNUM; i++) {
		if (pthread_create(&(threadId[i]), &attr, &chainCreatorthread, (void*) (&arguments[i]))) {
			printf("Error creating thread!");
		}
	}


	/*Parent thread task, writing chains to file after they have been computed.*/
	while(writenChains < chains) {
		pthread_mutex_lock(&mutex);
		if (!queueIsEmpty(hashQueue)) {
			watingToBeWriten--;
			currentChainOutput = (unsigned char*)queueGetAndRemoveFirst(hashQueue);
			pthread_mutex_unlock(&mutex);
			 if (charsToBewWriten != fwrite(currentChainOutput, sizeof(char), charsToBewWriten, fd))
				 perror("");
			//int c;
			//for (c = 0; c < 6; c++)
			//	printf("%c", currentChainOutput[c]);
			//printf(" ");
			//c++;
			//for (; c<39; c++)
			//	printf("%02x", currentChainOutput[c]);
			//printf("\n");
			writenChains++;
			alreadyFlushed = alreadyFlushed + passLen + 34;
			free(currentChainOutput);
		} else {
			pthread_mutex_unlock(&mutex);
			//nanosleep(&sleepTime, &rem);
			Sleep(50);
			if (((alreadyFlushed*100)/estimatedSize) > (percentage + 1)) {
				percentage = (alreadyFlushed*100)/estimatedSize;
				printf("%02d%% done.\n", percentage);
			}
		}
	}


	/*Stop time lapsing and print results.*/
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Time elapsed: %f\n", time_spent);


	/*Wait to collect the dead threads before exiting.*/
	double trash = 0;
	for (i = 0; i < THREADSNUM; i++) {
		pthread_join(threadId[i], (void**) (&trash));
	}


	/*Clean and exit.*/
	fclose(fd);
	queueDestroy(hashQueue);
	return 0;
}


