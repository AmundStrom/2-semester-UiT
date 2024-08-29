import pygame
import config as con
from drawable_object import Drawable_object

class Rect(Drawable_object):
    def __init__(self):
        super().__init__()
        self.image.fill(con.BLUE)

class Circle(Drawable_object):
    def __init__(self):
        super().__init__()
        pygame.draw.circle(self.image, con.BLUE, (con.size_of_obs / 2, con.size_of_obs / 2), con.size_of_obs/2)