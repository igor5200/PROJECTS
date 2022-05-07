from cv2 import THRESH_BINARY
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import random


paleta2 = np.array([[0,0,0],         #black
[0,1,1],          #aqua
[0,0,1],          #blue
[1,0,1],          #fuchsia
[0,0.5,0],        #green
[0.5,0.5,0.5],    #grey
[0,1,0],          #lime
[0.5,0,0],        #maroon
[0,0,0.5],        #navy
[0.5,0.5,0],      #olive
[0.5,0,0.5],      #purple
[1,0,0],          #red
[0.75,0.75,0.75], #silver
[0,0.5,0.5],      #teal
[1,1,1],          #white
[1,1,0]])          #yellow


paleta3 = np.array([[0., 0., 0.],
[0., 0., 1.],
[0., 1., 0.],
[0., 1., 1.],
[1., 0., 0.],
[1., 0., 1.],
[1., 1., 0.],
[1., 1., 1.]])



def kwantyzacja(img, d):
    typ = img.dtype
    m =-1
    n = 1
    quant_signal = img.astype(np.float32)
    if( np.issubdtype(typ,np.integer)):
        m = np.iinfo(typ).min
        n = np.iinfo(typ).max
    
        
    quant_signal = (quant_signal - m) /(n-m)
    quant_signal = np.round( quant_signal * d) / d

    quant_signal = quant_signal * (n-m) + m
    quant_signal = quant_signal.astype(typ)

    return quant_signal





def colorFit(pixel, paleta):
    return paleta[np.argmin(np.linalg.norm(pixel - paleta, axis=1))]

def Fit(img, paleta):
    if len(img.shape) < 3:
        new_img = np.zeros((img.shape[0], img.shape[1]))
    else:
        new_img = np.zeros(img.shape)
        
    new_img = new_img.astype(np.float32)
    new_img = new_img/255
    for row in range(img.shape[0]):
        for col in range(img.shape[1]):
            new_img[row][col] = colorFit(img[row][col], paleta)

    return new_img

def random_dithering(img):
    new_img = np.zeros((img.shape[0],img.shape[1]),dtype=np.float32)

    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            if img[i,j] > np.random.rand():
                new_img[i,j] = 1
    return new_img
    
    

def FS(img, paleta):
    new_img = np.copy(img)
    new_img = new_img.astype(np.float32)
    new_img = new_img/255
    for x in range(img.shape[0]):
        for y in range(img.shape[1]):
            oldpixel = np.copy(new_img[x][y])
            newpixel = colorFit(oldpixel, paleta) 
            new_img[x][y] = newpixel
            quant_error = oldpixel - newpixel

            if x + 1 < img.shape[0]:
                new_img[x + 1][y] += quant_error * 7 / 16
                if y + 1 < img.shape[1]:
                    new_img[x + 1][y + 1] += quant_error * 1 / 16

            if y + 1 < img.shape[1]:
                new_img[x][y + 1] += quant_error * 5 / 16
                new_img[x - 1][y + 1] += quant_error * 3 / 16
                
    return np.clip(new_img, 0, 1)


def create_bayer_matrix(BITS):
    if BITS == 1:
        return np.array([[0.5]])
    
    M = np.array([[0, 2], [3, 1]])
    n = 2
    while(n!=BITS):
        M = np.hstack((np.vstack((M * 4,M * 4 + 3)),np.vstack((M * 4 + 2,M * 4 + 1))))
        n *= 2
        
    M = M/BITS**2-0.5
    return M

def OD(img, paleta, BITS):
    M = create_bayer_matrix(BITS)
    new_img = np.copy(img)
    new_img = new_img.astype(np.float32)
    if( np.issubdtype(img.dtype,np.integer)):
        new_img /= 255
    
    for row in range(img.shape[0]):
        for col in range(img.shape[1]):
            new_img[row][col] = colorFit(new_img[row][col] + M[row%M.shape[0]][col % M.shape[0]], paleta)
    
    
    
    return new_img
    
    

def main():
    paleta = np.linspace(0, 1, 2).reshape(2, 1)
    d = 2**1 - 1
    img = plt.imread("./SM_LAB04/0016.jpg")[:, :, 0:3]
    new_img = kwantyzacja(img, 2)
    # plt.imshow(new_img, 'gray')
    # plt.show()
    # plt.imshow(random_dithering(img), cmap='gray')
    # plt.show()
    plt.imshow(img)
    plt.show()
    plt.imshow(OD(img, paleta2, 8), cmap='gray')
    plt.show()
    plt.imshow(FS(img, paleta2), cmap='gray')
    plt.show()

   

            
    
        
    



main()