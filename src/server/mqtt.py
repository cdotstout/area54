import json

import paho.mqtt.client as mqtt_client

from animations import ANIMATIONS


class MqttClient(mqtt_client.Client):

    HOST = '192.168.44.10'
    ADDRESSES = {
        'a': 'area54/a020a61be29a',
        's': 'area54/a020a616c8ed',
        'd': 'area54/a020a61bdf18',
        'f': 'area54/a020a61bc23f',
        'g': 'area54/a020a61bd9cc',
        'h': 'area54/a020a61bd8aa',
    }

    @classmethod
    def get_client(cls):
        """Initialize and connect."""
        client = cls()
        client.connect(cls.HOST)
        return client

    def get_address(self, step):
        """Get the topic for the step."""
        return self.ADDRESSES[step]

    def send_animation(self, step, animation='fuchsia'):
        """Send the animation to the beacon."""
        self.publish(self.ADDRESSES[step], json.dumps(ANIMATIONS[animation]))


if __name__ == '__main__':
    TOPIC = 'test'
    TEST_MSG = 'testing...'

    client = get_client()
    client.publish(TOPIC, TEST_MSG)
