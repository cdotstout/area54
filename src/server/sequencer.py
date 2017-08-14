from collections import OrderedDict
from threading import Thread
from time import sleep

import redis

import term
from audio_player import AudioPlayer
from mqtt import *


class Sequencer:

    def __init__(self):
        self.mqtt_client = MqttClient.get_client()

    def send_animation(self, address, animation):
        self.mqtt_client.send_animation(address, animation)


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
        term.clear()

    def play_step(self, step, animation='simon', play_audio=True):
        """Play the LED animation and audio, and display the step."""
        self.send_animation(self.ADDRESSES[step], animation)
        if self.audio_player and play_audio:
            self.audio_player.play_audio(step)
        self.print_to_term(step)

    def print_to_term(self, step):
        """Write the step to the window."""
        term.clear()
        print(self.format_step(step))

    def format_step(self, step):
        """Pad the step for display in the window."""
        return list(self.ADDRESSES).index(step) * ' ' + step

    def game_over(self):
        """Send game over animation to beacons and window."""
        term.clear()
        print('GAME OVER')
        # TODO this should send 1 msg to a broadcast topic
        for address in self.ADDRESSES.values():
            self.send_animation(address, 'game_over')
        if self.audio_player:
            self.audio_player.play_game_over()


class BpmSequencer(Sequencer):

    def __init__(self, playlist, bpm=120):
        super().__init__()
        self.redis_connection = redis.StrictRedis()
        self.bpm = bpm
        self.playlist = playlist
        self.timer_thread = None
        self.reset_timer()
        self._continue_timer = True

    @property
    def bpm(self):
        current_bpm = int(self.redis_connection.get('bpm'))
        if current_bpm is None:
            current_bpm = 120
        return current_bpm

    @bpm.setter
    def bpm(self, bpm):
        self.redis_connection.set('bpm', int(bpm))

    def generate_beats(self):
        for current_sequence, num_cycles in self.playlist:
            for _ in range(num_cycles):
                for beat in current_sequence:
                    if beat:
                        for cue in beat:
                            for address in cue.addresses:
                                self.send_animation(address, cue.animation)
                    yield

    def _start_timer(self):
        """Play beats from the sequence at the bpm rate."""
        beat_generator = self.generate_beats()
        while self._continue_timer:
            try:
                next(beat_generator)
            except StopIteration:
                beat_generator = self.generate_beats()
                next(beat_generator)
            sleep(60.0 / self.bpm)

    def start(self):
        """Start the timer thread."""
        self.timer_thread.start()

    def stop(self):
        """Stop and reset the timer thread."""
        self._continue_timer = False
        self.timer_thread.join(timeout=1.0)
        self.reset_timer()

    def reset_timer(self):
        self.timer_thread = Thread(target=self._start_timer, name='timer')
        self._continue_timer = True


class PlayList(tuple):
    pass


class BeatSequence(tuple):
    pass


class Beat(tuple):
    pass


class Cue:

    def __init__(self, addresses, animation):
        self.addresses = addresses
        self.animation = animation


def get_test_sequencer():
    even_fuchsia = Cue((BEACON2, BEACON4, BEACON6,), 'fuchsia')
    odd_red = Cue((BEACON1, BEACON3, BEACON5,), 'game_over')
    both_beat = Beat([even_fuchsia, odd_red, ])
    just_fuchsia_beat = Beat([even_fuchsia, ])
    both_seq = BeatSequence((both_beat, just_fuchsia_beat,))
    just_fuchsia_seq = BeatSequence((just_fuchsia_beat,))
    playlist = PlayList({both_seq: 3, just_fuchsia_seq: 1, })
    return BpmSequencer(playlist)


def test1():
    test_cue = Cue((BEACON1,), 'fuchsia')
    test_beat = Beat([test_cue, ])
    test_seq = BeatSequence((test_beat,))
    playlist = PlayList({test_seq: 1, })
    return BpmSequencer(playlist, 90)


def test2():
    test_fuchsia = Cue((BEACON1,), 'fuchsia')
    test_red = Cue((BEACON1,), 'red')
    beat_fuchsia = Beat([test_fuchsia, ])
    beat_red = Beat([test_red, ])
    test_seq = BeatSequence((beat_fuchsia,beat_red))
    playlist = PlayList({test_seq: 1, })
    return BpmSequencer(playlist, 118)


def test3():
    test_cue1 = Cue((BEACON1, ), 'new')
    test_cue2 = Cue((BEACON2, ), 'new')
    test_cue3 = Cue((BEACON3, ), 'new')
    test_cue4 = Cue((BEACON4, ), 'new')
    test_cue5 = Cue((BEACON5, ), 'new')
    test_cue6 = Cue((BEACON6, ), 'new')
    test_beat1 = Beat([test_cue1, test_cue2, test_cue3, test_cue4, test_cue5, test_cue6])
    # test_beat2 = Beat([test_cue2, ])
    # test_beat3 = Beat([test_cue3, ])
    # test_beat4 = Beat([test_cue4, ])
    # test_beat5 = Beat([test_cue5, ])
    # test_beat6 = Beat([test_cue6, ])
    test_seq1 = BeatSequence((test_beat1, ))
    # cue1 = Cue((BEACON1, BEACON3, BEACON5), 'redfast')
    # cue2 = Cue((BEACON4, BEACON6), 'redfast')
    # cue3 = Cue((BEACON1, BEACON3, BEACON4, BEACON5, BEACON6, ), 'game_over')
    # beat1 = Beat([cue1, ])
    # beat2 = Beat([cue2, ])
    # beat3 = Beat([cue3, ])
    # seq2 = BeatSequence((beat1,
    #                      beat2,
    #                      beat1,
    #                      beat2,
    #                      beat1, ))
    # seq3 = BeatSequence((beat3,
    #                      None,
    #                      beat3,
    #                      None,
    #                      beat3, ))
    # seq4 = BeatSequence((beat3, ))
    # noneseq = BeatSequence((None,
    #                        None,
    #                        None,
    #                        None,
    #                        None, ))
    playlist = PlayList(((test_seq1, 1), ))

    return BpmSequencer(playlist, 120)


if __name__ == '__main__':
    seq = test3()
    seq.start()
