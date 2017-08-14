import signal
import sys
import termios
import tty
from contextlib import contextmanager


class Timeout(Exception):
    pass


def timeout(sig, frame):
    signal.alarm(0)
    raise Timeout('No user input')


def get_settings(fd):
    return termios.tcgetattr(fd)


def set_raw(fd):
    # tty.setraw(fd)
    tty.setcbreak(fd)


def set_default(fd, settings):
    termios.tcsetattr(fd, termios.TCSADRAIN, settings)


def clear():
    # CLEAR = '\033[F'
    # CLEAR = '\033[K'
    # sys.stdout.write(CLEAR)
    pass


@contextmanager
def get_raw_input(use_timeout=True):
    fd = sys.stdin.fileno()
    default_settings = get_settings(fd)
    if use_timeout:
        signal.alarm(5)
    set_raw(fd)
    yield sys.stdin.read(1)
    set_default(fd, default_settings)
