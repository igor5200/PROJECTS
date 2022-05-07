import numpy as np
import sounddevice as sd
import soundfile as sf
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d
import scipy.fftpack



def kwantyzacja(signal, d):
    signal_type = signal.dtype
    if(np.issubdtype(signal_type, np.integer)):
        m = np.iinfo(signal_type).min
        n = np.iinfo(signal_type).max
    else:
        m = -1
        n = 1
    new_signal = signal.copy().astype(float)
    new_signal = np.round((new_signal - m)/(n-m) * d) 
    new_signal = (new_signal/d)*(n-m) + m
    new_signal = new_signal.astype(signal_type)
    return new_signal
    
def decymacja(signal, n, FS):
    new_FS = int(FS/n)
    new_signal = signal[::n].copy()
    return new_signal, new_FS
    
def interpolacja(signal, old_fs, new_fs, linear):
    if linear:
        T = len(signal)/old_fs
        x = np.linspace(0, T,len(signal))
        metode_lin=interp1d(x,signal)
        x1 = int(T*new_fs)

        Y = np.linspace(0, T,x1)
        
        y=metode_lin(Y).astype(signal.dtype)
    else:
        T = len(signal)/old_fs
        x = np.linspace(0, T,len(signal))
        metode_nonlin=interp1d(x,signal, kind='cubic')
        x1 = int(T*new_fs)
        Y = np.linspace(0, T,x1)
        y=metode_nonlin(Y).astype(signal.dtype)
    
    return y, new_fs
    

def plot(signal, FS):
    fsize=2**8
    plt.figure()
    plt.subplot(2,1,1)
    plt.xlabel('czas')
    plt.plot(np.arange(0,signal.shape[0])/FS,signal)
    plt.xlim((0.02, 0.1))
    plt.subplot(2,1,2)
    plt.xlabel('częstotliwość')
    yf = scipy.fftpack.fft(signal,fsize)
    
    plt.plot(np.arange(0,FS/2,FS/fsize),np.abs(yf[:fsize//2]))
    plt.show()


def plot_dec(signal,fs):
    fsize=2**8
    yf = scipy.fftpack.fft(signal,fsize)
    
    plt.plot(np.arange(0,fs/2,fs/fsize),20*np.log10( np.abs(yf[:fsize//2])))
    plt.xlabel('Częstotliwość')
    plt.show()


def main():
    BITS = 8
    d = 2**BITS - 1
    data, FS = sf.read('sin_60Hz.wav', dtype=np.int32)
    new_signal, new_FS = interpolacja(data, FS, FS//2000, False)
    plot(new_signal, new_FS)




main()