import json

import paho.mqtt.client as mqtt_client

from animations import ANIMATIONS

BEACON1 = 'area54/a020a61be29a'
BEACON2 = 'area54/a020a616c8ed'
BEACON3 = 'area54/a020a61bdf18'
BEACON4 = 'area54/a020a61bc23f'
BEACON5 = 'area54/a020a61bd9cc'
BEACON6 = 'area54/6001941c7e9b'


class MqttClient(mqtt_client.Client):

    HOST = 'localhost'

    @classmethod
    def get_client(cls):
        """Initialize and connect."""
        client = cls()
        client.connect(cls.HOST)
        return client

    def send_animation(self, address, animation='fuchsia'):
        """Send the animation to the beacon address."""
        self.publish(address, json.dumps(ANIMATIONS[animation]))


if __name__ == '__main__':
    TOPIC = 'test'
    TEST_MSG = 'testing...'

    client = get_client()
    client.publish(TOPIC, TEST_MSG)
