import matplotlib.pyplot as plt
import numpy as np

x = np.fromfile("x.raw", dtype=np.double)
y = np.fromfile("y.raw", dtype=np.double)
r = np.fromfile("r.raw", dtype=np.double)

fig, axs = plt.subplots(3, 1)
axs[0].plot(x)
axs[0].set_xlim(1000, 1500)
axs[1].plot(y)
axs[1].set_xlim(1000, 1500)
axs[2].plot(r)
axs[2].set_xlim(800, 1200)

axs[0].set_title("Transmitted")
axs[1].set_title("Received")
axs[2].set_title("ACF")

plt.show()
