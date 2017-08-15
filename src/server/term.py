import sys
import termios
import tty
from contextlib import contextmanager


def get_settings(fd):
    return termios.tcgetattr(fd)


def set_raw(fd):
    tty.setraw(fd)


def set_default(fd, settings):
    termios.tcsetattr(fd, termios.TCSADRAIN, settings)


@contextmanager
def get_raw_input():
    fd = sys.stdin.fileno()
    default_settings = get_settings(fd)
    set_raw(fd)
    yield sys.stdin.read(1)
    set_default(fd, default_settings)

