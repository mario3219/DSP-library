#ifndef UTILS_H
#define UTILS_H

#include <cstddef>

typedef struct {
  double *data;
  size_t N;
} Signal;

int utils_read(int fd, void *buffer, size_t *N);
int utils_write(int fd, const void *buffer, size_t count);

int load_signal(const char *path, Signal *signal);
int save_signal(const char *path, Signal *signal);

#endif
