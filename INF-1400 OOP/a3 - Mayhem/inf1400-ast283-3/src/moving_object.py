import pygame
from pygame import Vector2

class Moving_object(pygame.sprite.Sprite):
    """" Parent class for all objects that are supposed to move """
    def __init__(self, size, pos, vel):
        pygame.sprite.Sprite.__init__(self)

        # Necessary sprite attributes
        self.pos = Vector2(pos)
        self.rect = self.image.get_rect(center=self.pos)

        # Other attributes
        self.original_image = self.image
        self.velocity = Vector2(vel)
        self.angle = 0
        self.radius = size/2.5

    def update(self, time_passed):
        """ Initialize rule for Moving object, and moves object by time passed since last time this method was called. """
        self.outOfBounds()

        self.pos += self.velocity * time_passed
        self.rect.center = self.pos

    def outOfBounds(self):
        """ Must be implemented in child class """
        pass

    def rotate(self, angle):
        """ Rotates image and velocity of self with the given angle.
        Code taken from stackoverflow(the post is down, so couldnt find a link) """
        Vector2.rotate_ip(self.velocity, angle)
        self.angle += angle
        if self.angle > 360:
            self.angle -= 360
        elif self.angle < 0:
            self.angle += 360
        self.image = pygame.transform.rotate(self.original_image, -self.angle)
        self.rect = self.image.get_rect(center = self.rect.center)