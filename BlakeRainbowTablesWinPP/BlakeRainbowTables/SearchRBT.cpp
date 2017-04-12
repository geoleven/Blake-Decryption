#include "hashOperations.hpp"
#include "RedFun.hpp"
#include "blake_ref.hpp"
#include <cstdio>


extern const char* chractersToUse;

#define CHAINLEN 3000

char* reverseHash(void* vhash, char* humanRedableHash) {
	std::unordered_map <std::string, char*>* myHash = (std::unordered_map <std::string, char*>*)vhash;
	char* found;
	int i;
	unsigned long int tries = 0;
	unsigned char* nonReadableHash = (unsigned char*)malloc(sizeof(unsigned char)*33);
	nonReadableHash[32] = '\0';
	char* nextPassToHash = (char*)malloc(sizeof(char)*(PASSLEN+1));
	nextPassToHash[PASSLEN] = '\0';
	unsigned char givenHash[33];
	givenHash[32] = '\0';

	for (i = 0; i < 32; ++i) {
		sscanf(humanRedableHash + (i << 1), "%02x", &(nonReadableHash[i]));
	}
	memcpy(givenHash, nonReadableHash, 32);

	char* passForChainUnfold = (char*)malloc(sizeof(char)*(PASSLEN + 1));
	passForChainUnfold[PASSLEN] = '\0';
	unsigned char hashForChainUnfold[33];
	hashForChainUnfold[32] = '\0';
	
	int tries2 = 0;

	found = getPassFromHash(myHash, nonReadableHash);
	
	//char readableHash[65];
	//readableHash[64] = '\0';
	//changeFormat(readableHash, nonReadableHash, 0);
	//if (myHash->find(std::string(readableHash)) != myHash->end) {
	//	auto range = myHash->equal_range(std::string(readableHash));
	//	for_each(
	//		range.first,
	//		range.second,
	//		[](std::unordered_multimap <std::string, char*>::value_type& element) {
	//			strncpy(passForChainUnfold, element.second, 6);
	//			found = NULL;
	//			tries2 = 0;
	//			while (tries2 < CHAINLEN + 1) {
	//				Hash(256, (BitSequence*)passForChainUnfold, PASSLENINBIT, (BitSequence*)hashForChainUnfold);
	//				if (!memcmp(hashForChainUnfold, givenHash, 32)) {
	//					printf("Pass found.\n");
	//					return passForChainUnfold;
	//				}
	//				reduct(hashForChainUnfold, passForChainUnfold);
	//				tries2++;
	//			}
	//		}
	//	);
	//}

	if (found != NULL) {
		strncpy(passForChainUnfold, found, 6);
		found = NULL;
		tries2 = 0;
		while (tries2 < CHAINLEN + 1) {
			Hash(256, (BitSequence*)passForChainUnfold, PASSLENINBIT, (BitSequence*)hashForChainUnfold);
			if (!memcmp(hashForChainUnfold, givenHash, 32)) {
				printf("Pass found.\n");
				return passForChainUnfold;
			}
			reduct(hashForChainUnfold, passForChainUnfold);
			tries2++;
		}
	}
	else {
		while (tries < CHAINLEN - 1) {//&& found == NULL) {
			reduct(nonReadableHash, nextPassToHash);
			//printf("Next pass to hash: %s\n", nextPassToHash);
			Hash(256, (BitSequence*)nextPassToHash, PASSLENINBIT, (BitSequence*)nonReadableHash);
			found = getPassFromHash(myHash, nonReadableHash);
			tries++;
			if (found != NULL) {
				//printf("Found possible start of chain: %s, trying to retrieve the real pass.\n", found);
				strncpy(passForChainUnfold, found, 6);
				found = NULL;
				tries2 = 0;
				while (tries2 < CHAINLEN + 1) {
					Hash(256, (BitSequence*)passForChainUnfold, PASSLENINBIT, (BitSequence*)hashForChainUnfold);
					if (!memcmp(hashForChainUnfold, givenHash, 32)) {
						printf("Pass found.\n");
						return passForChainUnfold;
					}
					reduct(hashForChainUnfold, passForChainUnfold);
					tries2++;
				}
				//printf("Weird things happening. Tries: %d.\n", tries);
			}
		}
	}
	return NULL;
}
