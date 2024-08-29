import pygame
import config as con
from pygame import Vector2
from moving_object import Moving_object
from bullet import Bullet
from drawable_objects import Fuel

class Player_rules(Moving_object):
    """ Contains the behaviour and rules for the player """
    def __init__(self, size, pos, vel, player_sprites, bullet_sprites, obstacles_sprites, interactable_sprites, exp):

        # Necessary sprite attributes
        self.image = pygame.image.load("png/ship.png").convert_alpha()
        self.image = pygame.transform.scale(self.image, (size, size))

        # Inherit constrocter and methods from Moving_object class
        super().__init__(size, pos, vel)

        # Other attributes for player
        self.spawn_point = Vector2(pos)
        self.accelerate = Vector2(con.player_accelerate, 0)
        self.fire_counter = 0
        self.score = 0
            # Fuel attributes
        self.fuel = 100
        self.refuel_counter = 5
        self.refuel_pos = []

        # Sprite groups
        self.player_sprites = player_sprites
        self.bullet_sprites = bullet_sprites
        self.obstacles_sprites = obstacles_sprites
        self.interactable_sprites = interactable_sprites

        # Explosion class
        self.exp = exp

    def update(self, time_passed):
        """ Initialize rules for player """
        self.controls(time_passed)
        self.collision()
        self.refuel(time_passed)
        self.gravity()
            
        return super().update(time_passed)

    def controls(self, time_passed):
        """ Behaviours for different controles """
        key = pygame.key.get_pressed()
        if key[self.left]:
            self.rotate(-con.player_rotate)
            Vector2.rotate_ip(self.accelerate, -con.player_rotate)
        if key[self.right]:
            self.rotate(con.player_rotate)
            Vector2.rotate_ip(self.accelerate, con.player_rotate)

        if key[self.forward]:
            if self.fuel >= con.player_fueldrain:
                self.fuel -= con.player_fueldrain
                if Vector2.length(self.velocity) < con.player_topspeed:
                    self.velocity += self.accelerate
            else:
                if self.velocity != Vector2(0,0):
                    self.velocity -= self.accelerate
        else:
            if self.velocity != Vector2(0,0):
                self.velocity -= self.accelerate

        self.fire_counter += time_passed
        if key[self.fire]:
            if self.fire_counter > con.player_firerate:
                self.shoot()
                self.fire_counter = 0

    def shoot(self):
        """ Create bullet object and adds it to bullet group """
        bullet = Bullet(con.bullet_size, (self.pos + (self.accelerate * 2.5)), (con.bullet_vel,0), con.YELLOW, self.bullet_sprites)
        bullet.rotate(self.angle)
        self.bullet_sprites.add(bullet)

    def collision(self):
        """ Checks for all the differnt collisions and reacts accordingly """

        # Bullet and obstacle
        pygame.sprite.groupcollide(self.bullet_sprites, self.obstacles_sprites, True, False)

        # Player and interactable
        for inter in self.interactable_sprites:
            if pygame.sprite.collide_circle(self, inter):
                self.fuel = con.player_fuel
                self.interactable_sprites.remove(inter)
                self.refuel_pos.append(inter.pos)

        # Player and obstacle
        for obstacle in self.obstacles_sprites:
            if pygame.sprite.collide_rect(self, obstacle):
                # Explosion attributes
                self.exp.crash = True
                self.exp.pos = self.pos
                # Player attributes
                self.fuel = con.player_fuel
                self.score -= 5
                self.velocity = Vector2(0,0)
                self.pos = Vector2.copy(self.spawn_point)

        # Player and player
        for player in self.player_sprites:
            if player is self:
                continue
            if pygame.sprite.collide_circle(self, player):
                # Explosion attributes
                self.exp.crash = True
                self.exp.pos = self.pos
                self.exp.sec_pos = player.pos
                # Player attributes
                self.fuel = con.player_fuel
                self.score -= 2
                self.velocity = Vector2(0,0)
                self.pos = Vector2.copy(self.spawn_point)
                player.fuel = con.player_fuel
                player.score -= 2
                player.velocity = Vector2(0,0)
                player.pos = Vector2.copy(player.spawn_point)

        # Player and bullet
        for bullet in self.bullet_sprites:
            if pygame.sprite.collide_circle(self, bullet):
                # Explosion attributes
                self.exp.crash = True
                self.exp.pos = self.pos
                # Player attributes
                for player in self.player_sprites:
                    if player is self:
                        continue
                    player.score += 7
                self.fuel = con.player_fuel
                self.score -= 3
                self.velocity = Vector2(0,0)
                self.pos = Vector2.copy(self.spawn_point)
                self.bullet_sprites.remove(bullet)

    def refuel(self, time_passed):
        """ Counts down for fuel to respawn at the same position """
        if len(self.refuel_pos) > 0:
            self.refuel_counter -= time_passed
            if self.refuel_counter < 0:
                pos = self.refuel_pos[0]
                fuel = Fuel(pos, 0.15)
                self.interactable_sprites.add(fuel)
                self.refuel_pos.pop(0)
                self.refuel_counter = con.player_fuel_respawn 
    
    def gravity(self):
        """ Moves the player towards the center """
        direction = Vector2(self.pos[0] - con.CENTER[0], self.pos[1] - con.CENTER[1])
        if direction != Vector2(0,0):
            direction = Vector2.normalize(direction)
        self.pos -= direction * con.player_gravity_weight

    def outOfBounds(self):
        """ Moves the player to the other side of the screen """
        if(self.rect.left > con.SCREEN_X):
            self.pos[0] = 0
        if(self.rect.right < 0):
            self.pos[0] = con.SCREEN_X
        if(self.rect.top > con.SCREEN_Y):
            self.pos[1] = 0
        if(self.rect.bottom < 0):
            self.pos[1] = con.SCREEN_Y

    def stats(self):
        """ Return a tuple of score and fuel """
        return (self.score, self.fuel)