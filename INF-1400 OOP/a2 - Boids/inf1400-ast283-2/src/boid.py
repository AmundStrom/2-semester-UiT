import pygame
from pygame import Vector2
import config as con
from moving_object import Moving_object

class Boid(Moving_object):
    def __init__(self, size, pos, color, main_sprites, obstacles_sprites):
        super().__init__(size, pos, color, main_sprites, obstacles_sprites)

    def update(self, time_passed):
        # Initialize rules for boid
        ali_rule = self.alignment()
        coh_rule = self.cohesion()
        sep_rule = self.separation()
        col_rule = self.collision()

        old_velocity = self.velocity.copy() # Hold the old velocity

        # Updates velocity, normalizes it, then scale it to the correct length
        self.velocity += ((ali_rule * con.ali_weight) + (coh_rule * con.coh_weight) + 
                          (sep_rule * con.sep_weight) + (col_rule * con.col_weight))
        Vector2.normalize(self.velocity)
        Vector2.scale_to_length(self.velocity, con.top_velocity)

        # calculate the angle between the old and new velocity, so the image rotates correctly
        self.rotate(Vector2(old_velocity).angle_to(Vector2(self.velocity)))
        return super().update(time_passed)
    
    # Boid will travel the same direction as local boids
    def alignment(self):
        local_boids = velocity_x = velocity_y = 0
        # Iterates through boid sprite-group
        for sprite in self.main_sprites:
            # If current sprite is self, do nothing
            if sprite is self:
                continue
            # If a sprite from the group is inside radius of self
            if pygame.sprite.collide_circle(self, sprite):
                local_boids += 1
                velocity_x += sprite.velocity[0]
                velocity_y += sprite.velocity[1]
        # If there has been a collision, calculate average velocity, and return a normalize vector
        if local_boids > 0:
            velocity_x = velocity_x / local_boids
            velocity_y = velocity_y / local_boids
            direction = Vector2.normalize( Vector2(velocity_x, velocity_y) )
            return direction
        # If there has been no collision, return empty vector
        return Vector2(0,0)

    # Boid will travel to the center of local boids
    def cohesion(self):
        local_boids = pos_x = pos_y = 0
        # Iterates through boid sprite-group
        for sprite in self.main_sprites:
            # If current sprite is self, do nothing
            if sprite is self:
                continue
            # If a sprite from the group is inside radius of self
            if pygame.sprite.collide_circle(self, sprite):
                local_boids += 1
                pos_x += sprite.pos[0]
                pos_y += sprite.pos[1]
        # If there has been a collision, calculate average position, and return a normalize vector
        if local_boids > 0:
            pos_x = pos_x / local_boids
            pos_y = pos_y / local_boids
            direction = Vector2.normalize( Vector2(pos_x - self.pos[0], pos_y - self.pos[1]) )
            return direction
        # If there has been no collision, return empty vector
        return Vector2(0,0)