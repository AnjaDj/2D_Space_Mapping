import pygame
import numpy as np
import math


class Sensor:
    """
    Class for sensor
    """
    def __init__(self, sensor_range, map):
        """

        :param sensor_range: range in which sensor can detect obstacle
        :param map: 2D map with obstacles that needs to be detected
        """
        self.sensor_range = sensor_range
        self.position = (0, 0)
        self.map = map
        self.width, self.height = pygame.display.get_surface().get_size()

    def distance(self, obstacle_position):
        """
        :param obstacle_position: position of detected obstacle in format (x, y)
        :return: distance from current position of sensor to detected obstacle
        """
        px = (obstacle_position[0] - self.position[0]) ** 2
        py = (obstacle_position[1] - self.position[1]) ** 2
        return math.sqrt(px + py)

    def get_sensor_data(self):
        """
        :return: data that sensor collected from its current position

        Function that collects data from the one position of sensor in all the 360 degrees in space around it,
        withing the given range. Line segment between sensor position and range is chopped into 100 segments,
        and for each segment it is being checked if there is an obstacle. Obstacle is each pixel of colour black.
        If there is such an obstacle, then add its distance and angle to the list sensor_data, relative to the current
        position of sensor. Also for each distance, angle pair add current sensor position to the sensor data list.
        """
        sensor_data = []
        x_sensor, y_sensor = self.position[0], self.position[1]

        for angle in np.linspace(0, 2*math.pi, 60, False):

            x_range, y_range = (x_sensor + self.sensor_range * math.cos(angle), y_sensor - self.sensor_range * math.sin(angle))

            # sample line segment into 100 samples, and check each for potential obstacle
            for i in range(100):
                w = i / 100
                x = int(x_range * w + x_sensor * (1-w))
                y = int(y_range * w + y_sensor * (1-w))

                if 0 < x < self.width and 0 < y < self.height:
                    color = self.map.get_at((x, y))
                    if (color[0], color[1], color[2]) == (0, 0, 0):
                        distance = self.distance((x, y))
                        obstacle = [distance, angle]
                        obstacle.append(self.position)
                        sensor_data.append(obstacle)
                        break
        if len(sensor_data) > 0:
            return sensor_data
        else:
            return False
