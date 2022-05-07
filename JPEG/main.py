import enum
import matplotlib.pyplot as plt
import cv2
import numpy as np
import scipy.fftpack
from math import sqrt

class TData:
    pass


def compress(img):
    YCrCb = cv2.cvtColor(img, cv2.COLOR_RGB2YCrCb).astype(int)
    Y = YCrCb[:, :, 0]
    Cr = YCrCb[:, :, 1]
    Cb = YCrCb[:, :, 2]
    data = TData()
    data.Y = Y
    data.Cr = Cr
    data.Cb = Cb
    return data


def decompress(data):
    Y = data.Y
    Cr = data.Cr
    Cb = data.Cb
    
    
    YCrCb=np.dstack([Y,Cr,Cb]).clip(0, 255).astype(np.uint8)
    
    img = cv2.cvtColor(YCrCb,cv2.COLOR_YCrCb2RGB)
    return img
    
def chroma_sampling(data, flag = True):
    if flag:    #4:4:4
        data.chroma_subsampling = "4:4:4"
        return data
    else:       #4:2:2
        data.Cr = data.Cr[:, ::2]
        data.Cb = data.Cb[:, ::2]
        data.chroma_subsampling = "4:2:2"
    
        
def chroma_desampling(data):
    if data.chroma_subsampling == "4:2:2":
        temp_list_Cr = np.zeros((data.Cr.shape[0], data.Cr.shape[1]*2), dtype = int)
        temp_list_Cb = np.zeros((data.Cb.shape[0], data.Cb.shape[1]*2), dtype = int)
        counter = 0
        for col in range(data.Cr.shape[1]):
            temp_list_Cr[:, col+counter] = data.Cr[:, col]
            temp_list_Cb[:, col+counter] = data.Cb[:, col]
            counter += 1
            temp_list_Cr[:, col+counter] = data.Cr[:, col]
            temp_list_Cb[:, col+counter] = data.Cb[:, col]
        
        data.Cr = temp_list_Cr
        data.Cb = temp_list_Cb

def split_data(data):
    splited_Y = []
    splited_Cr = []
    splited_Cb = []
    for row in range(0, data.Cr.shape[0], 8):
        for col in range(0, data.Cr.shape[1], 8):
            splited_Cr.append(data.Cr[row:row+8, col:col+8])
            splited_Cb.append(data.Cb[row:row+8, col:col+8])
            
    for row in range(0, data.Y.shape[0], 8):
        for col in range(0, data.Y.shape[1], 8):
            splited_Y.append(data.Y[row:row+8, col:col+8])

    data.splited_Y = np.array(splited_Y) - 128
    data.splited_Cr = np.array(splited_Cr) - 128
    data.splited_Cb = np.array(splited_Cb) - 128
    
    

def join_data(data):
    pass
    
    

    

def dct2(a):
    return scipy.fftpack.dct( scipy.fftpack.dct( a.astype(float), axis=0, norm='ortho' ), axis=1, norm='ortho' )


def idct2(a):
    return scipy.fftpack.idct( scipy.fftpack.idct( a.astype(float), axis=0 , norm='ortho'), axis=1 , norm='ortho')


def zigzag(A):
    template = np.array([
            [0,  1,  5,  6,  14, 15, 27, 28],
            [2,  4,  7,  13, 16, 26, 29, 42],
            [3,  8,  12, 17, 25, 30, 41, 43],
            [9,  11, 18, 24, 31, 40, 44, 53],
            [10, 19, 23, 32, 39, 45, 52, 54],
            [20, 22, 33, 38, 46, 51, 55, 60],
            [21, 34, 37, 47, 50, 56, 59, 61],
            [35, 36, 48, 49, 57, 58, 62, 63],
            ])
    if len(A.shape)==1:
        B=np.zeros((8,8))
        for r in range(0,8):
            for c in range(0,8):
                B[r,c]=A[template[r,c]]
    else:
        B = np.zeros((64,))
        for r in range(0,8):
            for c in range(0,8):
                B[template[r,c]]=A[r,c]
    return B


def DCT(data):
    dct_Y = []
    dct_Cr = []
    dct_Cb = []
    for block in data.splited_Y:
        dct_Y.append(zigzag(dct2(block)))
        
    for block, block2 in zip(data.splited_Cr, data.splited_Cb):
        dct_Cr.append(zigzag(dct2(block)))
        dct_Cb.append(zigzag(dct2(block2)))
        
    data.dct_Y = np.array(dct_Y).astype(int)
    data.dct_Cr = np.array(dct_Cr).astype(int)
    data.dct_Cb = np.array(dct_Cb).astype(int)
    
def IDCT(data):
    idct_Y = []
    idct_Cr = []
    idct_Cb = []
    
    for block in data.dct_Y:
        idct_Y.append(idct2(block.reshape((8, 8))))
    
    for block, block2 in zip(data.dct_Cr, data.dct_Cb):
        idct_Cr.append(idct2(block.reshape((8, 8))))
        idct_Cb.append(idct2(block2.reshape((8, 8))))

    data.idct_Y = np.array(idct_Y)
    data.idct_Cr = np.array(idct_Cr)
    data.idct_Cb = np.array(idct_Cb)
    
temp = np.array([[6, 1, 5 ,9, 1, 2, 5, 3],
                 [9, 7, 9, 2, 5, 5, 2, 4],
                 [6, 0, 7, 7, 1, 0, 8, 4],
                 [0, 8, 2, 4, 3, 6, 3, 6],
                 [6, 6, 7, 8, 9, 5, 2, 3],
                 [9, 9, 9, 9, 0, 7, 6, 7],
                 [3, 3, 6, 0, 4, 9, 7, 1],
                 [9, 0, 0, 5, 5, 3, 8, 5]])
img = (plt.imread("orange.png")[:, :, :3] * 255).astype(np.uint8)
data = compress(img)
chroma_sampling(data, False)
split_data(data)
DCT(data)
IDCT(data)
data.Y = data.idct_Y.reshape((128, -1))
data.Cr = data.idct_Cr.reshape((128, -1))
data.Cb = data.idct_Cb.reshape((128, -1))
chroma_desampling(data)
img2 = decompress(data)


fig, axs = plt.subplots(4, 2 , sharey=True)
fig.set_size_inches(9,13)
axs[0,0].imshow(img)
YCrCb = cv2.cvtColor(img, cv2.COLOR_RGB2YCrCb)





axs[1, 0].imshow(YCrCb[:, :, 0], cmap=plt.cm.gray)
axs[2, 0].imshow(YCrCb[:, :, 1], cmap=plt.cm.gray)
axs[3, 0].imshow(YCrCb[:, :, 2], cmap=plt.cm.gray)

axs[0,1].imshow(img2)
YCrCb2 = cv2.cvtColor(img2, cv2.COLOR_RGB2YCrCb)
axs[1, 1].imshow(YCrCb2[:, :, 0], cmap=plt.cm.gray)
axs[2, 1].imshow(YCrCb2[:, :, 1], cmap=plt.cm.gray)
axs[3, 1].imshow(YCrCb2[:, :, 2], cmap=plt.cm.gray)

plt.show()