#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  double *data;
  size_t N;
} Signal;

double *read_all(int fd, size_t *N) {
  size_t size = 4096 * sizeof(double);
  double *buffer = malloc(size);
  if (buffer == NULL) {
    return NULL;
  }
  unsigned char *ptr = (unsigned char *)buffer;
  size_t count = sizeof(double);
  ssize_t n = 1;
  size_t total_bytes_read = 0;
  while (n > 0) {
    if (total_bytes_read + count > size) {
      size_t new_size = size * 2;
      void *tmp = realloc(buffer, new_size);
      if (tmp == NULL) {
        free(buffer);
        return NULL;
      }
      buffer = tmp;
      size = new_size;
      ptr = (unsigned char *)buffer + total_bytes_read;
    }
    n = read(fd, ptr, count);
    if (n < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        free(buffer);
        return NULL;
      }
    }
    total_bytes_read += n;
    ptr += n;
  }
  *N = total_bytes_read / sizeof(double);
  void *tmp = realloc(buffer, total_bytes_read);
  if (tmp == NULL) {
    free(buffer);
    return NULL;
  }
  buffer = tmp;
  return buffer;
}

int write_all(int fd, const void *buffer, size_t count) {
  const unsigned char *ptr = buffer;
  while (count > 0) {
    ssize_t n = write(fd, ptr, count);

    if (n < 0) {
      if (errno == EINTR)
        continue;

      return -1;
    }
    if (n == 0)
      return -1;

    ptr += n;
    count -= (size_t)n;
  }
  return 0;
}

Signal corr(const Signal *x, const Signal *y) {
  Signal output = {0};
  size_t N = y->N;
  output.data = malloc(N * sizeof(output.data));
  if (output.data == NULL) {
    return output;
  }
  double tmp = 0.0;
  for (size_t l = 0; l < N; l++) {
    for (size_t i = 0; i < N; i++) {
      if (i < l) {
        tmp += 0;
      } else {
        tmp += x->data[i] * y->data[i - l];
      }
    }
    output.data[l] = tmp;
    tmp = 0.0;
  }
  output.N = N;
  return output;
}

Signal load_signal(const char *path) {
  int fd = -1;
  Signal signal = {0};
  fd = open(path, O_RDONLY);
  if (fd == -1) {
    perror("open");
    return signal;
  }
  signal.data = read_all(fd, &signal.N);
  if (close(fd) == -1) {
    fprintf(stderr, "Failed to close descriptor\n");
    free(signal.data);
    signal.data = NULL;
    signal.N = 0;
    return signal;
  }
  return signal;
}

int save_signal(const char *path, Signal *signal) {
  int fd = -1;
  fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }
  int ret = write_all(fd, signal->data, (signal->N) * sizeof(signal->data));
  if (ret != 0) {
    fprintf(stderr, "Failed to write\n");
    return -1;
  }
  if (close(fd) == -1) {
    fprintf(stderr, "Failed to close descriptor\n");
    return -1;
  }
  return 0;
}

int main(void) {

  Signal x = {0};
  Signal y = {0};
  Signal rr = {0};

  x = load_signal("chirp.raw");
  y = load_signal("received.raw");

  if (x.data == NULL || y.data == NULL) {
    fprintf(stderr, "Failed to load data\n");
    goto cleanup;
  }

  rr = corr(&y, &x);

  if (rr.data == NULL) {
    fprintf(stderr, "Failed to generate crosscorrelations\n");
    goto cleanup;
  }

  int ret = save_signal("filtered.raw", &rr);
  if (ret != 0) {
    fprintf(stderr, "Failed to save signal\n");
    goto cleanup;
  }

cleanup:

  free(x.data);
  x.data = NULL;
  x.N = 0;

  free(y.data);
  y.data = NULL;
  y.N = 0;

  free(rr.data);
  rr.data = NULL;
  rr.N = 0;

  return 0;
}
