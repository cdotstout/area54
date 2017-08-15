import curses
import random
import sys

import term
from sequencer import BpmSequencer, GameSequencer, get_test_sequencer

random = random.SystemRandom()
DELAY_INTERVAL = 1000


class Simon:

    def __init__(self, window, audio=True):
        self.window = window
        self.sequencer = GameSequencer(window, audio)
        self.pattern = []
        self.add_step()

    @classmethod
    def play_simon(cls, window, audio=True):
        """Create a Simon instance then play forever."""
        simon = cls(window, audio)
        # set a 20 second timeout
        curses.halfdelay(100)
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
        self.window.clear()

    def add_step(self):
        """Add a step to the pattern."""
        step = random.choice(list(self.sequencer.ADDRESSES))
        self.pattern.append(step)

    def get_user_input(self):
        """Get a keypress event and check that it's valid."""
        step = self.window.getkey()
        if step in self.sequencer.ADDRESSES.keys():
            return step

    def reward(self):
        """Play a reward animation then briefly wait."""
        # TODO add reward animation
        curses.napms(1000)

    def reset(self):
        """Reset the pattern."""
        self.pattern = []
        curses.napms(500)

    def game_over(self):
        """Display game over animation then wait for a button press."""
        self.sequencer.game_over()
        # press any key to continue
        self.window.getkey()

if __name__ == '__main__':
    bpm_sequencer = get_test_sequencer()
    fd = sys.stdin.fileno()
    term_settings = term.get_settings(fd)
    while True:
        try:
            curses.wrapper(Simon.play_simon)
        except curses.error:
            bpm_sequencer.start()
            with term.get_raw_input() as stop:
                print(stop)
            bpm_sequencer.stop()
