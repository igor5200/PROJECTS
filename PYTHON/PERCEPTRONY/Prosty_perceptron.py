import numpy as np
from sklearn import datasets
import matplotlib.pyplot as plt
import random
def gen_data():
    X, y = datasets.make_blobs(n_samples=150,n_features=2, centers=2, cluster_std=1.05, random_state=2)
    y[y == 0] = -1
    X = np.c_[np.ones(X.shape[0]), X]
    return X, y

def plotting(X, y, w):
    x1 = [min(X[:,1]), max(X[:,1])]
    m = np.array(-w[1]/w[2])
    c = np.array(-w[0]/w[2])
    x2 = m*x1 + c
    plt.plot(X[:, 1][y==-1], X[:, 2][y==-1], "rs")
    plt.plot(X[:, 1][y==1], X[:, 2][y==1], "bs")
    plt.plot(x1, x2, 'y-')
    plt.show()

def Simple_Perceptron(X, y, n):
    dataset = np.c_[X, y]
    number_of_atributes = dataset.shape[1] - 1
    w = np.zeros(number_of_atributes)
    k = 0
    while True:
        E = []
        for row in dataset:
            pred = 1.0 if np.dot(row[:-1], w) > 0 else -1.0
            if pred != row[-1]:
                E.append(row)
        
        if len(E) != 0:
            temp_row = E[random.randint(0, len(E) - 1)]
            w += n*temp_row[-1]*temp_row[:-1]
            k += 1
        else:
            break
    
    return w, k



def main():
    X, y = gen_data()
    w, k = Simple_Perceptron(X, y, 0.4)
    plotting(X, y, w)


main()
