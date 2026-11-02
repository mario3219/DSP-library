#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PI 3.14159265358979323846

double *wf(size_t N_out) {
  double *output = malloc(N_out * sizeof(*output));
  if (output == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < N_out; i++) {
    output[i] = 2.0f * ((float)rand() / RAND_MAX) - 1.0f;
  }
  return output;
}

double *xf2(size_t N_out, double f, double fs) {
  double *output = malloc(N_out * sizeof(*output));
  if (output == NULL)
    return NULL;
  for (size_t i = 0; i < N_out; i++) {
    output[i] = 2.0 * ((double)rand() / RAND_MAX) - 1.0;
  }
  return output;
}

double *xf(size_t N_out, double f, double fs) {
  double *output = malloc(N_out * sizeof(*output));
  if (output == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < N_out; i++) {
    output[i] = sin(2.0f * (float)PI * f * i / fs);
  }
  return output;
}

double *yf(size_t N_out, size_t D, double a, double *input) {
  double *output = malloc(N_out * sizeof(*output));
  if (output == NULL) {
    return NULL;
  }
  double *w = wf(N_out);
  if (w == NULL) {
    return NULL;
  }
  for (size_t i = 0; i < N_out; i++) {
    if (i < D) {
      output[i] = w[i];
    } else {
      output[i] = a * input[i - D] + w[i];
    }
  }
  free(w);
  return output;
}

double *corr(double *x, double *y, size_t N_out) {
  double *output = malloc(N_out * sizeof(*output));
  if (output == NULL) {
    return NULL;
  }
  double tmp = 0.0;
  for (size_t l = 0; l < N_out; l++) {
    for (size_t i = 0; i < N_out; i++) {
      if (i < l) {
        tmp += 0;
      } else {
        tmp += x[i] * y[i - l];
      }
    }
    output[l] = tmp;
    tmp = 0.0;
  }
  return output;
}

int main(void) {
  int fd1 = open("x.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd1 == -1) {
    perror("open");
    return 1;
  }

  int fd2 = open("y.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd2 == -1) {
    perror("open");
    return 1;
  }

  int fd3 = open("r.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd3 == -1) {
    perror("open");
    return 1;
  }

  size_t N_out = 10000;
  size_t D = 1000;
  double a = 0.9;
  double f = 233e6;
  double fs = 1e9;

  // double *x = xf(N_out, f, fs);
  double *x = xf2(N_out, f, fs); // switched to a non-periodic signal
  if (x == NULL) {
    fprintf(stderr, "Failed to generate transmitted signal");
    close(fd1);
    close(fd2);
    close(fd3);
    return EXIT_FAILURE;
  }
  double *y = yf(N_out, D, a, x);
  if (y == NULL) {
    fprintf(stderr, "Failed to generate received signal");
    free(x);
    free(y);
    close(fd1);
    close(fd2);
    close(fd3);
    return EXIT_FAILURE;
  }

  double *R = corr(y, x, N_out);
  if (R == NULL) {
    fprintf(stderr, "Failed to generate crosscorrelations");
    free(x);
    free(y);
    free(R);
    close(fd1);
    close(fd2);
    close(fd3);
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < N_out; i++) {
    double *ptr = &x[i];
    ssize_t n = write(fd1, ptr, sizeof(*ptr));
  }

  for (size_t i = 0; i < N_out; i++) {
    double *ptr = &y[i];
    ssize_t n = write(fd2, ptr, sizeof(*ptr));
  }

  for (size_t i = 0; i < N_out; i++) {
    double *ptr = &R[i];
    ssize_t n = write(fd3, ptr, sizeof(*ptr));
  }

  free(x);
  free(y);
  free(R);
  close(fd1);
  close(fd2);
  close(fd3);

  return 0;
}
