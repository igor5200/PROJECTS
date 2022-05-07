import numpy as np
import sys
counter_function = 0
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
    flat_stream = flat_stream.astype(np.int32)
    coded_stream = []
    
        
    for i in range(len(flat_stream)-1):
        if flat_stream[i] == flat_stream[i+1]:
            counter +=1 
        else:
            coded_stream.append([counter, flat_stream[i]])
            counter = 1
           
    coded_stream.append([counter, flat_stream[-1]])
    coded_stream = np.array(coded_stream).flatten()
    
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
        return np.array(decoded_stream)
    else:
        return np.array(decoded_stream).reshape(tuple(dimensions))
        



        

def split_img(img):
    horizontal_split = np.array_split(img, 2, axis = 0)
    I1, I2 = np.array_split(horizontal_split[0], 2, axis = 1)
    I3, I4 = np.array_split(horizontal_split[1], 2, axis = 1)
    return I1, I2, I3, I4
    
            
def join_img(I1, I2, I3, I4):
    I12 = np.concatenate((I1, I2), axis = 1)
    I34 = np.concatenate((I3, I4), axis = 1)
    I = np.concatenate((I12, I34), axis = 0)
    return I
        
                

    
    
    
    
    
        
class Node:
    def __init__(self, img):
        self.img = img
        
        
        if len(img.shape) < 3:
            if len(np.unique(img)) != 1 and len(img) > 0:
                I1, I2, I3, I4 = split_img(img)
                self.first_son = Node(I1)
                self.second_son = Node(I2)
                self.third_son = Node(I3)
                self.forth_son = Node(I4)
        else:
            counter = 0
            for layer in range(len(img.shape)):
                if len(np.unique(img[:, :, layer])) == 1:
                    counter += 1
            if counter != 3:
                I1, I2, I3, I4 = split_img(img)
                self.first_son = Node(I1)
                self.second_son = Node(I2)
                self.third_son = Node(I3)
                self.forth_son = Node(I4)
                
    def QUAD_TREE_DECODE(self):
        return join_img(self.first_son.QUAD_TREE_DECODE(), self.second_son.QUAD_TREE_DECODE, self.third_son.QUAD_TREE_DECODE, self.forth_son.QUAD_TREE_DECODE())
                
                
        
                
                
            
        
        
        
        
        
        
        
        
        
    
        







def main():
    stream1 = np.array([1,1,1,1,2,1,1,1,1,2,1,1,1,1])
    stream2 = np.array([1,2,3,1,2,3,1,2,3])
    stream3 = np.array([5,1,5,1,5,5,1,1,5,5,1,1,5])
    stream4 = np.array([-1,-1,-1,-5,-5,-3,-4,-2,1,2,2,1])
    stream5 = np.zeros((1,520))
    stream6 = np.arange(0,521,1)
    stream7 = np.array([[1, 2], [3, 4], [5, 6]])
    stream8 = np.eye(7)
    stream9 = np.dstack([np.eye(7),np.eye(7),np.eye(7)])
    stream10 = np.array([[2, 2, 2, 4, 4],
                         [2, 2, 2, 4, 0],
                         [2, 2, 4, 4, 0],
                         [1, 2, 2, 4, 4]])
    

    stream = stream10
   
    
    
    if(np.all(RLE_decoder(RLE_coder(stream)) == stream)):
        print("Dziala")
    else:
        print("Nie dziala")
    
    
    
    
    
    
    



main()