# Radar Pulse Compression

A long radar pulse provides more transmitted energy, but a simple unmodulated long pulse gives poor range resolution. A short pulse provides better range resolution, but at the same peak power it transmits less energy.

Instead of transmitting a constant frequency pulse, we transmit a chirp, a signal whose frequency changes over time.

When we correlate the received chirp with the transmitted chirp, we obtain a narrow correlation peak.

**Learning Goal:** A long transmitted pulse can produce a narrow peak after matched filtering, allowing accurate delay estimation and fine range resolution.

**Development steps**

* Generate the chirp signal
* Analyze the FFT and examine frequency contents
* Simulate the received signal containing delayed copies of the transmitted signal along with added noise
* Implement matched filtering
* Convolve received signal with the matched filter and investigate peaks corresponding to the target delays
* Investigate various chirps with different bandwidths and the resulting range resolution
