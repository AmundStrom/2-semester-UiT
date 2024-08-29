import pygame, math
import random as rnd
from pygame import Vector2
import config as con

class Moving_object(pygame.sprite.Sprite):
    def __init__(self, size, pos, color, main_sprites, obstacles_sprites):
        pygame.sprite.Sprite.__init__(self)
        # Necessary sprite attributes
        self.image = pygame.Surface((size + 5, size), pygame.SRCALPHA)
        self.original_image = self.image
        self.pos = Vector2(pos)
        self.rect = self.image.get_rect(center=self.pos)
        # Will give random velocity in x and y direction, but will always be the same "top velocity"
        self.velocity_x = rnd.randint(1,(con.top_velocity - 1))
        self.velocity_y = math.sqrt(math.pow(con.top_velocity,2) - math.pow(self.velocity_x, 2))
        self.velocity = Vector2(rnd.choice([-1,1]) * self.velocity_x, rnd.choice([-1,1]) * self.velocity_y)
        # Other attributes
        self.angle = 0
        self.radius = con.flock_radius
        # Sprite groups
        self.main_sprites = main_sprites
        self.obstacles_sprites = obstacles_sprites
        # Drawing of object
        pygame.draw.polygon(self.image, color, ((0, 0), (0, size), (size + 5, size / 2)))
        self.image = pygame.transform.rotate(self.original_image, Vector2(self.velocity).angle_to(Vector2(1,0)))
        self.original_image = self.image

    def update(self, time_passed):
        self.pos += self.velocity * time_passed
        self.rect.center = self.pos
        # Will move object to the other side if it moves out of the screen
        if(self.rect.left > con.SCREEN_X):
            self.pos[0] = 0
        if(self.rect.right < 0):
            self.pos[0] = con.SCREEN_X
        if(self.rect.top > con.SCREEN_Y):
            self.pos[1] = 0
        if(self.rect.bottom < 0):
            self.pos[1] = con.SCREEN_Y

    # Rotate image of object with the given rotation angle, taken from stackoverflow(the post is down, so couldnt find a link)
    def rotate(self, rotation):
        self.angle += rotation
        if self.angle > 360:
            self.angle -= 360
        elif self.angle < 0:
            self.angle += 360
        self.image = pygame.transform.rotate(self.original_image, -self.angle)
        self.rect = self.image.get_rect(center = self.rect.center)
    
    # Moving objects of same sprite group will not crash into eachother
    def separation(self):
        local_boids = pos_x = pos_y = 0
        # Iterates through its own sprite-group 
        for sprite in self.main_sprites: 
            # If current sprite is self, do nothing
            if sprite is self:
                continue
            # If a sprite from the group is inside the scaled up rectangle of self
            if pygame.sprite.collide_rect_ratio(1.5)(self, sprite):
                local_boids += 1
                pos_x += sprite.pos[0] - self.pos[0]
                pos_y += sprite.pos[1] - self.pos[1]
        # If there has been a collision, calculate average position, and return a normalize vector
        if local_boids > 0:
            pos_x = pos_x / local_boids
            pos_y = pos_y / local_boids
            direction = Vector2.normalize( -1 * Vector2(pos_x, pos_y) )
            return direction
        # If there has been no collision, return empty vector
        return Vector2(0,0)

    # Moving object will not crash into Drawable objects
    def collision(self):
        local_boids = pos_x = pos_y = 0
        # Iterates through obstacles sprite-group
        for sprite in self.obstacles_sprites:
            # If a sprite from the group is inside the scaled up rectangle of self
            if pygame.sprite.collide_rect_ratio(2.5)(self, sprite):
                local_boids += 1
                pos_x += sprite.rect.x - self.pos[0]
                pos_y += sprite.rect.y - self.pos[1]
        # If there has been a collision, calculate average position, and return a normalize vector
        if local_boids > 0:
            pos_x = pos_x / local_boids
            pos_y = pos_y / local_boids
            direction = Vector2.normalize( -1 * Vector2(pos_x, pos_y) )
            return direction
        # If there has been no collision, return empty vector
        return Vector2(0,0)