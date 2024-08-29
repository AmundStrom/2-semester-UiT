# python -m pygame.docs
import pygame
import config as con
from main import Main

if __name__ == '__main__':
    # Initialize pygame
    pygame.init()
    pygame.display.set_caption("Mayhem")
    screen = pygame.display.set_mode((con.SCREEN_X, con.SCREEN_Y), 0, 32)

    # Run game
    Main(screen)