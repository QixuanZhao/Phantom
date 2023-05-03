import numpy as np
import matplotlib.pyplot as plt

N = 512

def fov (theta):
    cosine = np.cos(theta)
    tangent = np.tan(theta)
    denominator = np.sqrt(1 + 2 * tangent * tangent)
    return np.arccos(cosine / denominator)

X = 90 * np.array(list(range(N + 1))) / N
Y = np.degrees(fov(np.radians(X)))

fig, ((ax)) = plt.subplots(1, 1)

ax.plot(X, Y)
ax.plot([0, 90], [90, 90])
ax.plot([0, 90], [45, 45])
ax.plot([0, 90], [0, 90])
ax.plot([45, 45], [0, Y[-1]])
ax.set_xlabel('cut-off')
ax.set_ylabel('fov')
ax.set_xlim(0, 90)
ax.set_ylim(0, 90)
plt.show()
