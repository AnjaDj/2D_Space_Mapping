import pygame
import math
from mapping_simulation import sensor

"""
Loaded reference map 'map.png' to the external_map, with its dimension set statically.
"""
external_map = pygame.image.load('mapping_simulation/map.png')
map_width, map_height = (1200, 900)
pygame.display.set_caption('2D-mapping simulation')

"""
White surface created, and blit (add) real map with black coloured obstacles onto it.
After that copy it to the original_map variable and pass it to sensor.
"""
surface = pygame.display.set_mode((map_width, map_height))
surface.blit(external_map, (0, 0))
original_map = surface.copy()
sensor = sensor.Sensor(100, original_map)

"""
Black map created, for obstacles to be shown onto it later. Obstacles will be in red.
"""
surface.fill((0, 0, 0))
obstacles_map = surface.copy()  # obstacles_map is painted in black and obstacles will be added to it

# make empty list to store coordinates of detected obstacles
point_cloud = []
# empty list to store all coordinates of sensor
sensor_positions_cloud = []


def distance_angle_to_position(distance, angle, sensor_position):
    """

    :param distance: stores distance of obstacle from sensor
    :param angle: stores angle of obstacle from sensor
    :param sensor_position: position of sensor in the moment of sensing the obstacle in (x, y) format
    :return: tuple of coordinates of sensed obstacle

    Function that transforms sensor data from (distance, angle) format to (x, y) format
    """
    # based on current position of sensor
    x = distance * math.cos(angle) + sensor_position[0]
    y = -distance * math.sin(angle) + sensor_position[1]
    return (int(x), int(y))


def store_data_in_point_cloud(data):
    """
    :param data: list of sensor data to be processed

    Function that stores sensor data in to the list called point_cloud,
    each entry in data is (distance, angle, (sen_pos_x, sen_pos_y)),
    so it needs to be translated into coordinates format and then stored into point_cloud
    """
    if data:
        for element in data:
            point = distance_angle_to_position(element[0], element[1], element[2])
            sen_pos = element[2]
            if point not in point_cloud:
                point_cloud.append(point)
            if sen_pos not in sensor_positions_cloud:
                sensor_positions_cloud.append(sen_pos)


def show_sensor_data():
    """"
    Function that prints out sensordata on the black map,
    red pixels for obstacles,
    green round circles for sensor position
    """
    for point in point_cloud:
        obstacles_map.set_at((int(point[0]), int(point[1])), (255, 0, 0))

    for sen_pos in sensor_positions_cloud:
        pygame.draw.circle(obstacles_map, (0, 255, 0), (int(sen_pos[0]), int(sen_pos[1])), 3)
        obstacles_map.set_at((int(sen_pos[0]), int(sen_pos[1])), (0, 255, 0))


"""
Standard pygame code, using running variable, and while loop that ends when 'red x' icon is pressed in the 
pygame window. For the purpose of simulation mouse cursor will play the role of sensor. For that we use 
pygame.mouse.get_pos() function
In each iteration of the loop new data is collected from the sensor, and should be added to the obstacle_map.
"""
running = True
pygame.init()

while running:
    sensorOn = True
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if pygame.mouse.get_focused():
            sensorOn = True
        elif not pygame.mouse.get_focused():
            sensorOn = True

    if sensorOn:
        sensor.position = pygame.mouse.get_pos()
        sensor_data = sensor.get_sensor_data()
        store_data_in_point_cloud(sensor_data)
        show_sensor_data()

    surface.blit(obstacles_map, (0, 0))
    pygame.display.update()

pygame.quit()
