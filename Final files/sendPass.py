#!/usr/bin/python

import sys
from socket import socket

if __name__ == '__main__':
    sock = socket()
    sock.connect(('195.134.71.138', 16666))
    sock.send(str(sys.argv[1])+"\n")
    sock.close()