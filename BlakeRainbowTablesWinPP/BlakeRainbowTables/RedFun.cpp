#include "RedFun.hpp"
#include <cstdint>
//#include <stdlib.h>					//strtol
//#include <stdio.h>
//
////void reduct(unsigned char* previousHash, char* destinationString, int destinationStringLength,
////		int charsToUse, int CharsTimesSize, const char* charTable, int tableSize) {
//void reduct(unsigned char* previousHash, char* destinationString, int destinationStringLength, const char* charTable) {
//	int charsToUse = 5;
//	int CharsTimesSize = 5;
//	int tableSize = 64;
//
//	char hashChunk[(charsToUse<<1)+1];
//	int counter, i;
//
//	hashChunk[(charsToUse<<1)] = '\0';
//
//	for (counter = 0; counter < destinationStringLength; counter++) {
//		for (i = 0; i < charsToUse; i++)
//			sprintf(hashChunk+(i<<1), "%02x", (previousHash+(counter*CharsTimesSize))[i]);
//
//		destinationString[counter] = charTable[(strtoull(hashChunk, NULL, 16) % tableSize)];
//	}
//	destinationString[destinationStringLength] = '\0';
//
//
//}
//
//
//void reduct(unsigned char* previousHash, char* destinationString, int destinationStringLength, const char* charTable) {
//	int i;
//	for(i=0;i<destinationStringLength;i++) {
//		destinationString[i] = charTable[(previousHash[(i<<2)] & 63)];
//	}
//}

extern const char* chractersToUse;

void reduct(unsigned char* previousHash, char* destinationString) {
	destinationString[0] = chractersToUse[(uint32_t)(previousHash[0] & 63)];
	destinationString[1] = chractersToUse[(uint32_t)(previousHash[4] & 63)];
	destinationString[2] = chractersToUse[(uint32_t)(previousHash[8] & 63)];
	destinationString[3] = chractersToUse[(uint32_t)(previousHash[12] & 63)];
	destinationString[4] = chractersToUse[(uint32_t)(previousHash[16] & 63)];
	destinationString[5] = chractersToUse[(uint32_t)(previousHash[20] & 63)];
}