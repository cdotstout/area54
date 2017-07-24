import paho.mqtt.client as mqtt_client

HOST = 'pihole.deserts'


def get_client():
    """Create a mqtt client and connect to the server."""
    client = mqtt_client.Client()
    client.connect(HOST)
    return client


if __name__ == '__main__':
    TOPIC = 'test'
    TEST_MSG = 'testing...'

    client = get_client()
    client.publish(TOPIC, TEST_MSG)
