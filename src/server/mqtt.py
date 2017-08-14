import json

import paho.mqtt.client as mqtt_client

from animations import ANIMATIONS

BEACON1 = 'area54/a020a61bdf18'
BEACON2 = 'area54/a020a61bc23f'
BEACON3 = 'area54/6001941c7e9b'
BEACON4 = 'area54/a020a61bd9cc'
BEACON5 = 'area54/a020a61be29a'
BEACON6 = 'area54/a020a616c8ed'
ALL_BEACONS = (BEACON1, BEACON2, BEACON3, BEACON4, BEACON5, BEACON6,)
ODD_BEACONS = (BEACON1, BEACON3, BEACON5,)
EVEN_BEACONS = (BEACON2, BEACON4, BEACON6, )


class MqttClient(mqtt_client.Client):

    HOST = 'localhost'

    @classmethod
    def get_client(cls):
        """Initialize and connect."""
        client = cls()
        client.connect(cls.HOST)
        return client

    def send_animation(self, address, animation='simon'):
        """Send the animation to the beacon address."""
        self.publish(address, json.dumps(ANIMATIONS[animation]))


if __name__ == '__main__':
    TOPIC = 'test'
    TEST_MSG = 'testing...'

    client = get_client()
    client.publish(TOPIC, TEST_MSG)
