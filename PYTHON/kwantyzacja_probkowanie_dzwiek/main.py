import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd
import soundfile as sf
import scipy.fftpack
from scipy.io import wavfile
from scipy.interpolate import interp1d
from scipy.io.wavfile import write


def quantization(signal,no_of_bits):
    typ = signal.dtype
    m =-1
    n = 1
    quant_signal = signal.astype(np.float32)
    levels = 2**no_of_bits -1
    
    if( np.issubdtype(typ,np.integer)):
        m = np.iinfo(typ).min
        n = np.iinfo(typ).max
    
        
    quant_signal = (quant_signal - m) /(n-m)
    quant_signal =np.round( quant_signal * levels) / levels

    quant_signal = quant_signal * (n-m) + m
    quant_signal = quant_signal.astype(typ)

    return quant_signal


def decimation(signal, n, FS):
    return signal[::n], FS // n




def interpolation(signal, FS, new_FS, linear):
    if linear:
        T = len(signal)/FS
        x = np.linspace(0, T,len(signal))
        lin = interp1d(x,signal)
        x1 = int(T*new_FS)
        Y = np.linspace(0, T,x1)
        y = lin(Y).astype(signal.dtype)
    else:
        T = len(signal)/FS
        x = np.linspace(0, T,len(signal))
        nonlin = interp1d(x,signal, kind='cubic')
        x1 = int(T*new_FS)
        Y = np.linspace(0, T,x1)
        y = nonlin(Y).astype(signal.dtype)
    
    return y, new_FS


def plot_signal(signal, FS):
    fsize=2**8
    plt.figure()
    
    
    plt.subplot(3,1,1)
    plt.plot(np.arange(0,signal.shape[0])/FS,signal)
    plt.xlim((0.02, 0.03))
    plt.xlabel('Czas')
    #plt.show()
    
    plt.subplot(3,1,2)
    yf = scipy.fftpack.fft(signal,fsize)
    plt.plot(np.arange(0,FS/2,FS/fsize),np.abs(yf[:fsize//2]))
    plt.xlabel('Częstotliwość')
    #plt.show()
    
    plt.subplot(3,1,3)
    plt.plot(np.arange(0,FS/2,FS/fsize),20*np.log10( np.abs(yf[:fsize//2])))
    plt.xlabel('Częstotliwość')
    plt.show()
    





def main():
    signal, FS = sf.read('sin_8000Hz.wav', dtype=np.int32)
    #plot_signal(signal, FS)
    BITS = [4, 8, 16, 24]
    Hzs = [2000, 4000, 8000, 16000, 24000, 41000]
    # for bits in BITS:
    #     new_signal = quantization(signal, bits)
    #     plot_signal(new_signal, FS)
    
    # for bits in BITS:
    #     new_signal = quantization(signal, bits)
    #     name = "new_signal" + str(bits) + ".wav"
    #     write(name, FS, new_signal.astype(np.int32))
    
    for hz in Hzs:
        new_signal, new_FS = decimation(signal, int(FS/hz), FS)
        plot_signal(new_signal, new_FS)
    
    # for hz in Hzs:
    #     new_signal, new_FS = decimation(signal, int(FS/hz), FS)
    #     name = "new_signal" + str(hz) + ".wav"
    #     write(name, new_FS, new_signal.astype(np.int32))
    
    # for hz in Hzs:
    #     print(hz)
    #     new_signal, new_FS = interpolation(signal, FS, hz, False)
    #     plot_signal(new_signal, new_FS)
    
    # for hz in Hzs:
    #     new_signal, new_FS = interpolation(signal, FS, hz, True)
    #     name = "new_signal" + str(hz) + ".wav"
    #     write(name, new_FS, new_signal.astype(np.int32))

        
    
    
    
main()