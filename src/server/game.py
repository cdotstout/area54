import random
import signal
from time import sleep

import term
from sequencer import BpmSequencer, GameSequencer, get_test_sequencer

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
        while True:
            simon.play_level()

    def play_level(self):
        """Play a level using the current pattern."""
        self.sequencer.display_pattern(self.pattern)
        for step in self.pattern:
            if step != self.get_user_input():
                self.game_over()
                self.reset()
                return
            else:
                self.sequencer.play_step(step)
        self.reward()
        self.add_step()
        term.clear()

    def add_step(self):
        """Add a step to the pattern."""
        step = random.choice(list(self.sequencer.ADDRESSES))
        self.pattern.append(step)

    def get_user_input(self, timeout=True):
        """Get a keypress event and check that it's valid."""
        with term.get_raw_input(timeout) as step:
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
        self.get_user_input()


def main():
    # Handle alarm signal for user input timeout
    signal.signal(signal.SIGALRM, term.timeout)
    bpm_sequencer = get_test_sequencer()

    # Play Simon until there's a user input timeout, then switch to bpm mode.
    # Switch back to Simon on user input.
    while True:
        try:
            Simon.play_simon()
        except term.Timeout:
            bpm_sequencer.start()
            with term.get_raw_input(timeout=False) as _:
                pass
            bpm_sequencer.stop()


if __name__ == '__main__':
    main()
