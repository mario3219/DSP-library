import matplotlib.pyplot as plt
import numpy as np

samples = np.fromfile("build/data.raw", dtype=np.float32)
down_sampled = np.fromfile("build/data.downsampled", dtype=np.float32)

fig, ax = plt.subplots(2, 1, sharex=False)

ax[0].plot(samples)
ax[0].set_title("Original signal")
ax[0].grid()

ax[1].plot(down_sampled)
ax[1].set_title("Downsampled signal")
ax[1].grid()

plt.tight_layout()
plt.show()
