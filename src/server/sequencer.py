import curses
from threading import Thread
from time import sleep

from audio_player import AudioPlayer
from mqtt import *


class GameSequencer:

    ADDRESSES = {
        'a': BEACON1,
        's': BEACON2,
        'd': BEACON3,
        'f': BEACON4,
        'g': BEACON5,
        'h': BEACON6,
    }

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

    def play_step(self, step, animation='fuchsia'):
        """Play the LED animation and audio, and display the step."""
        self.mqtt_client.send_animation(self.ADDRESSES[step], animation)
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
            self.mqtt_client.send_animation(address, 'game_over')
        if self.audio_sequencer:
            self.audio_sequencer.play_game_over()


class BpmSequencer:

    def __init__(self):
        self.mqtt_client = MqttClient.get_client()
        self.bpm = 120
        self.current_sequence = None
        self.counter = 0
        self.timer_thread = None
        self.reset_timer()
        self._stop_timer = False

    def play_beat(self):
        """Play the current beat in the sequence."""
        # Send the command to each address
        current_beat = self.current_sequence[self.counter]
        for address in current_beat.addresses:
            self.mqtt_client.send_animation(address, current_beat.animation)
        # Increment and roll over the counter
        self.counter += 1
        if self.counter >= len(self.current_sequence):
            self.counter = 0

    def start_timer(self):
        """Play beats from the sequence at the bpm rate."""
        while not self._stop_timer:
            self.play_beat()
            sleep(60.0 / self.bpm)

    def start(self):
        """Start the timer thread."""
        self.timer_thread.start()

    def stop(self):
        """Stop and reset the timer thread."""
        self._stop_timer = True
        self.timer_thread.join(timeout=0.5)
        self.reset_timer()

    def reset_timer(self):
        self.timer_thread = Thread(target=self.start_timer, name='timer')
        self._stop_timer = False


class BeatSequence(list):
    pass


class Beat:

    def __init__(self, addresses, animation):
        self.addresses = addresses
        self.animation = animation


if __name__ == '__main__':
    even_fuchsia = Beat((BEACON2, BEACON4, BEACON6), 'fuchsia')
    odd_red = Beat((BEACON1, BEACON3, BEACON5), 'game_over')
    seq = BeatSequence([even_fuchsia, odd_red])
    sequencer = BpmSequencer()
    sequencer.current_sequence = seq
    sequencer.start()
