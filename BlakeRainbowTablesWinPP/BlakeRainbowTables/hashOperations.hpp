#ifndef HASHOPERATIONS_H_
#define HASHOPERATIONS_H_

#include <unordered_map>

#define PASSLEN 6
#define PASSLENINBIT PASSLEN * CHAR_BIT
#define CHAININFOSIZE 40

struct uchar32Hash {
	std::size_t operator()(const unsigned char*& key) const {
		return std::hash<unsigned long long>()(*key) ^ std::hash<unsigned long long>()(*(key+8)) ^ (std::hash<unsigned long long>()(*(key+16)) << 1) ^ std::hash<unsigned long long>()(*(key+24));
	}
};

struct eqstr {
	bool operator()(const unsigned char*& bs1, unsigned char*& bs2) const {
		return (memcmp(bs1, bs2, 32) == 0);
	}
};

void changeFormat(char* read, unsigned char* unread, int mode);
int addHash(std::unordered_map <std::string, char*>* hash, unsigned char* passPlusHash);
void destroyHash(std::unordered_map <std::string, char*>* hash);
char* getPassFromHash(std::unordered_map <std::string, char*>* hash, unsigned char* leakedHashNonReadable);
//std::pair<std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, char*>>>>, std::_List_iterator<std::_List_val<std::_List_simple_types<std::pair<const std::string, char*>>>>> getPassFromHash(std::unordered_map<std::string, char*>* myHash, unsigned char* leakedHashNonReadable);
int loadFromFile(std::unordered_map <std::string, char*>* hash, char* fileName, unsigned long int* entries, unsigned long int* collisions);


#endif /* HASHOPERATIONS_H_ */
