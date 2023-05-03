import numpy as np

def probable(x):
    return np.exp(-(x**2)/2) / np.sqrt(2 * np.pi)

def distributionBetween(a, b):
    area = 0
    step = 1e-6
    if a > b: step *= -1
    for x in range(int(a / step), int(b / step)):
        area += probable(x * step) * step
    
    return area

def distributionUntil(a):
    area = 0.5
    step = 1e-6
    if a < 0: step *= -1
    for x in range(0, int(a / step)):
        area += probable(x * step) * step
    
    return area

