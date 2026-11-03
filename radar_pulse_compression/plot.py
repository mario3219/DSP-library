import matplotlib.pyplot as plt
import numpy as np

x = np.fromfile("build/chirp.raw", dtype=np.double)
fft = np.fromfile("build/fft.raw", dtype=np.double)
y = np.fromfile("build/received.raw", dtype=np.double)

N = len(fft)
fs = 500
spectrum = np.fft.fftshift(fft)
frequencies = np.fft.fftshift(np.fft.fftfreq(N, d=1 / fs))

fig, axs = plt.subplots(3)

axs[0].plot(x)

axs[1].plot(frequencies, spectrum)
axs[1].set_xlabel("F(Hz)")

axs[2].plot(y)

axs[0].set_title("Transmitted signal")
axs[1].set_title("DFT of Transmitted signal")
axs[2].set_title("Received signal")

plt.show()
