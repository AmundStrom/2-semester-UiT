from pygame import Vector2
import pygame
import math
import random as rnd

SCREEN_X = 1065
SCREEN_Y = 620

pygame.init()
screen = pygame.display.set_mode((SCREEN_X, SCREEN_Y), 0, 32)
background = pygame.Surface((SCREEN_X, SCREEN_Y))

# PNG's for enemy blocks
BLOCK_IDLE_START = "breakout\idle\idle_trans_start.png"
block_idle_start = pygame.image.load(BLOCK_IDLE_START).convert_alpha()
BLOCK_IDLE_END = "breakout\idle\idle_trans_end.png"
block_idle_end = pygame.image.load(BLOCK_IDLE_END).convert_alpha()

BLOCK_LAUGH_START = "breakout\laugh\laugh_trans_start.png"
block_laugh_start = pygame.image.load(BLOCK_LAUGH_START).convert_alpha()
BLOCK_LAUGH_END = "breakout\laugh\laugh_trans_end.png"
block_laugh_end = pygame.image.load(BLOCK_LAUGH_END).convert_alpha()

# Pre-code
def intersect_rectangle_circle(rec_pos, sx, sy,
                               circle_pos, circle_radius, circle_speed):
    """ Parameters:
    rec_pos     - A Vector2 representing the position of the rectangles upper,
                  left corner.
    sx          - Width of rectangle.
    sy          - Height of rectangle.
    circle_pos  - A Vector2 representing the circle's position.
    circle_radius - The circle's radius.
    circle_speed - A Vector2 representing the circles speed.
    """

    # Position of the walls relative to the ball
    top = (rec_pos.y) - circle_pos.y
    bottom = (rec_pos.y + sy) - circle_pos.y
    left = (rec_pos.x) - circle_pos.x
    right = (rec_pos.x + sx) - circle_pos.x

    r = circle_radius
    intersecting = left <= r and top <= r and right >= -r and bottom >= -r

    if intersecting:
        # Now need to figure out the vector to return.
        impulse = circle_speed.normalize()

        if abs(left) <= r and impulse.x > 0:
            impulse.x = -impulse.x
        if abs(right) <= r and impulse.x < 0:
            impulse.x = -impulse.x
        if abs(top) <= r and impulse.y > 0:
            impulse.y = -impulse.y
        if abs(bottom) <= r and impulse.y < 0:
            impulse.y = -impulse.y
        return impulse.normalize()
    return None

# Pre-code
def intersect_circles(circle_a_position, circle_a_radius,
                      circle_b_position, circle_b_radius):
    """ Parameters:
    circle_a_position       - A Vector2D representing circle A's position
    circle_a_radius    - Circle A's radius
    circle_b_position       - A Vector2D representing circle B's position
    circle_b_radius    - Circle B's radius
    """
    # vector from A to B
    dp1p2 = circle_b_position - circle_a_position

    if circle_a_radius + circle_b_radius >= pygame.math.Vector2.length(dp1p2):
        return dp1p2.normalize()
    else:
        return None

# Class for printing text on the screen
class text:
    def __init__(self):
        self.myfont = pygame.font.SysFont('Arial', 30)
        self.textspace = self.myfont.render('Press Space to Start', False, (255, 255, 255))
        self.textcontrols = self.myfont.render('Move with Arrow Keys', False, (255, 255, 255))
        self.task = self.myfont.render('Your Task is to Destroy All Blocks on the Screen', False, (255, 255, 255))

        self.defeat = self.myfont.render('You Have Lost, Press Space to Restart', False, (255, 255, 255))
        self.victory = self.myfont.render('You Have Won! Press Space to Restart', False, (255, 255, 255))

        self.score = 0
        self.msg = 'Score: ' + str(self.score)
        self.font = pygame.font.SysFont('Arial', 30)
        self.img = self.font.render(self.msg, True, (255, 255, 255))

        self.rect = self.img.get_rect()
        self.rect.topleft = (10, 10)

    def gamescore(self):
        self.msg = 'Score: ' + str(self.score)
        self.img = self.font.render(self.msg, True, (255, 255, 255))
        screen.blit(self.img, self.rect)

# Class for the platform the user will control
class player:
    def __init__(self):
        self.w = 200
        self.h = 30
        self.y = 540
        self.x = (SCREEN_X/2) - (self.w/2)
        self.speed = 6
        self.radius = 180
        self.r_color = (255, 0, 255)
        self.c_color = (255, 255, 255)

    # Methond for moving the platform
    def move(self):
        keys=pygame.key.get_pressed()
        # If the left or right arrow key is held down, move the platoform accordingly
        if keys[pygame.K_LEFT]:
            self.x -= self.speed
            if self.x <= 0:
                self.x = 0
        if keys[pygame.K_RIGHT]:   
            self.x += self.speed
            if self.x >= SCREEN_X - self.w:
                self.x = SCREEN_X - self.w

        # Vectors for the player platform, r_pos is the platform, 
        # c_pos is a circle that follows the platfrom, we will use this to determinate the angle of the ball to bounce
        self.r_pos = Vector2(self.x, self.y)
        self.c_pos = Vector2(self.x + (self.w/2), self.y + 50)

    # Method for restarting the game, we set all the values to the original
    def respawn(self):
        self.y = 540
        self.x = (SCREEN_X/2) - (self.w/2)

    def draw(self):
        pygame.draw.rect(screen, self.r_color, (self.x, self.y, self.w, self.h))

# Class for a ball the player will bounce around
class ball:
    def __init__(self):
        self.x = (SCREEN_X/2)
        self.y = 300
        self.radius = 13
        self.color  = (255, 180, 150)
        self.spawn_timer = 180
        # Vectors we use to deteminate the angles of the ball to bounce
        self.speed = Vector2(0,300)
        self.pos = Vector2(self.x, self.y)
        # We use this when the ball thouches the buttom of the screen
        self.restart = False

    def move(self, time_passed):
        # wait some time before starting to move when the game launches
        if(self.spawn_timer > 0):
            self.spawn_timer -= 1
        else:
            # change position of the ball for every clock tick, this creates the illusion of the ball moving
            self.x += self.speed[0] * time_passed
            self.y += self.speed[1] * time_passed
            self.pos = Vector2(self.x, self.y)

            # if the ball hit a screen border, change direction 
            if self.x - self.radius <= 0 or self.x + self.radius >= SCREEN_X:
                self.speed[0] = (self.speed[0]) * (-1)
            if self.y - self.radius <= 0:
                self.speed[1] = (self.speed[1]) * (-1)
            # If the ball hits the bottom of the screen, set restart to True
            if self.y + self.radius >= SCREEN_Y:
                self.restart = True

    # Method for restarting the game, we set all the values to the original
    def respawn(self):
        self.x = (SCREEN_X/2)
        self.y = 300
        self.spawn_timer = 180
        self.speed = Vector2(0,300)
        self.pos = Vector2(self.x, self.y)
        self.restart = False

    def draw(self):
        # before the ball starts moving, we make it blink
        if self.spawn_timer % 60 < 30:
            pygame.draw.circle(screen, self.color, (round(self.x), round(self.y)), self.radius)

# class for the enemy blocks
class blocks:
    def __init__(self, x_pos, y_pos):
        self.x = 10 + x_pos
        self.y = 60 + y_pos
        self.h = 30
        self.w = 100
        self.color = ((255 - y_pos), (50), (50 + (y_pos)))
        self.pos = Vector2(self.x, self.y)

        self.img = block_idle_start
        self.random_block = rnd.random() * 10
        self.random_expression = rnd.random() * 100
        self.counter = 0
    
    # Method for drawing the enemy blocks, and changing expressions
    def draw(self):
        # draw rectangle
        pygame.draw.rect(screen, self.color, (self.x, self.y, self.w, self.h))
        # checking if a block will have expression or not
        if(self.random_block > 6):
            self.counter += 1
            # block will start laughing
            if(self.random_expression > 90):
                if(self.counter == 60):
                    self.img = block_laugh_start
                if(self.counter == 120):
                    self.img = block_laugh_end
                    self.random_expression = rnd.random() * 100
                    self.counter = 0
            else:
                # idle expression
                if(self.counter == 60):
                    self.img = block_idle_end
                if(self.counter == 120):
                    self.img = block_idle_start
                    self.random_expression = rnd.random() * 100
                    self.counter = 0
            # draw expression image
            screen.blit(self.img, (self.x, self.y))

def create_blocks():
    x_pos = 0
    y_pos = 0
    blockList = []
    # We use a for-loop to add all the blocks for the game in a list
    # And change the X and Y coordinates accordingly
    # To change how many blocks there will be in the game, edit x inside range(x) to whatever you want
    for i in range(48):
        blockSingle = blocks(x_pos, y_pos)
        blockList.append(blockSingle)
        x_pos += (blockSingle.w + 5)
        # This row of blocks is 10
        if blockSingle.x == (SCREEN_X - blockSingle.w - 10):
            x_pos = (blockSingle.w/2 + 5)
            y_pos += (blockSingle.h + 5)
        # This row of blocks is 9
        if blockSingle.x == (SCREEN_X - blockSingle.w - 10 - (blockSingle.w/2)):
            x_pos = 0
            y_pos += (blockSingle.h + 5)
    return blockList

# give values to variables we will use in game loop
gameblocks = create_blocks()
gametext = text()
playerOne = player()
ballOne = ball()
clock = pygame.time.Clock()
text_screen = True

pygame.display.flip()
# game loop
while True:
    events = pygame.event.get()
    for event in events:
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()

    key_press = pygame.key.get_pressed()
    # start screen, when space is pressed, exit if-statement
    if text_screen:
        if key_press[pygame.K_SPACE]:
            text_screen = False
        screen.blit(gametext.task, (270, 200))
        screen.blit(gametext.textcontrols, (395,250))
        screen.blit(gametext.textspace, (400,300))
        pygame.display.update()   
    else:

        time_passed = clock.tick(60) / 1000.0

        # Initiate move functions
        # Checking if the ball has hit the bottom of the screen, or there are no more enemy blocks
        if not (ballOne.restart):
            if gameblocks:
                playerOne.move()
                ballOne.move(time_passed)

        # If the Player ball collide with the Player platform
        impulse = intersect_rectangle_circle(playerOne.r_pos, playerOne.w, playerOne.h,
                                                ballOne.pos, ballOne.radius, ballOne.speed)
        if impulse:
            # Multiply the vector-result of the fucntion with the speed the ball
            # We use pythagoras to find the speed, then multiply it with -1 to change the direction
            ballOne.speed = (intersect_circles(ballOne.pos, ballOne.radius, playerOne.c_pos, playerOne.radius) 
                            * math.sqrt((ballOne.speed[0]**2) + (ballOne.speed[1]**2))) * (-1)  

        # We use a for-loop to iterate through the Block list
        for obj in gameblocks:
            blockSingle = obj
            # Than we check for every block if it collides with the player ball
            impulse = intersect_rectangle_circle(blockSingle.pos, blockSingle.w, blockSingle.h,
                                                ballOne.pos, ballOne.radius, ballOne.speed)
            if impulse:
                # We use the same logic as the code above
                ballOne.speed = (intersect_rectangle_circle(blockSingle.pos, blockSingle.w, blockSingle.h,
                                                            ballOne.pos, ballOne.radius, ballOne.speed)
                                                            * math.sqrt((ballOne.speed[0]**2) + (ballOne.speed[1]**2)))
                # Remove the block form the list, and increase the score
                gameblocks.remove(blockSingle)
                gametext.score += 10

        # draw black background and score in top left
        screen.blit(background, (0, 0))
        gametext.gamescore()

        # if ball hits bottom of the screen, set ballOne.restart to True, this will stop the game
        # if space is pressed, resart game
        if ballOne.restart:
            screen.blit(gametext.defeat, (350,250))
            if key_press[pygame.K_SPACE]:
                ballOne.respawn()
                playerOne.respawn()
                gameblocks = create_blocks()
                gametext.score = 0
        
        # if gameblocks list is empty, the game will stop, and the game is won
        # press space to restart game
        if not gameblocks:
            screen.blit(gametext.victory, (350,250))
            if key_press[pygame.K_SPACE]:
                ballOne.respawn()
                playerOne.respawn()
                gameblocks = create_blocks()
                gametext.score = 0

        # Initiate the draw fucntions
        for obj in gameblocks:
            obj.draw()
        playerOne.draw() 
        ballOne.draw()

        pygame.display.update()