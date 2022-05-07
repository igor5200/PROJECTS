import numpy as np



array = np.array([11, 12, 13, 14, 15])

test = np.array([[1, 2, 3, 4, 5], [6, 7, 8, 9, 10]])


test = np.append(test, array, axis = 0)
print(test)