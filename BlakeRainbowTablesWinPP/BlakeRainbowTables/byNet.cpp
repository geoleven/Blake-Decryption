#include "byNet.hpp"
#include "SearchRBT.hpp"
#include <pthread.h>
#include <unordered_map>

#include <iostream>

#define PASSLEN 6

pthread_t threadId;
pthread_attr_t attr;
std::unordered_map <std::string, char*>* myHash;

static	bool quit = false;

static void* netThread(void* argsptr) {
	SocketHandler h;
	ListenSocket<myS> l(h);
	if (l.Bind(16666)){
		exit(-1);
	}
	h.Add(&l);
	h.Select(1, 0);
	while (!quit) {
		h.Select(1, 0);
	}
	return NULL;
}


void startNet(void* pthash) {
	myHash = (std::unordered_map <std::string, char*>*)pthash;
	if (pthread_attr_init(&attr) != 0)
		printf("Thread attribute initialization not completed\n");
	if (pthread_create(&threadId, &attr, &netThread, NULL)) {
		printf("Error creating thread!");
	}	
	//return 0;
}

void endNet() {
	double trash = 0;
	pthread_join(threadId, (void**)(&trash));
}

myS::myS(ISocketHandler& h) : TcpSocket(h) {
	SetLineProtocol();
}

void myS::OnLine(const std::string& line) {
	char* password = (char*)malloc(sizeof(char)*(PASSLEN + 1));
	char* leakedHash = (char*)malloc(sizeof(char) * 65);
	leakedHash[64] = '\0';
	std::cout << line << "\n" << std::endl;
	memcpy(leakedHash, line.c_str(), 64);
	//strncpy(leakedHash, line.c_str(), 64);
	printf("Going to check for a password. . . \n");
	password = reverseHash((void*)myHash, leakedHash);
	if (password != NULL) {
		printf("The password generating the given hash is:\n\n%s\n\n", password);
		std::string systemCall = "\"python sendPass.py " + std::string(password) +"\"";
		system(systemCall.c_str());
	}
}