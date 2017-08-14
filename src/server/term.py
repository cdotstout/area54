import signal
import sys
import termios
import tty


class Timeout(Exception):
    pass


def timeout(sig, frame):
    signal.alarm(0)
    print('Timed out: no user input')
    raise Timeout('No user input')


def get_settings(fd):
    return termios.tcgetattr(fd)


def set_raw(fd):
    # tty.setraw(fd)
    tty.setcbreak(fd)


def set_default(fd, settings):
    termios.tcsetattr(fd, termios.TCSADRAIN, settings)


def get_raw_input(timeout_duration=10):
    if timeout_duration is not None:
        signal.alarm(timeout_duration)
    return sys.stdin.read(1)

