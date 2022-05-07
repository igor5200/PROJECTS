import numpy as np
import matplotlib.pyplot as plt
from tqdm import tqdm
import sys

def get_size(obj, seen=None):
    size = sys.getsizeof(obj)
    if seen is None:
        seen = set()
    obj_id = id(obj)
    if obj_id in seen:
        return 0
    seen.add(obj_id)
    if isinstance(obj,np.ndarray):
        size=obj.nbytes
    elif isinstance(obj, dict):
        size += sum([get_size(v, seen) for v in obj.values()])
        size += sum([get_size(k, seen) for k in obj.keys()])
    elif hasattr(obj, '__dict__'):
        size += get_size(obj.__dict__, seen)
    elif hasattr(obj, '__iter__') and not isinstance(obj, (str, bytes, bytearray)):
        size += sum([get_size(i, seen) for i in obj])
    return size

def RLE_coder(stream):
    counter = 1
    flat_stream = stream.flatten()
    flat_stream = flat_stream.astype(np.uint8)
    coded_stream = []
    
        
    for i in range(len(flat_stream)-1):
        if flat_stream[i] == flat_stream[i+1]:
            counter +=1 
        else:
            coded_stream.append([counter, flat_stream[i]])
            counter = 1
           
    coded_stream.append([counter, flat_stream[-1]])
    #coded_stream = np.array(coded_stream, dtype = np.uint8).flatten()#Dla samej kompresji. Wartosc wymiarow przekracza zakres uint8
    coded_stream = np.array(coded_stream).flatten()#Dla dekompresji. 
    dimensions = stream.shape
    for i in reversed(dimensions):
        coded_stream = np.insert(coded_stream, 0, i)
        
    
    coded_stream = np.insert(coded_stream, 0, len(stream.shape))
    return coded_stream

def RLE_decoder(stream):
    nr_of_dimensions = stream[0]
    dimensions = []
    for i in range(1, nr_of_dimensions+1):
        dimensions.append(stream[i])
    
    
    repetitions = stream[nr_of_dimensions+1::2]
    values = stream[nr_of_dimensions+2::2]
    decoded_stream = []

    for rep, val in zip(repetitions, values):
        for i in range(rep):
            decoded_stream.append(val)
    
    
    if nr_of_dimensions == 1:
        return np.array(decoded_stream, dtype=np.uint8)
    else:
        return np.array(decoded_stream, dtype=np.uint8).reshape(tuple(dimensions))

def split_img(img):
    I12, I34 = np.array_split(img, 2, axis = 0)
    I1, I2 = np.array_split(I12, 2, axis = 1)
    I3, I4 = np.array_split(I34, 2, axis = 1)
    return I1, I2, I3, I4

def join_img(I1, I2, I3, I4):
    I12 = np.concatenate((I1, I2), axis = 1)
    I34 = np.concatenate((I3, I4), axis = 1)
    I = np.concatenate((I12, I34), axis = 0)
    return I


def is_leaf(img):
        if len(img.shape) < 3:
            return 0
        else:
            return np.zeros((1, 3)).astype(int)
    

def decode(root):
    if root.is_leaf:
        return np.tile(root.leaf, (root.height, root.width))
    return join_img(decode(root.first_son), decode(root.second_son), decode(root.third_son), decode(root.fourth_son)).astype(int)
    
    
    


class Node:
    def __init__(self, img):
        self.height = img.shape[0]
        self.width = img.shape[1]
        self.is_leaf = True
        
        if len(img) == 0:
            self.leaf = is_leaf(img)
        
        self.leaf = np.mean(img, axis = (0, 1)).astype(int)
        
        if not np.all(img == self.leaf):
            self.is_leaf = False
            I1, I2, I3, I4 = split_img(img)
            self.first_son = Node(I1)
            self.second_son = Node(I2)
            self.third_son = Node(I3)
            self.fourth_son = Node(I4)
            



def main():
    # stream1 = np.array([1,1,1,1,2,1,1,1,1,2,1,1,1,1])
    # stream2 = np.array([1,2,3,1,2,3,1,2,3])
    # stream3 = np.array([5,1,5,1,5,5,1,1,5,5,1,1,5])
    # stream4 = np.array([-1,-1,-1,-5,-5,-3,-4,-2,1,2,2,1])
    # stream5 = np.zeros((1,520))
    # stream6 = np.arange(0,521,1)
    # stream7 = np.array([[1, 2], [3, 4], [5, 6]])
    # stream8 = np.eye(7)
    # stream9 = np.dstack([np.eye(7),np.eye(7),np.eye(7)])
    stream10 = np.array([[2, 2, 2, 4, 4],
                         [2, 2, 2, 4, 0],
                         [2, 2, 4, 4, 0],
                         [1, 2, 2, 4, 4]])
    

    stream = plt.imread('rysunek_techniczny.jpg')
    
    #stream = stream10
    #coded_stream = RLE_coder(stream)
    coded_stream = Node(stream)
    #new_stream = RLE_decoder(coded_stream)
    new_stream = decode(coded_stream).reshape(stream.shape)
    if np.array_equal(stream, new_stream):
        print("Dziala")
    print(f"Rozmiar oryginalny {get_size(stream)}\nRozmiar po kompresji {get_size(coded_stream)}\nSkutecznosc kompresji {get_size(coded_stream)/get_size(stream)}")
    
        
    




main()