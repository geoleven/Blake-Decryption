#ifndef REDFUN_H_
#define REDFUN_H_

//Takes a hex hash value and reduce it to 6 letters string, needs newString malloced with 7*char
//void reduct(unsigned char* previousHash, char* destinationString, int destinationStringLength, const char* charTable);
void reduct(unsigned char* previousHash, char* destinationString);

#endif /* REDFUN_H_ */
