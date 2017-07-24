import curses
import json

from animations import ANIMATIONS
from mqtt import get_client


class Sequencer:

    ADDRESSES = {
        'a': 'area54/a020a61be29a',
        's': 'area54/a020a616c8ed',
        'd': 'area54/a020a61bdf18',
        'f': 'area54/a020a61bc23f',
        'g': 'area54/a020a61bd9cc',
        # 'h': 'area54/a020a61bd8aa',
    }

    def __init__(self, window):
        self.mqtt_client = get_client()
        self.window = window

    def display_pattern(self, pattern, duration_between=300):
        """Display the pattern with the beacons and in the window."""
        for step in pattern:
            self.send_animation(step)
            self.write_to_window(step)
            curses.napms(duration_between)
        self.window.clear()

    def send_animation(self, step, animation='fuchsia'):
        """Send the animation to the beacon."""
        self.mqtt_client.publish(self.ADDRESSES[step], json.dumps(ANIMATIONS[animation]))

    def write_to_window(self, step):
        """Write the step to the window."""
        self.window.clear()
        self.window.addstr(self.format_step(step), curses.A_BOLD)
        self.window.refresh()

    def format_step(self, step):
        """Pad the step."""
        return list(self.ADDRESSES).index(step) * ' ' + step

    def display_game_over(self):
        """Send game over animation to beacons and window."""
        self.window.clear()
        self.window.addstr('GAME OVER')
        # TODO this should send 1 msg to a broadcast topic
        for address in self.ADDRESSES.values():
            self.mqtt_client.publish(address, json.dumps(ANIMATIONS['game_over']))
