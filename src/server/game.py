import random
from time import sleep

import term
from sequencer import BpmSequencer, GameSequencer, PLAYLISTS

from mqtt import ALL_BEACONS

random = random.SystemRandom()
DELAY_INTERVAL = 1000


class Simon:

    def __init__(self, audio=True):
        self.sequencer = GameSequencer(audio)
        self.pattern = []
        self.add_step()

    @classmethod
    def play_simon(cls, audio=True):
        """Create a Simon instance then play forever."""
        simon = cls(audio)
        # set a 20 second timeout
        print('Playing simon...')
        print('Blacking out...')
        for beacon in ALL_BEACONS:
            simon.sequencer.mqtt_client.send_animation(beacon, 'blackout')
            sleep(0.5)
        while True:
            simon.play_level()

    def play_level(self):
        """Play a level using the current pattern."""
        print('Displaying pattern...')
        self.sequencer.display_pattern(self.pattern)
        for step in self.pattern:
            print('Waiting for user input...')
            if step != self.get_user_input():
                print('Wrong input!')
                self.game_over()
                self.reset()
                return
            else:
                print('Correct input!')
                self.sequencer.play_step(step)
        self.reward()
        self.add_step()

    def add_step(self):
        """Add a step to the pattern."""
        step = random.choice(list(self.sequencer.ADDRESSES))
        self.pattern.append(step)

    def get_user_input(self, timeout_duration=10):
        """Get a keypress event and check that it's valid."""
        while True:
            step = term.get_raw_input(timeout_duration)
            if step in self.sequencer.ADDRESSES.keys():
                return step

    def reward(self):
        """Play a reward animation then briefly wait."""
        # TODO add reward animation
        sleep(1)

    def reset(self):
        """Reset the pattern."""
        self.pattern = []
        sleep(0.5)

    def game_over(self):
        """Display game over animation then wait for a button press."""
        self.sequencer.game_over()
        # press any key to continue
        # self.get_user_input()
        sleep(1.0)


def main():
    # Set the terminal to raw mode and set signal handlers
    term.setup()

    bpm_sequencer = BpmSequencer(PLAYLISTS)

    # Play Simon until there's a user input timeout, then switch to bpm mode.
    # Switch back to Simon on user input.
    while True:
        try:
            Simon.play_simon()
        except term.Timeout:
            bpm_sequencer.start()
            term.get_raw_input(timeout_duration=None)
            bpm_sequencer.stop()


if __name__ == '__main__':
    main()
