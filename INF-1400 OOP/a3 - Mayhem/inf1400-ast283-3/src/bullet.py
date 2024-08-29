import pygame
import config as con
from moving_object import Moving_object

class Bullet(Moving_object):
    """ Contains the behaviour for the bullet """
    def __init__(self, size, pos, vel, color, bullet_sprites):
        # Necessary sprite attributes
        self.image = pygame.Surface((size, size), pygame.SRCALPHA)

        # Inherit constrocter and methods from Moving_object class
        super().__init__(size, pos, vel)

        # Other attributes
        self.bullet_sprites = bullet_sprites
        self.image.fill(color)

    def outOfBounds(self):
        """ Removes bullet from sprite group if it moves out of the screen """
        if(self.rect.left > con.SCREEN_X):
            self.bullet_sprites.remove(self)
        if(self.rect.right < 0):
            self.bullet_sprites.remove(self)
        if(self.rect.top > con.SCREEN_Y):
            self.bullet_sprites.remove(self)
        if(self.rect.bottom < 0):
            self.bullet_sprites.remove(self)

