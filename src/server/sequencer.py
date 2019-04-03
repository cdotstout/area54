#!/usr/bin/python3

from random import randrange, shuffle
from threading import Thread
from time import sleep

#from audio_player import AudioPlayer
from beacon import *


class Sequencer:

    def __init__(self):
        self.beacon_client = BeaconClient.get_client()

    def send_animation(self, address, animation):
        self.beacon_client.send_animation(address, animation)


class GameSequencer(Sequencer):

    ADDRESSES = {
        'a': BEACON1,
        's': BEACON2,
        'd': BEACON3,
        'f': BEACON4,
        'g': BEACON5,
        'h': BEACON6,
    }

    def __init__(self, audio):
        super().__init__()
        self.audio_player = None
        if audio:
            self.audio_player = AudioPlayer()

    def display_pattern(self, pattern, duration_between=0.5):
        """Display the pattern with the beacons and in the window."""
        for step in pattern:
            self.play_step(step, play_audio=False)
            sleep(duration_between)

    def play_step(self, step, animation='simon', play_audio=True):
        """Play the LED animation and audio, and display the step."""
        self.send_animation(self.ADDRESSES[step], animation)
        if self.audio_player and play_audio:
            self.audio_player.play_audio(step)
        self.print_to_term(step)

    def print_to_term(self, step):
        """Write the step to the window."""
        print(self.format_step(step))

    def format_step(self, step):
        """Pad the step for display in the window."""
        return list(self.ADDRESSES).index(step) * ' ' + step

    def game_over(self):
        """Send game over animation to beacons and window."""
        print('GAME OVER')
        # TODO this should send 1 msg to a broadcast topic
        for address in self.ADDRESSES.values():
            self.send_animation(address, 'game_over')
        if self.audio_player:
            self.audio_player.play_game_over()


class BpmSequencer(Sequencer):

    def __init__(self, playlists):
        super().__init__()
        self.bpm = None
        self.playlists = playlists
        self.timer_thread = None
        self.reset_timer()
        self._continue_timer = True

    def generate_beats(self):
        """Send a beat animation from playlists."""
        shuffled_playlists = list(self.playlists)
        shuffle(shuffled_playlists)
        for playlist in shuffled_playlists:
            self.bpm = playlist.bpm
            for _ in range(randrange(4, 9, 2)):
                beats = playlist.generate_beats()
                for beat in beats:
                    if beat:
                        for msg in beat.get_messages():
                            self.send_animation(*msg)
                    yield

            print('Switching playlists...')

    def _start_timer(self):
        """Play beats from the sequence at the bpm rate forever."""
        beat_generator = self.generate_beats()
        while self._continue_timer:
            try:
                print('Sequencer beat...')
                next(beat_generator)
            # Repeat forever
            except StopIteration:
                beat_generator = self.generate_beats()
                next(beat_generator)
            sleep(60.0 / self.bpm)

    def start(self):
        """Start the timer thread."""
        print('Starting sequencer...')
        self.timer_thread.start()

    def stop(self):
        """Stop and reset the timer thread."""
        print('Stopping sequencer...')
        self._continue_timer = False
        self.timer_thread.join(timeout=1.0)
        self.reset_timer()
        print('Stopped sequencer')

    def reset_timer(self):
        self.timer_thread = Thread(target=self._start_timer, name='timer')
        self._continue_timer = True


class PlayList:

    def __init__(self, beat_sequences, bpm):
        self.beat_sequences = beat_sequences
        self.bpm = bpm

    def __iter__(self):
        yield from self.beat_sequences

    def generate_beats(self):
        for current_sequence, num_cycles in self:
            for _ in range(num_cycles):
                for beat in current_sequence:
                    yield beat


class BeatSequence(tuple):
    pass


class Beat(tuple):

    def get_messages(self):
        for cue in self:
            #for address in cue.addresses:
                #yield (address, cue.animation)
            yield (cue.addresses, cue.animation)


class Cue:

    def __init__(self, addresses, animation):
        self.addresses = addresses
        self.animation = animation


def pulse():
    pulse_cue1 = Cue(BEACON1, 'pulse')
    pulse_cue2 = Cue(BEACON2, 'pulse')
    pulse_cue3 = Cue(BEACON3, 'pulse')
    pulse_beat1 = Beat((pulse_cue1, ))
    pulse_beat2 = Beat((pulse_cue2, ))
    pulse_beat3 = Beat((pulse_cue3, ))
    pulse_seq = BeatSequence((pulse_beat1, None, pulse_beat2, None, 
        pulse_beat3, None, pulse_beat1, None, 
        pulse_beat2, None, pulse_beat3, None,
        ))
    return PlayList(((pulse_seq, 1), ), bpm=120)

def swirl():
    test_cue1 = Cue((BEACON1, ), 'swirl_pulse')
    test_cue2 = Cue((BEACON2, ), 'swirl_pulse')
    test_cue3 = Cue((BEACON3, ), 'swirl_pulse')
    test_cue4 = Cue((BEACON4, ), 'swirl_pulse')
    test_cue5 = Cue((BEACON5, ), 'swirl_pulse')
    test_cue6 = Cue((BEACON6, ), 'swirl_pulse')
    test_beat1 = Beat([test_cue1, ])
    test_beat2 = Beat([test_cue2, ])
    test_beat3 = Beat([test_cue3, ])
    test_beat4 = Beat([test_cue4, ])
    test_beat5 = Beat([test_cue5, ])
    test_beat6 = Beat([test_cue6, ])
    test_seq1 = BeatSequence((test_beat1,
                              test_beat2,
                              test_beat3,
                              test_beat4,
                              test_beat5,
                              test_beat6, ))
    test_seq1_rev = BeatSequence((test_beat6,
                                  test_beat5,
                                  test_beat4,
                                  test_beat3,
                                  test_beat2,
                                  test_beat1, ))
    none_seq = BeatSequence((None, None, None, None, ))
    steady_cue = Cue(ALL_BEACONS, 'swirl_steady')
    steady_beat = Beat((steady_cue, ))
    steady_seq = BeatSequence((steady_beat, ))
    slow_cue = Cue(ALL_BEACONS, 'swirl_pulse_slow')
    slow_beat = Beat((slow_cue, ))
    slow_seq = BeatSequence((slow_beat, ))
    return PlayList(((test_seq1, 7),
                     (steady_seq, 1),
                     (slow_seq, 1),
                     (none_seq, 3),
                     (slow_seq, 1),
                     (none_seq, 3),
                     (test_seq1_rev, 7),
                     (steady_seq, 1),
                     (slow_seq, 1),
                     (none_seq, 3),
                     (slow_seq, 1),
                     (none_seq, 3)), bpm=360)


def cool():
    cool_cue = Cue((BEACON1,
                    BEACON2,
                    BEACON3,
                    #BEACON4,
                    #BEACON5,
                    #BEACON6
                    ), 'cool')
    cool_beat = Beat((cool_cue, ))
    cool_seq = BeatSequence((cool_beat, None, None, None, None, None, None, None, ))
    return PlayList(((cool_seq, 1), ), bpm=120)

def pattyjill():
    cue = Cue((BEACON1,
                    BEACON2,
                    BEACON3,
                    #BEACON4,
                    #BEACON5,
                    #BEACON6
                    ), 'tbd')
    beat = Beat((cue, ))
    seq = BeatSequence((beat, None, ))
    return PlayList(((seq, 1), ), bpm=120)


def bounce():
    bounce_cue = Cue(ALL_BEACONS, 'bounce')
    bounce_beat = Beat((bounce_cue, ))
    bounce_seq = BeatSequence((bounce_beat, None, None, ))
    return PlayList(((bounce_seq, 1), ), bpm=120)


def test_simon():
    simon_cue = Cue(ALL_BEACONS, 'simon')
    simon_beat = Beat((simon_cue, ))
    simon_seq = BeatSequence((simon_beat, None, simon_beat, None))
    return PlayList(((simon_seq, 1), ), bpm=24)


#PLAYLISTS = (cool(), pulse(), bounce())
PLAYLISTS = (pattyjill(), )
#PLAYLISTS = (test_simon(), )
#PLAYLISTS = (cool(), )
#PLAYLISTS = (pulse(), )

if __name__ == '__main__':
    seq = BpmSequencer(PLAYLISTS)
    seq.start()
