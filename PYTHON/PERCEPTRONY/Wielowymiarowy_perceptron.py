import numpy as np
from sklearn import datasets
import matplotlib.pyplot as plt
import random
import time

def norm_data(X, y):
    m = y.size
    X[:, 0] -= np.pi
    X[:, 0] /= np.max(X[:, 0])
    return X, y

def gen_data(m=1000):
    X1 = np.random.uniform(0, 2 * np.pi, [m, 1])
    X2 = np.random.uniform(-1, 1, [m, 1])
    y = np.ones(m)
    X = np.c_[X1, X2]
    for i in range(m):
        if np.abs(np.sin(X[i, 0])) > np.abs(X[i, 1]):
            y[i] = -1
    
    X = np.c_[np.ones(X.shape[0]), X]
    return norm_data(X, y)

def gen_centers(dim):
    return np.c_[np.random.uniform(-1, 1, [dim, 1]), np.random.uniform(-1, 1, [dim, 1])]


def increase_dim(X, centers, sigma):
    rows = X.shape[0]
    z = np.zeros((rows, len(centers)))
    licznik = 0
    for j, c in enumerate(centers):
        for i, row in enumerate(X):
            licznik = (row[0] - c[0])**2 + (row[1] - c[1])**2
            z[i, j] = np.exp(-licznik/(2*sigma)**2)

    
    z = np.c_[np.ones(z.shape[0]), z]
    return z


def Simple_Perceptron(X, y, n, max_it):
    dataset = np.c_[X, y]
    number_of_atributes = dataset.shape[1]
    w = np.zeros(number_of_atributes - 1)
    k = 0
    while True:
        E = []
        for row in dataset:
            pred = 1.0 if np.dot(row[:-1], w) > 0 else -1.0
            if pred != row[-1]: 
                E.append(row)
        
        if len(E) == 0 or k > max_it:
            return w, k


        if len(E) != 0:
            temp_row = E[random.randint(0, len(E) - 1)]
            w += n*temp_row[-1]*temp_row[:-1]
            k += 1
        
    
    return w, k



def contour_plot(w, I, centers, sigma):
    pass


def plotting(X, y, centers):
    if len(centers) != 0:
        plt.scatter(X[:, 0][y == 1], X[:, 1][y == 1], c='green', s=10)
        plt.scatter(X[:, 0][y == -1], X[:, 1][y == -1], c='blue', s=10)
        plt.scatter(centers[:, 0], centers[:, 1])
        plt.show()
    else:
        plt.scatter(X[:, 0][y == 1], X[:, 1][y == 1], c='green', s=10)
        plt.scatter(X[:, 0][y == -1], X[:, 1][y == -1], c='blue', s=10)
        plt.show()





def main():
    max_it = 2500
    I = 1000
    dim = 70
    sigma = 0.1
    n = 0.2
    centers = gen_centers(dim)
    X, y = gen_data(I)
    #plotting(X, y, centers)
    z = increase_dim(X, centers, sigma)
    w, k = Simple_Perceptron(z, y, n, max_it)
    contour_plot(w, I, centers, sigma)


main()
