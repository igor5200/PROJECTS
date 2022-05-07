import random
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
from sklearn import datasets




def f(x):
    return 1.0 if x > 0 else -1




def Perceptron(X, n):
    x, y = X.shape
    w = np.zeros(y - 1)
    k = 0
    
    while True:
        E = []
        for i, row in enumerate(X):
            pred = f(np.dot(row[:-1], w))
            if(pred - row[-1]) != 0:
                E.append(row)
                w += n *row[-1]*row[:-1]
                k = k + 1
        if len(E) == 0:
            break
    return w, k
        
def main():    
    X, y = datasets.make_blobs(n_samples=100,n_features=2, centers=2, cluster_std=0.5)
    y[y == 0] = -1
    X = np.c_[np.ones(X.shape[0]), X, y]
    w, k = Perceptron(X, 0.9)
    x1 = [min(X[:,1]), max(X[:,1])]
    m = np.array(-w[1]/w[2])
    c = np.array(-w[0]/w[2])
    x2 = m*x1 + c
    plt.plot(X[:, 1][y==-1], X[:, 2][y==-1], "rs")
    plt.plot(X[:, 1][y==1], X[:, 2][y==1], "bs")
    plt.plot(x1, x2, 'y-')
    plt.show()

main()