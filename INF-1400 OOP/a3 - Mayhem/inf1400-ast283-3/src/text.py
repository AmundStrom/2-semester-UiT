import pygame

class Text():
    """ Draw different texts on screen """
    def __init__(self):
        self.font = pygame.font.SysFont('Calibri', 30)

    def draw_score(self, text, screen, pos):
        """ Draw player score on screen """
        self.text = 'Score: ' + str(text)
        self.draw(screen, pos)

    def draw_fuel(self, text, screen, pos):
        """ Draw player fuel on screen """
        self.text = 'Fuel: ' + str(text)
        self.draw(screen, pos)

    def draw(self, screen, pos):
        """ Draw given text on screen """
        self.image = self.font.render(self.text, True, (255, 255, 255))
        self.rect = self.image.get_rect()
        self.rect.topleft = (pos)
        screen.blit(self.image, self.rect)