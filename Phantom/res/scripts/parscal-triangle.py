import numpy as np

array = []

N = 17

for i in range(N):
    for j in range(i - 1, 0, -1):
        array[j] += array[j - 1]
    array.append(1)
    # if i % 2 == 0:
    #     print(i, " :", np.array(array) / 2 ** i)

result = np.array(array) / 2 ** (N - 1)
print(len(result))
print(*result[int(N/2):], sep=', ')