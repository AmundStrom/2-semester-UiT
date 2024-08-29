# python -m pygame.docs
import pygame
import random as rnd
import config as con
from boid import Boid # Import Boid class and behaviour
from hoik import Hoik # Import Hoik class and behaviour
from obstacles import Rect # Import Rect class and behaviour
from obstacles import Circle # Import Circle class and behaviour

# Create group of obstacles sprites
obstacles_sprites = pygame.sprite.Group()

# Create group of hoik sprites
hoik_sprites = pygame.sprite.Group()

# Create group of boid sprites
boid_sprites = pygame.sprite.Group()
for _ in range(con.amount_of_boids):
    boid = Boid(con.size_of_boid, (rnd.randint(150, 915), rnd.randint(150, 470)), con.RED, boid_sprites, obstacles_sprites)
    boid_sprites.add(boid)

# Initialize pygame
pygame.init()
screen = pygame.display.set_mode((con.SCREEN_X, con.SCREEN_Y), 0, 32)

# Game loop
clock = pygame.time.Clock()
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        # If corresponding key is pressed, add new sprite on screen
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_r:
                rect = Rect()
                obstacles_sprites.add(rect)
            if event.key == pygame.K_c:
                circle = Circle()
                obstacles_sprites.add(circle)
            if event.key == pygame.K_h:
                hoik = Hoik(con.size_of_hoik, pygame.mouse.get_pos(), con.GREEN, hoik_sprites, obstacles_sprites, boid_sprites)
                hoik_sprites.add(hoik)
            if event.key == pygame.K_b:
                boid = Boid(con.size_of_boid, pygame.mouse.get_pos(), con.RED, boid_sprites, obstacles_sprites)
                boid_sprites.add(boid)
     
    time_passed = clock.tick(60) / 1000.0

    # Update all the different sprites
    boid_sprites.update(time_passed)
    hoik_sprites.update(time_passed)
    obstacles_sprites.update()

    # Fill entire screen
    screen.fill(con.BLACK)

    # Draw all the different sprites
    boid_sprites.draw(screen)
    hoik_sprites.draw(screen)
    obstacles_sprites.draw(screen)
    pygame.display.update()