#!/usr/bin/env python

import sys
import traceback

import gobject

import dbus
import dbus.mainloop.glib

from socket import socket

sock = socket()
sock.connect(('127.0.0.1', 16666))


def catchall_signal_handler(*args, **kwargs):
    print("Got string: ")
    print(str(args[0]))
    print("\nChecking if it matches and trying to send it.\n")
    if kwargs['dbus_interface'] == "com.secure.PassGenService":
        sock.send(((str(args[0]))[33:]) + "\n")


if __name__ == '__main__':
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    bus = dbus.SessionBus()
    bus.add_signal_receiver(catchall_signal_handler, interface_keyword='dbus_interface', member_keyword='member')
    loop = gobject.MainLoop()
    loop.run()
    sock.close()
