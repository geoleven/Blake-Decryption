#include "hashOperations.hpp"
#include <cstring>				//strncpy
#include <cstdio>				//sprintf

void changeFormat(char* read, unsigned char* unread, int mode) {
	//1 for readable to un, 0 inverse
	int i = 0;
	if (mode)
		for (; i < 32; i++)
			sscanf(read + (i << 1), "%02x", &(unread[i]));
	else
		for (; i < 32; i++)
			sprintf(read+(i << 1 ) , "%02x", unread[i]);
}

int addHash(std::unordered_map<std::string, char*>* myHash, unsigned char* passPlusHash) {
	//unsigned char* nonReadableHash = (unsigned char*)malloc(sizeof(unsigned char) * 33);
	//nonReadableHash[32] = '\0';
	//memcpy(nonReadableHash, passPlusHash+7, 32);


	char* password = (char*)malloc(sizeof(char)*(PASSLEN + 1));
	password[PASSLEN] = '\0';
	memcpy(password, passPlusHash, PASSLEN);

	
	char* readableHash = (char*)malloc(sizeof(char) * 65);
	changeFormat(readableHash, passPlusHash+7, 0);
	readableHash[64] = '\0';
	std::string theKey(readableHash);
	free(readableHash);
	

	if (myHash->count(theKey) != 0) {
		free(password);
		return 0;
	} else {
		(*myHash)[theKey] = password;
		return 1;
	}
}

void destroyHash(std::unordered_map<std::string, char*>* myHash) {
	for (auto iterator = myHash->begin(); iterator != myHash->end(); ++iterator) {
		free(iterator->second);
	}
}

char* getPassFromHash(std::unordered_map<std::string, char*>* myHash, unsigned char* leakedHashNonReadable) {
	//char readableHash[65];
	char* readableHash = (char*)malloc(sizeof(char) * 65);
	readableHash[64] = '\0';
	changeFormat(readableHash, leakedHashNonReadable, 0);
	std::string theKey(readableHash);
	free(readableHash);

	if (myHash->count(theKey) != 0)
		return (*myHash)[theKey];
	else
		return NULL;
}


//std::pair<std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, char*>>>>, std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, char*>>>>> getPassFromHash(std::unordered_multimap<std::string, char*>* myHash, unsigned char* leakedHashNonReadable) {
//
//	char readableHash[65];
//	readableHash[64] = '\0';
//	changeFormat(readableHash, leakedHashNonReadable, 0);
//	std::string theKey(readableHash);
//
//	return myHash->equal_range(theKey);
//}


int loadFromFile(std::unordered_map <std::string, char*>* myHash, char* fileName, unsigned long int* entries, unsigned long int* collisions) {
	FILE* fd = NULL;
	long long int fileSize = 0;
	fd = fopen(fileName,"rb");
    if(NULL == fd) {
    	printf("Fopen() error!!!\n");
    	return 0;
    }
    fseek (fd , 0 , SEEK_END);
    fileSize = ftell(fd);
	unsigned char buffer[CHAININFOSIZE];
    int charSize = sizeof(char);
	long long int remainingChains = fileSize / CHAININFOSIZE;
    rewind(fd);
    while (remainingChains) {
		if (CHAININFOSIZE != fread(buffer, charSize, CHAININFOSIZE, fd)) {
    		printf("Something weird happening while reading from file.\n");
    		fclose(fd);
    		return 0;
		}
		else {
			if (addHash(myHash, buffer)) {
				(*entries)++;
			} else {
				(*collisions)++;
			}
    	}
    	remainingChains--;
		
    }
    printf("Done loading from file.\n%lu entries are now in the hash map.\n%lu collisions happened.\n", (*entries), (*collisions));
    fclose(fd);
    return 1;
}





