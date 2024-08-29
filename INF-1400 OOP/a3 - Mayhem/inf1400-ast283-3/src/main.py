import pygame
import config as con
from players import Player_one
from players import Player_two
from drawable_objects import Block
from drawable_objects import Spawn
from drawable_objects import Fuel
from explosion import Explosion
from text import Text

class Main():
    """ Initialize other classes and create sprite groups, then run game-loop """
    def __init__(self, screen):

        # Initialize classes that are not sprites
        self.explosion = Explosion()
        self.text = Text()

        # Sprite groups
        self.bullet = self.bullet_Group()
        self.obstacle = self.obstacle_Group()
        self.terrain = self.terrain_Group()
        self.interactable = self.interactable_Group()
        self.player = self.player_Group()

        self.gameloop(screen)

    def bullet_Group(self):
        """ Create bullet group, then return group """
        self.bullet_sprites = pygame.sprite.Group()

        return self.bullet_sprites

    def obstacle_Group(self):
        """ Create and add obstacles to group, then return group """
        self.obstacle_sprites = pygame.sprite.Group()

        for pos in con.pos_list:
            obstacle = Block((pos[0], pos[1]), 0.1)
            self.obstacle_sprites.add(obstacle)

        return self.obstacle_sprites
    
    def terrain_Group(self):
        """ Create and add terrain to group, then return group """
        self.terrain_sprites = pygame.sprite.Group()

        spawn1 = Spawn(con.player_one_pos, 0.2)
        spawn2 = Spawn(con.player_two_pos, 0.2)

        self.terrain_sprites.add(spawn1)
        self.terrain_sprites.add(spawn2)

        return self.terrain_sprites

    def interactable_Group(self):
        """ Create and add interactable to group, then return group """
        self.interactable_sprites = pygame.sprite.Group()

        refuel1 = Fuel((con.SCREEN_X/2, 50), 0.15)
        refuel2 = Fuel((con.SCREEN_X/2, con.SCREEN_Y - 50), 0.15)

        self.interactable_sprites.add(refuel1)
        self.interactable_sprites.add(refuel2)

        return self.interactable_sprites

    def player_Group(self):
        """ Create and add player to group, then return group """
        self.player_sprites = pygame.sprite.Group()

        self.player1 = Player_one(con.player_size, con.player_one_pos, con.player_vel, 
                     self.player_sprites, self.bullet_sprites, self.obstacle_sprites, 
                     self.interactable_sprites, self.explosion)
        self.player2 = Player_two(con.player_size, con.player_two_pos, con.player_vel, 
                     self.player_sprites, self.bullet_sprites, self.obstacle_sprites,
                     self.interactable_sprites, self.explosion)

        self.player_sprites.add(self.player1)
        self.player_sprites.add(self.player2)
        
        return self.player_sprites

    def draw_Text(self, screen):
        """ Draw different texts """
        self.text.draw_score(self.player1.stats()[0], screen, (10,10))
        self.text.draw_fuel(self.player1.stats()[1], screen, (10, 40))
        self.text.draw_score(self.player2.stats()[0], screen, (con.SCREEN_X - 130, 10))
        self.text.draw_fuel(self.player2.stats()[1], screen, (con.SCREEN_X - 130, 40))

    def update_Group(self, time_passed):
        """ Update all the different sprite groups """
        self.bullet.update(time_passed)
        self.obstacle.update()
        self.terrain.update(time_passed)
        self.interactable.update()
        self.player.update(time_passed)

    def draw_Group(self, screen):
        """ Draw all the different sprite groups """
        self.bullet.draw(screen)
        self.obstacle.draw(screen)
        self.terrain.draw(screen)
        self.interactable.draw(screen)
        self.player.draw(screen)

    def gameloop(self, screen):
        """ Draw screen and runs the game """
        background = pygame.image.load("png/vortex.png").convert()
        background = pygame.transform.scale(background,(con.SCREEN_X, con.SCREEN_Y))
        clock = pygame.time.Clock()

        # Game loop
        running = True
        while running:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
            
            # Time since last loop
            time_passed = clock.tick(60) / 1000.0

            # Update all the different sprites
            self.update_Group(time_passed)

            # Fill screen
            screen.blit(background,(0,0))

            # Draw all the different sprites
            self.draw_Group(screen)

            # Player crash animation
            self.explosion.animation(screen, time_passed)

            # Draw different texts
            self.draw_Text(screen)

            pygame.display.update()