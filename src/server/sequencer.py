import curses

from audio_player import AudioPlayer
from mqtt import MqttClient


class Sequencer:

    def __init__(self, window, audio):
        self.mqtt_client = MqttClient.get_client()
        self.window = window
        self.audio_sequencer = None
        if audio:
            self.audio_sequencer = AudioPlayer()

    def display_pattern(self, pattern, duration_between=500):
        """Display the pattern with the beacons and in the window."""
        for step in pattern:
            self.play_step(step)
            curses.napms(duration_between)
        self.window.clear()

    def play_step(self, step, animation='fuchsia', audio_category='owens'):
        """Play the LED animation and audio, and display the step."""
        self.mqtt_client.send_animation(step, animation)
        if self.audio_sequencer:
            self.audio_sequencer.play_audio(step)
        self.write_to_window(step)

    def write_to_window(self, step):
        """Write the step to the window."""
        self.window.clear()
        self.window.addstr(self.format_step(step), curses.A_BOLD)
        self.window.refresh()

    def format_step(self, step):
        """Pad the step for display in the window."""
        return list(self.ADDRESSES).index(step) * ' ' + step

    def game_over(self):
        """Send game over animation to beacons and window."""
        self.window.clear()
        self.window.addstr('GAME OVER')
        # TODO this should send 1 msg to a broadcast topic
        for address in self.ADDRESSES.values():
            self.mqtt_client.publish(address, json.dumps(ANIMATIONS['game_over']))
        if self.audio_sequencer:
            self.audio_sequencer.play_game_over()
