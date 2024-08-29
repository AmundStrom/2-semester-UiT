import pygame
from pygame import Vector2
import config as con
from moving_object import Moving_object

class Hoik(Moving_object):
    def __init__(self, size, pos, color, main_sprites, obstacles_sprites, prey_sprites):
        super().__init__(size, pos, color, main_sprites, obstacles_sprites)
        self.prey_sprites = prey_sprites

    def update(self, time_passed):
        # Initialize rules for hoik
        sep_rule = self.separation()
        col_rule = self.collision()
        cha_rule = self.chase()
        self.eat()

        old_velocity = self.velocity.copy() # Hold the old velocity

        # Updates velocity, normalizes it, then scale it to the correct length
        self.velocity += ((sep_rule * con.sep_weight) + (col_rule * con.col_weight) + (cha_rule * con.cha_weight))
        Vector2.normalize(self.velocity)
        Vector2.scale_to_length(self.velocity, con.top_velocity)

        # calculate the angle between the old and new velocity, so the image rotates correctly
        self.rotate(Vector2(old_velocity).angle_to(Vector2(self.velocity)))
        return super().update(time_passed)
    
    # Hoik will chase the nearest prey
    def chase(self):
        local_boids = 0
        pos_x = con.SCREEN_X
        pos_y = con.SCREEN_Y
        # Iterates through boid sprite-group
        for sprite in self.prey_sprites:
            # If a sprite from the group is inside radius of self, save nearest sprite
            if pygame.sprite.collide_circle(self, sprite):
                local_boids = 1
                if (abs((sprite.pos[0] + sprite.pos[1]) - (self.pos[0] + self.pos[1])) < 
                    abs((pos_x         + pos_y        ) - (self.pos[0] + self.pos[1]))):
                    pos_x = sprite.pos[0]
                    pos_y = sprite.pos[1]
        # If there has been a collision, calculate direction to nearest sprite, and return a normalize vector
        if local_boids > 0:
            direction = Vector2.normalize( Vector2(pos_x - self.pos[0], pos_y - self.pos[1]) )
            return direction
        # If there has been no collision, return empty vector
        return Vector2(0,0)

    # Hoik will eat prey if they collide
    def eat(self):
        pygame.sprite.groupcollide(self.main_sprites, self.prey_sprites, False, True)