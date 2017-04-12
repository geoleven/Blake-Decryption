#include "blake_ref.hpp"
#include "CreateRBT.hpp"
#include <cstring>
#include "SearchRBT.hpp"
#include <cstdio>
#include "hashOperations.hpp"
#include <unordered_map>
#include <iostream>
#include "byNet.hpp"

#include "RedFun.hpp"


extern const char* chractersToUse = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@";

int main(int argc, char* argv[]) {
	int choice = 0;
	int noUse;
	printf("For creation of the rainbow table press: \t1.\nTo search a rainbow table press: \t\t2.\n");
	noUse = scanf("%d", &choice);
	while (choice != 1 && choice != 2 && choice != 3) {
		printf("No such choice. Please try again:\n");
		noUse = scanf("%d", &choice);
	}
	if (choice == 1) {
		printf("You are about to create a rainbow table (for Blake).\n");
		printf("Please give the chain size:\n");
		int chainSize = 0;
		noUse = scanf("%d", &chainSize);
		while(chainSize < 1) {
			printf("Chain size cannot be less than 1! Please try again:\n");
			noUse = scanf("%d", &chainSize);
		}
		printf("Please give the number of chains:\n");
		int chainNumber = 0;
		noUse = scanf("%d", &chainNumber);
		while(chainNumber < 2) {
			printf("Chains cannot be less than 2! Please try again:\n");
			noUse = scanf("%d", &chainNumber);
		}
		printf("Please give the number of letters the passwords will be:\n");
		int letters = 0;
		noUse = scanf("%d", &letters);
		while(letters < 2) {
			printf("Passwords cannot be less than 2 letters long! Please try again:\n");
			noUse = scanf("%d", &letters);
		}
		printf("Please give the name of the file to save the table:\n");
		char file[50];
		memset(file, 0 , 50);
		noUse = scanf("%49s", file);
		while(strlen(file) < 3 || file == NULL) {
			printf("File names cannot be less than 3 letters long! Please try again:\n");
			noUse = scanf("%49s", file);
		}
		printf("Now please wait.\nLetters: %d. Chains: %d. Chain Size: %d. File: %s.\n", letters, chainNumber, chainSize, file);
		createRBT(letters, chainSize, chainNumber, file);
		printf("Done. Exiting now.\n");
	}
	else if (choice == 2) {
		unsigned long int collisions = 0;
		unsigned long int entries = 0;
		std::unordered_map <std::string, char*> myHash;
		printf("You are about to load in a hash table the specified rainbow table (for Blake).\n");
		printf("Please give the name (and path) of the file:\n");
		char file[50];
		memset(file, 0 , 50);
		noUse = scanf("%49s", file);
		while(strlen(file) < 3 || file == NULL) {
			printf("File names cannot be less than 3 letters long! Please try again:\n");
			noUse = scanf("%49s", file);
		}
		printf("Please wait as the table is properly loaded...\n");
		if (loadFromFile(&myHash, file, &entries, &collisions)) {
			printf("Would you like to write the hashes or take them from a socket?\n");
			printf("1: File.\n2: Socket.\n");
			noUse = scanf("%d", &choice);
			while (choice != 1 && choice != 2) {
				printf("1: File.\n2: Socket.\n");
				noUse = scanf("%d", &choice);
			}
			if (choice == 1)  {
				int flag = 1;
				while (flag) {
					printf("You are now ready to search for the password of a given hash.\n");
					printf("Please write the human readable format of the hash:\n");
					char* password;
					char leakedHash[65];
					noUse = scanf("%64s", leakedHash);
					while (leakedHash[63] == '\0') {
						printf("The entered hash is not in the correct format, please try again:\n");
						noUse = scanf("%64s", leakedHash);
					}
					//printf("Trying to reverse the hash, be patient...\n");
					password = reverseHash((void*)&myHash, leakedHash);
					if (password != NULL) {
						printf("The password generating the given hash is:\n\n%s\n\n", password);
						//flag = 0;
					}
					else {
						//printf("Sorry the password could not be found on this rainbow table.\n");
					}
					//printf("Do you want to try another one?\n");
					//printf("Yes:\t1.\nNo:\t2.\n");
					//noUse = scanf("%d", &choice);
					choice = 1;
					while (choice != 1 && choice != 2) {
						printf("No such choice. Please try again:\n");
						noUse = scanf("%d", &choice);
					}
					if (choice == 1) {
						//printf("OK, let's try once more.\n");
					}
					else if (choice == 2) {
						flag = 0;
					}
					else {
						flag = 0;
						printf("This is not even possible (normally)!\n");
					}
				}
			} else {
				startNet((void*)&myHash);

				//printf("check\n");

				endNet();
			}
		} else {
			printf("Loading encountered some problem. Exiting.\n");
		}
		destroyHash(&myHash);
	//} else if (choice == 3) {
	//	char ps[7] = "CAAAAA";
	//	unsigned char* out = (unsigned char*)malloc(sizeof(char) * 32);
	//	int i;
	//	for (i = 0; i < 200; i++) {
	//		Hash(256, (BitSequence*)ps, 48, (BitSequence*)out);
	//		printf("Password to be found: %s\n", ps);
	//		reduct(out, ps);
	//	}
	//	for (i = 0; i < 32; i++)
	//		printf("%02x", out[i]);
	//	printf("\n");
	//	FILE* fd = fopen("test", "r");
	//	char buffer[40];
	//	for (i = 0; i < 3000; i++){
	//		fread(buffer, 40, 1, fd);
	//		if (!strncmp((char *)out, buffer + 7, 32)){
	//			printf("Found it\n");
	//			printf("Starting password of the chain: %6s\n", buffer);
	//			for (i = 0; i < 32; i++)
	//				printf("%02x", (unsigned char)(buffer + 7)[i]);
	//			printf("\n");
	//		}
	//	}
	//	fclose(fd);
	} else
		printf("This is not even possible (normally)!\n");
	if (noUse)
		noUse = 0;
	printf("Exiting.\n");
	scanf("%d", &noUse);
	return 0;
}
