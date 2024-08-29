import pygame
from pygame import Vector2
from player_rules import Player_rules

class Player_one(Player_rules):
    """ Player one class, with unique controls """
    def __init__(self, size, pos, vel, player_sprites, bullet_sprites, obstacles_sprites, interactable_sprites, exp):
        super().__init__(size, pos, vel, player_sprites, bullet_sprites, obstacles_sprites, interactable_sprites, exp)

        # Rotates player
        self.rotate(0)
        Vector2.rotate_ip(self.accelerate, 0)

        # Define controls
        self.left = pygame.K_a
        self.right = pygame.K_d
        self.forward = pygame.K_w
        self.fire = pygame.K_LCTRL


class Player_two(Player_rules):
    """ Player two class, with unique controls """
    def __init__(self, size, pos, vel, player_sprites, bullet_sprites, obstacles_sprites, interactable_sprites, exp):
        super().__init__(size, pos, vel, player_sprites, bullet_sprites, obstacles_sprites, interactable_sprites, exp)

        # Rotates player
        self.rotate(180)
        Vector2.rotate_ip(self.accelerate, 180)

        # Define controls
        self.left = pygame.K_LEFT
        self.right = pygame.K_RIGHT
        self.forward = pygame.K_UP
        self.fire = pygame.K_RCTRL