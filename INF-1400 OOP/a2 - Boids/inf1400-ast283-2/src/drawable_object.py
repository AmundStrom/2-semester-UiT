import pygame
import config as con

class Drawable_object(pygame.sprite.Sprite):
    def __init__(self):
        pygame.sprite.Sprite.__init__(self)
        self.image = pygame.Surface((con.size_of_obs, con.size_of_obs), pygame.SRCALPHA)
        self.rect = self.image.get_rect()
        self.rect.center = pygame.mouse.get_pos()


