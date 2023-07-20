#!/usr/bin/python

import select
import socket
import sys

#UDP_IP = "10.0.1.75" # rapi wifi on Jersey
UDP_IP = "192.168.1.2" # raspi wifi on Robot Cart
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#sock.bind(('', UDP_PORT))

packet = bytearray(1)
if len(sys.argv) > 1:
    packet[0] = int(sys.argv[1])

result = sock.sendto(packet, (UDP_IP, 5005))
print 'bytes sent: %d' % result
