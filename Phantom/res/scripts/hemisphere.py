import numpy as np
import matplotlib.pyplot as plt

N = 512
angleScale = 0.85

kernel = []

def normalize (vec):
    return vec / np.sqrt(np.sum(vec * vec))

for i in range(N):
    scale = i / N
    scale = 0.1 + scale * scale * (1 - 0.1)
    vec = np.array([
        np.random.rand() * 2 - 1,
        np.random.rand() * 2 - 1,
        np.random.rand()
    ])
    length = np.sqrt(np.dot(vec, vec))
    cosine = np.dot(vec, np.array([0,0,1])) / length

    while cosine < np.cos(angleScale * np.pi / 2):
        vec = np.array([
            np.random.rand() * 2 - 1,
            np.random.rand() * 2 - 1,
            np.random.rand()
        ])
        length = np.sqrt(np.dot(vec, vec))
        cosine = np.dot(vec, np.array([0,0,1])) / length

    kernel.append(
        scale * np.random.rand() * normalize(vec)
    )

nKernel = np.array(kernel)

fig = plt.figure()
ax = fig.add_subplot(projection = '3d')

ax.scatter(nKernel[:, 0], nKernel[:, 1], nKernel[:, 2], marker='o')
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()

for point in kernel:
    print('vec3({0:n}, {1:n}, {2:n}),'.format(*point))
