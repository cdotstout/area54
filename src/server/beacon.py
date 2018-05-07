import json
import bluepy

from bluepy import btle
from animations import ANIMATIONS

BEACON1 = "30:AE:A4:7A:F7:9A"
BEACON2 = "30:AE:A4:75:6F:D6"
BEACON3 = "30:AE:A4:7B:01:8A"
BEACON4 = 4
BEACON5 = 5
BEACON6 = 6
#ALL_BEACONS = (BEACON1, BEACON2, BEACON3, BEACON4, BEACON5, BEACON6,)
#ALL_BEACONS = (BEACON1, BEACON2,)
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
        self.dev = {}
        self.svc = {}
        self.char = {}

    def connect(self):
        print("BeaconClient::connect")
        for beacon in ALL_BEACONS:
            print("Connecting to %s..." % beacon)
            device = btle.Peripheral(beacon)
            device.setMTU(550)
            self.dev[beacon] = device
            service = device.getServiceByUUID(service_uuid)
            self.svc[beacon] = service
            characteristics = service.getCharacteristics(char_uuid)
            self.char[beacon] = characteristics[0]

    def send_animation(self, address, animation='simon'):
        print("BeaconClient::send_animation %s" % address)
        if address == BEACON1 or address == BEACON2 or address == BEACON3:
            #self.char[address].write(self.encode(ANIMATIONS[animation]))
            self.char[address].write(json.dumps(ANIMATIONS[animation]).encode())

    def encode(self, json):
        msg = bytearray()
        msg.append(0x0A) # header 1
        msg.append(0x54) # header 2

        if (json["name"] == "AnimatedProgram"):
            msg.append(0x01) # element-type: name
            msg.append(0x01) # AnimatedProgram
        else:
            print("Didn't find a name")
        
        segments = json["segments"]
        msg.append(0x02) # element-type: segment
        msg += len(segments).to_bytes(1, byteorder='little') # number of segments (max 255)

        for segment in segments:
            if (segment["duration"]):
                msg.append(0x03)
                msg += int(segment["duration"]).to_bytes(2, byteorder='little')

        return msg
