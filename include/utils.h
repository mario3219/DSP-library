#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

typedef struct {
  double *data;
  size_t N;
} Signal;

int utils_read(int fd, double **buffer, size_t N_in, size_t *N_out);
int utils_write(int fd, const void *buffer, size_t count);

Signal *load_signal(const char *path);
int save_signal(const char *path, Signal *signal);

#endif
