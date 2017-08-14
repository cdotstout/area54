import os

import vlc


CATEGORIES = {
    'owen': {
        'a': 'owen/wow1.mp3',
        's': 'owen/wow2.mp3',
        'd': 'owen/wow3.mp3',
        'f': 'owen/wow6.mp3',
        'g': 'owen/wow5.mp3',
        'h': 'owen/wow4.mp3',
        'game_over': 'owen/wow4.mp3',
    }
}


class AudioPlayer:

    AUDIO_ROOT = '{}/{}/'.format(os.getcwd(), 'audio')

    def __init__(self):
        self.vlc_instance = vlc.Instance()
        self.players = {
            step: self.vlc_instance.media_player_new()
            for step
            in ('a', 's', 'd', 'f', 'g', 'h', 'game_over')
        }
        self.set_tracks('owen')

    def set_tracks(self, category):
        """Load the category's audio tracks."""
        audio_category = CATEGORIES[category]
        for step, file_name in audio_category.items():
            track_mrl = self.get_mrl(category, step)
            media = self.vlc_instance.media_new(track_mrl)
            self.players[step].set_media(media)

    def get_mrl(self, category, step):
        """Get the media resource locator for a category's step."""
        track_name = CATEGORIES[category][step]
        return 'file://{}{}'.format(self.AUDIO_ROOT, track_name)

    def play_audio(self, step):
        """Play the step's audio track for the current category."""
        # TODO check if player has track for step
        if self.players[step]:
            self.players[step].stop()
            self.players[step].play()

    def play_game_over(self):
        """Play the game over track for the current category."""
        self.play_audio('game_over')
