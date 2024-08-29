import pygame

class Drawable_object(pygame.sprite.Sprite):
    """ Parent class for all objects that are supposed to be drawn and stay still """
    def __init__(self, image, pos, size):
        pygame.sprite.Sprite.__init__(self)

        # Necessary sprite attributes
        self.image = image
        self.image = pygame.transform.scale(self.image, (self.image.get_width() * size, self.image.get_height() * size) )
        self.pos = pos
        self.rect = self.image.get_rect(center=self.pos)


class Block(Drawable_object):
    """ Obstacle for the player to crash into """
    def __init__(self, pos, size):

        # Necessary sprite attributes
        image = pygame.image.load("png/block.png").convert()

        super().__init__(image, pos, size)


class Spawn(Drawable_object):
    """ Image where the players spawns """
    def __init__(self, pos, size):

        # Necessary sprite attributes
        image = pygame.image.load("png/Spawn.png").convert_alpha()

        super().__init__(image, pos, size)

        # Other attributes
        self.original_image = self.image
        self.angle = 0

    def update(self, time_passed):
        """ Rotate image with time passed since last time this was called """
        self.angle += (time_passed * 50)
        if self.angle > 360:
            self.angle = 0
        self.image = pygame.transform.rotate(self.original_image, -self.angle)
        self.rect = self.image.get_rect(center = self.rect.center)
        

class Fuel(Drawable_object):
    """ Obstacle that refules player """
    def __init__(self, pos, size):

        # Necessary sprite attributes
        image = pygame.image.load("png/Gas.png").convert_alpha()

        super().__init__(image,pos,size)

        # Other attributes
        self.radius = size*50