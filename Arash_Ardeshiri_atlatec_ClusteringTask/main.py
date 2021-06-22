import cv2
import random

import numpy
import numpy as np
import sys


def remove_sfx(input_string: str, suffix: str) -> str:
    """removes a specific suffix from a string."""
    if suffix and input_string.endswith(suffix):
        return input_string[:-len(suffix)]
    return input_string


def random_point(x_low: int, x_limit: int, y_low: int, y_limit: int) -> object:
    """creates random 2D points based on a min and max value"""
    x = random.randint(x_low, x_limit)
    y = random.randint(y_low, y_limit)
    return x, y


def drill_points(img: numpy.ndarray) -> list:
    """finds best drilling points in the data set"""
    radius = 25
    drills = 64
    iteration = 700
    color = (255, 255, 255)
    thickness = -1
    final_coordinates = (0, 0)
    final_coordinates_list = []
    max_mask = None
    for i in range(drills):
        max_val = 0
        for j in range(iteration):
            center_coordinates = random_point(0, img.shape[1], 0, img.shape[0])
            plain = np.full(img.shape, 0, dtype=np.uint8)
            mask = cv2.circle(plain, center_coordinates, radius, color, thickness)
            result = cv2.bitwise_and(img, mask)
            count = cv2.sumElems(result)
            if count[0] > max_val:
                max_val = count[0]
                final_coordinates = center_coordinates
                max_mask = mask
        final_coordinates_list.append(final_coordinates)
        img = cv2.bitwise_and(img, cv2.bitwise_not(max_mask))  # removing drilled part from image
    return final_coordinates_list


if (len(sys.argv) > 1):
    filename = sys.argv[1]
    image = cv2.imread(filename, cv2.IMREAD_GRAYSCALE)
    drillingPoints_list = drill_points(image)
    filename = remove_sfx(filename, ".png") + "_drilling_plan" + ".txt"
    with open(filename, "w") as f:
        for cord in drillingPoints_list:
            print(cord[0], cord[1], file=f)
