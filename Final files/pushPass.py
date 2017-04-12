#!/usr/bin/env python

import sys
import socket

host = ''
port = 16666
backlog = 5
size = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host, port))
s.listen(backlog)
f = open('passServer.log', 'a')
while 1:
    client, address = s.accept()
    data = client.recv(size)
    if data:
        sock = socket.socket()
        sock.connect(('127.0.0.1', 4433))
        print (sock.recv(512))
        sock.send(str(data))
	temp = sock.recv(65536)
        print (temp)
        f.write(temp)
    client.close()
f.close()
sock.close()
