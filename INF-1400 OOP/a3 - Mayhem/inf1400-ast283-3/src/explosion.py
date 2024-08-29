import pygame
import config as con
""" SOME CODE IS TAKEN FROM: https://www.youtube.com/watch?v=M6e3_8LHc7A 
                             https://www.youtube.com/watch?v=nXOVcOBqFwM"""

class Explosion():
    """ Explosion animation where the player crashes """
    def __init__(self):
        # Sheet with all explosion images
        self.image = pygame.image.load("png/explosions/explosion_01_strip13.png").convert_alpha()

        # Animation loop
        self.last_update = 0
        self.current_time = 0
        self.frame = 0
        self.cooldown = 0.03
        self.list = self.animation_list()

        # Sent from player_rules
        self.pos = (0,0)
        self.sec_pos = (0,0)
        self.crash = False

    def get_image(self, frame, width, height, scale, alpha):
        """ Gets the different images from the sheet """
        image = pygame.Surface((width,height)).convert()
        image.set_alpha(alpha)
        image.blit(self.image, (0,0), ((frame * width), 0, width, height))
        image = pygame.transform.scale(image, (width * scale,height * scale))
        image.set_colorkey(con.BLACK)

        return image

    def animation_list(self):
        """ Adds all the different images to a list """
        list = []
        for frame in range(13):
            list.append(self.get_image(frame, (196), 190, 0.5, (200 - frame**2)))

        return list

    def animation(self, screen, time_passed):
        """ Iterates through the animation list and draws the image, creating the animation """
        if self.crash == True:
            self.current_time += time_passed
            if self.current_time - self.last_update >= self.cooldown:
                self.frame += 1
                self.last_update = self.current_time
                if self.frame == 13:
                    self.frame = 0
                    self.sec_pos = (0,0)
                    self.crash = False

            screen.blit(self.list[self.frame],(self.pos[0] - 196/4, self.pos[1] - 190/4))

            if self.sec_pos != (0,0):
                screen.blit(self.list[self.frame],(self.sec_pos[0] - 196/4, self.sec_pos[1] - 190/4))