import signal
import sys
import termios
import tty


class Timeout(Exception):
    pass


def setup():
    # Handle alarm signal for user input timeout
    signal.signal(signal.SIGALRM, timeout)

    # Set terminal to raw mode and fix terminal on sigterm
    fd = sys.stdin.fileno()
    default_settings = get_settings(fd)
    set_raw(fd)

    def reset_term(sig, frame):
        set_default(fd, default_settings)
        sys.exit()

    signal.signal(signal.SIGTERM, reset_term)


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

