#ifndef BYNET_H_
#define BYNET_H_

#include "../sockets/SocketHandler.h"
#include "../sockets/ListenSocket.h"
#include "../sockets/TcpSocket.h"
#include "../sockets/ISocketHandler.h"
//#include "hashOperations.hpp"

void startNet(void* myHash);
void endNet();

class myS : public TcpSocket{
public:
	myS(ISocketHandler&);
	void OnLine(const std::string&);

};

#endif /* BYNET_H_ */