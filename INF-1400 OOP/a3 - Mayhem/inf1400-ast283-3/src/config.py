SCREEN_X = 1065
SCREEN_Y = 620
CENTER = (SCREEN_X/2, SCREEN_Y/2)
BLACK = (0,0,0)
YELLOW = (255,255,0)

#DRAWABLE OBJECT
    # Positions for the obstacles
pos_list = [(250,150), (SCREEN_X/2 - 100, SCREEN_Y/2), (250, SCREEN_Y - 150),
            (SCREEN_X - 250, 150), (SCREEN_X/2 + 100, SCREEN_Y/2), (SCREEN_X - 250, SCREEN_Y - 150)]


#PLAYER OBJECT
player_topspeed = 500
player_accelerate = 6
player_rotate = 4 # Speed of rotation
player_vel = (0,0) # Start speed

player_size = 30
player_firerate = 0.3
player_gravity_weight = 0.5

player_fuel = 100
player_fueldrain = 0.25
player_fuel_respawn = 5

player_one_pos = (100, SCREEN_Y/2)

player_two_pos = (SCREEN_X - 100, SCREEN_Y/2)

#BULLET OBJECT
bullet_size = 4
bullet_vel = 600
