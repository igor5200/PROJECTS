import matplotlib
import numpy as np
import matplotlib.pyplot as plt
import math as m
import cv2
def f(img, x, y, Q_y, Q_y2, Q_x, Q_x2):
    return img[Q_y][Q_x]*(1-y)*(1-x)+img[Q_y2][Q_x]*y*(1-x)+img[Q_y][Q_x2]*(1-y)*x+img[Q_y2][Q_x2]*y*x


def NN(img, scale):
    new_width = int(img.shape[1]*scale)
    new_height = int(img.shape[0]*scale)
    
    height_grid = np.linspace(0, img.shape[0] - 1, new_height)
    width_grid = np.linspace(0, img.shape[1] - 1, new_width)
    
    new_img = np.zeros((new_height, new_width, 3), dtype=np.uint8)
    
    for row in range(new_height):
        est_y = round(height_grid[row])
        for col in range(new_width):
            est_x = round(width_grid[col])
            new_img[row][col] = img[est_y][est_x]
            
            
    return new_img


def Interpolacja_dwuliniowa(img, scale):
    new_width = int(img.shape[1]*scale)
    new_height = int(img.shape[0]*scale)
    
    height_grid = np.linspace(0, img.shape[0] - 1, new_height)
    width_grid = np.linspace(0, img.shape[1] - 1, new_width)
    
    new_img = np.zeros((new_height, new_width, 3), dtype=np.uint8)
    
    for row in range(new_height):
        Q_y = m.floor(height_grid[row])
        Q_y2 = int(np.ceil(height_grid[row]))
        y = height_grid[row] - Q_y
        for col in range(new_width):
            Q_x = m.floor(width_grid[col])
            Q_x2 = int(np.ceil(width_grid[col]))
            x = width_grid[col] - Q_x
            new_img[row, col] = f(img, x, y, Q_y, Q_y2, Q_x, Q_x2)
                
            
            
            
    return new_img
    
def srednia(img, scale):
    new_width = int(img.shape[1]*scale)
    new_height = int(img.shape[0]*scale)
    
    
    height_grid = np.linspace(0, img.shape[0] - 1, new_height)
    width_grid = np.linspace(0, img.shape[1] - 1, new_width)
    
    new_img = np.zeros((new_height, new_width, 3), dtype=np.uint8)
    for canal in range(3):
        for row in range(new_height):
            est_y = round(height_grid[row])
            for col in range(new_width):
                values = []
                est_x = round(width_grid[col])
                if(est_y - 1 > -1):#GORA
                    values.append(img[est_y - 1][est_x][canal])
                    if(est_x - 1 > -1):
                        values.append(img[est_y - 1][est_x - 1][canal])#LEWO
                    if(est_x + 1 < img.shape[1]):
                        values.append(img[est_y - 1][est_x + 1][canal])#PRAWO
                
                
                if(est_x - 1 > -1):#LEWO
                    values.append(img[est_y][est_x - 1][canal])
                if(est_x + 1 < img.shape[1]):#PRAWO
                    values.append(img[est_y][est_x + 1][canal])
                    
                    
                if(est_y + 1 < img.shape[0]):#DOL
                    values.append(img[est_y + 1][est_x][canal])
                    if(est_x - 1 > -1):#LEWO
                        values.append(img[est_y + 1][est_x - 1][canal])
                    if(est_x + 1 < img.shape[1]):#PRAWO
                        values.append(img[est_y + 1][est_x + 1][canal])
                
                values.append(img[est_y][est_x][canal])
                srednia = np.mean(values)
                new_img[row][col][canal] = srednia
                
    return new_img
            
            
def mediana(img, scale):
    new_width = int(img.shape[1]*scale)
    new_height = int(img.shape[0]*scale)
    
    
    height_grid = np.linspace(0, img.shape[0] - 1, new_height)
    width_grid = np.linspace(0, img.shape[1] - 1, new_width)
    
    new_img = np.zeros((new_height, new_width, 3), dtype=np.uint8)
    
    for canal in range(3):
        for row in range(new_height):
            est_y = round(height_grid[row])
            for col in range(new_width):
                values = []
                est_x = round(width_grid[col])
                if(est_y - 1 > -1):#GORA
                    values.append(img[est_y - 1][est_x][canal])
                    if(est_x - 1 > -1):
                        values.append(img[est_y - 1][est_x - 1][canal])#LEWO
                    if(est_x + 1 < img.shape[1]):
                        values.append(img[est_y - 1][est_x + 1][canal])#PRAWO
                
                
                if(est_x - 1 > -1):#LEWO
                    values.append(img[est_y][est_x - 1][canal])
                if(est_x + 1 < img.shape[1]):#PRAWO
                    values.append(img[est_y][est_x + 1][canal])
                    
                    
                if(est_y + 1 < img.shape[0]):#DOL
                    values.append(img[est_y + 1][est_x][canal])
                    if(est_x - 1 > -1):#LEWO
                        values.append(img[est_y + 1][est_x - 1][canal])
                    if(est_x + 1 < img.shape[1]):#PRAWO
                        values.append(img[est_y + 1][est_x + 1][canal])
                
                values.append(img[est_y][est_x][canal])
                srednia = np.median(values)
                new_img[row][col][canal] = srednia
                    
                
    return new_img
    
      
def srednia_wazona(img, scale):
    new_width = int(img.shape[1]*scale)
    new_height = int(img.shape[0]*scale)
    
    
    height_grid = np.linspace(0, img.shape[0] - 1, new_height)
    width_grid = np.linspace(0, img.shape[1] - 1, new_width)
    
    new_img = np.zeros((new_height, new_width, 3), dtype=np.uint8)
    
    for canal in range(3):

        for row in range(new_height):
            est_y = round(height_grid[row])
            for col in range(new_width):
                values = []
                weights = []
                est_x = round(width_grid[col])
                if(est_y - 1 > -1):#GORA
                    values.append(img[est_y - 1][est_x][canal] * 4)
                    weights.append(4)
                    if(est_x - 1 > -1):
                        values.append(img[est_y - 1][est_x - 1][canal] * 2)#LEWO
                        weights.append(2)
                    if(est_x + 1 < img.shape[1]):
                        values.append(img[est_y - 1][est_x + 1][canal] * 2)#PRAWO
                        weights.append(2)
                
                
                if(est_x - 1 > -1):#LEWO
                    values.append(img[est_y][est_x - 1][canal]*4)
                    weights.append(4)
                if(est_x + 1 < img.shape[1]):#PRAWO
                    values.append(img[est_y][est_x + 1][canal]*4)
                    weights.append(4)
                    
                    
                if(est_y + 1 < img.shape[0]):#DOL
                    values.append(img[est_y + 1][est_x][canal] * 4)
                    weights.append(4)
                    if(est_x - 1 > -1):#LEWO
                        values.append(img[est_y + 1][est_x - 1][canal] * 2)
                        weights.append(2)
                    if(est_x + 1 < img.shape[1]):#PRAWO
                        values.append(img[est_y + 1][est_x + 1][canal] * 2)
                        weights.append(2)
                values.append(img[est_y][est_x][canal]*5)
                weights.append(5)
                srednia = np.sum(values)/np.sum(weights)
                new_img[row][col][canal] = srednia    
    
    return new_img



def main():
    # img= np.zeros((100,100,3),dtype=np.uint8)
    # img[1,1,:] = 255
    #[0:1000, 4000:]
    img2 = cv2.imread('./SM_LAB03/0006.jpg')[690:2200, 1600:2250]
    img2 = cv2.cvtColor(img2 , cv2.COLOR_BGR2RGB)
    # img3 = matplotlib.image.imread("./SM_LAB03/0006.jpg")
    # img4 = matplotlib.image.imread("./SM_LAB03/0007.jpg")
    #new_img = NN(img2, 0.05)
    new_img2 = Interpolacja_dwuliniowa(img2, 0.05)
    # new_img = srednia(img2, 0.05)
    # new_img2 = mediana(img2, 0.05)
    # new_img3 = srednia_wazona(img2, 0.05)
    # plt.imshow(img2)
    # plt.show()
    # plt.imshow(new_img)
    # plt.show()
    # plt.imshow(new_img2)
    # plt.show()
    # plt.imshow(new_img3)
    # plt.show()
    # fig, axs = plt.subplots(1, 3)
    # axs[0].imshow(new_img)
    # axs[1].imshow(new_img2)
    # axs[2].imshow(new_img2)
    # plt.show()
    #300 350
    #200 250
    edges = cv2.Canny(new_img2, 300, 350)
    plt.imshow(edges)
    plt.show()

    

    

    

    
    
    
main()
    