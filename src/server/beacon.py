import json
import socket

from animations import AnimationGenerator

MCAST_GRP = '224.1.1.1'
MCAST_PORT = 5007

# New ESP32 rovers
BEACON1 = "30:AE:A4:C7:01:38"
BEACON2 = "30:AE:A4:C6:FC:68"
BEACON3 = "30:AE:A4:C5:5D:4C"

# Old ESP32s
#BEACON1 = "30:AE:A4:7A:F7:98"
#BEACON2 = "30:AE:A4:75:6F:D4"
#BEACON3 = "30:AE:A4:7B:01:88"
BEACON4 = 4
BEACON5 = 5
BEACON6 = 6
#ALL_BEACONS = (BEACON1, BEACON2, BEACON3, BEACON4, BEACON5, BEACON6,)
ALL_BEACONS = (BEACON1, BEACON2, BEACON3,)
ODD_BEACONS = (BEACON1, BEACON3, BEACON5,)
EVEN_BEACONS = (BEACON2, BEACON4, BEACON6, )

service_uuid="6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
char_uuid="6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

class BeaconClient:

    @classmethod
    def get_client(cls):
        """Initialize and connect."""
        client = cls()
        client.connect()
        return client

    def __init__(self):
        self.sock = {}

    def connect(self):
        print("BeaconClient::connect")
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 32)
        # Requres superuser privilege - needed if there are multiple interfaces
        # (say ethernet connected)
        self.sock.setsockopt(socket.SOL_SOCKET, 25, str("wlan0"+'\0').encode('utf-8'))

    def send_animation(self, addresses, animation='simon'):
        msg = AnimationGenerator.Get(addresses, animation)
        try:
            self.sock.sendto(msg, (MCAST_GRP, MCAST_PORT))
        except:
            print("Caught socket exception")
