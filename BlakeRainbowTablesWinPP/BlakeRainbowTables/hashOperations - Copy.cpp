#include "hashOperations.hpp"
#include <cstring>				//strncpy
#include <cstdio>				//sprintf
//#include <unordered_map>

void changeFormat(char* read, unsigned char* unread, int mode) {
	//1 for readable to un, 0 inverse
	int i = 0;
	if (mode) {
		for (; i < 32; i++) {
			sscanf(read + (i << 1), "%02x", &(unread[i]));
		}
	}
	else {
		for (; i < 32; i++) {
			sprintf(read+(i<<2) , "%02x", unread[i]);
		}
	}
}

int addHash(std::unordered_map<std::string, char*>* hash, unsigned char* passPlusHash) {
	//unsigned char* nonReadableHash = (unsigned char*)malloc(sizeof(unsigned char) * 33);
	//nonReadableHash[32] = '\0';
	//hashKey theKey;
	//theKey.nonReadableHash[32] = '\0';
	char* password = (char*)malloc(sizeof(char)*(PASSLEN + 1));
	password[PASSLEN] = '\0';

	memcpy(password, passPlusHash, PASSLEN);
	//memcpy(theKey.nonReadableHash, passPlusHash + PASSLEN + 1, 32);
	//std::string theKey((char*)(passPlusHash + 7));
	
	char* readableHash = (char*)malloc(sizeof(char) * 65);
	changeFormat(readableHash, passPlusHash+7, 0);
	readableHash[64] = '\0';
	std::string theKey(readableHash);
	

	if (hash->find(theKey) != hash->end()) {
		//free(nonReadableHash);
		free(password);
		return 0;
	} else {
		(*hash)[theKey] = password;
		return 1;
	}
}

void destroyHash(std::unordered_map<std::string, char*>* hash) {
	for (auto iterator = hash->begin(); iterator != hash->end(); ++iterator) {
		free(iterator->second);
	}
}

char* getPassFromHash(std::unordered_map<std::string, char*>* hash, unsigned char* leakedHashNonReadable) {
	//auto iterator = hash->find(*((hashKey*)(leakedHashNonReadable)));										//Hack magic

	char readableHash[65];
	readableHash[64] = '\0';
	changeFormat(readableHash, leakedHashNonReadable, 0);

	//if (hash->find(std::string((char*)(leakedHashNonReadable))) != hash->end())
	//	return (*hash)[(char*)(leakedHashNonReadable)];
	if (hash->find(std::string(readableHash)) != hash->end())
		return (*hash)[std::string(readableHash)];
	else
		return NULL;
}

int loadFromFile(std::unordered_map<std::string, char*>* hash, char* fileName, unsigned long int* entries, unsigned long int* collisions) {
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
    	} else {
			if (addHash(hash, buffer))
				(*entries)++;
			else
				(*collisions)++;
    	}
    	remainingChains--;
    }
    printf("Done loading from file.\n%lu entries are now in the hash map.\n%lu collisions happened.\n", *entries, *collisions);
    fclose(fd);
    return 1;
}





