In this experiment, I implement the Cauchy residue theorem and investigate how the ROC affects the recovered time domain samples.

# Development

* Create an impulse signal
* Create the impulse response of the H(z) system and its inverse transform
* Implement the Cauchy residue theorem
* Numerically evaluate the inverse Z-transform
* Experiment with different contour radius

# Mathematics

First order IIR filter:

$
H(z)=\frac{1}{1-0.5z^{-1}} \\
$

The difference equation:

$
y(n)=x(n)+0.5y(n-1)
$

The inverse Z-transform:

$
h(n)=(0.5)^n u(n)
$

The inverse Z-transform equation:

$
h(n)=\frac{1}{2\pi j} \oint_C H(z)z^{n-1}dz \\
z=re^{j \theta} \\
dz=jre^{j \theta}d\theta = jzd \theta \\
h(n)=\frac{1}{2\pi j} \int_0^{2\pi}H(z)z^{n-1}(jz)d\theta
$

$
\boxed{ h(n)=\frac{1}{2\pi} \int_0^{2\pi} H(re^{j\theta})(re^{j\theta})^n d\theta }
$

Approximate using equally spaced samples (divide the circle into M angles):

$
\theta_k = \frac{2\pi k}{M}
$

$
h(n)\approx \frac{1}{M} \sum_{k=0}^{M-1}H(re^{j\theta_k})(re^{j\theta_k})^n
$

$
\boxed{
  h(n)\approx \frac{1}{M} \sum_{k=0}^{M-1}H(re^{j2\pi k/M})(re^{j2\pi k / M})^n
}
$

The idea is to evaluate $H(z)$ at specific steps along a contour. We define the radius of the contour using $r$. Instead of integrating over $\theta$, we integrate over $k$ up to the amount of angles $M$.

The $H(z)$ function we are evaluating at each time index is therefore:

$
H(re^{j2\pi k/M})=\frac{1}{1-0.5 re^{j2\pi k/M} }
$
