#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <papi.h>

double get_time() {
  struct timeval tp;
  gettimeofday(&tp, NULL);

  return (double)tp.tv_usec / 1000000 + tp.tv_sec;
}

int64_t *ijk_type(int64_t *a, int64_t *b, int32_t *size) { // 0
  int32_t N = *size;
  int64_t *res = (int64_t*)malloc(N * N * sizeof(int64_t));
  res = memset(res, 0, N * N * sizeof(int64_t));

  for (int32_t i = 0; i < N; i++) {
    for (int32_t j = 0; j < N; j++) {
      for (int32_t k = 0; k < N; k++) {
        res[i * N + j] += a[i * N + k] * b[k * N + j];
      }
    }
  }

  return res;
}

int64_t *ikj_type(int64_t *a, int64_t *b, int32_t *size) { // 1
  int32_t N = *size;
  int64_t *res = (int64_t *)malloc(N * N * sizeof(int64_t));
  res = memset(res, 0, N * N * sizeof(int64_t));

  for (int32_t i = 0; i < N; ++i) {
    for (int32_t k = 0; k < N; ++k) {
      for (int32_t j = 0; j < N; ++j) {
        res[i * N + j] += a[i * N + k] * b[k * N + j];
      }
    }
  }

  return res;
}

int64_t *jik_type(int64_t *a, int64_t *b, int32_t *size) { // 3
  int32_t N = *size;
  int64_t *res = (int64_t *)malloc(N * N * sizeof(int64_t));
  res = memset(res, 0, N * N * sizeof(int64_t));

  for (int32_t j = 0; j < N; j++) {
    for (int32_t i = 0; i < N; i++) {
      for (int32_t k = 0; k < N; k++) {
        res[i * N + j] += a[i * N + k] * b[k * N + j];
      }
    }
  }

  return res;
}

int64_t *jki_type(int64_t *a, int64_t *b, int32_t *size) { // 4
  int32_t N = *size;
  int64_t *out_m = (int64_t *)malloc(N * N * sizeof(int64_t));
  out_m = memset(out_m, 0, N * N * sizeof(int64_t));

  for (int32_t j = 0; j < N; j++) {
    for (int32_t k = 0; k < N; k++) {
      for (int32_t i = 0; i < N; i++) {
        out_m[i * N + j] += a[i * N + k] * b[k * N + j];
      }
    }
  }

  return out_m;
}

int64_t *kij_type(int64_t *a, int64_t *b, int32_t *size) { // 2
  int32_t N = *size;
  int64_t *res = (int64_t *)malloc(N * N * sizeof(int64_t));
  res = memset(res, 0, N * N * sizeof(int64_t));

  for (int32_t k = 0; k < N; k++) {
    for (int32_t i = 0; i < N; i++) {
      for (int32_t j = 0; j < N; j++) {
        res[i * N + j] += a[i * N + k] * b[k * N + j];
      }
    }
  }

  return res;
}

int64_t *kji_type(int64_t *a, int64_t *b, int32_t *size) { // 5
  int32_t N = *size;
  int64_t *res = (int64_t *)malloc(N * N * sizeof(int64_t));
  res = memset(res, 0, N * N * sizeof(int64_t));

  for (int32_t k = 0; k < N; k++) {
    for (int32_t j = 0; j < N; j++) {
      for (int32_t i = 0; i < N; i++) {
        res[i * N + j] += a[i * N + k] * b[k * N + j];
      }
    }
  }

  return res;
}

int64_t *input(FILE *out, int32_t *size) {
  char T;
  int byte_count;
  byte_count = fread(&T, sizeof(char), 1, out);
  int32_t N;
  byte_count = fread(&N, sizeof(int32_t), 1, out);
  size[0] = N;
  int64_t *data_in = (int64_t *)malloc(N * N * sizeof(int64_t));

  if (T == 'd') {
    for (int32_t i = 0; i < N; ++i) {
      for (int32_t j = 0; j < N; ++j) {
        int32_t num;
        byte_count = fread(&num, sizeof(int32_t), 1, out);
        data_in[i * N + j] = (int64_t)num;
      }
    }
  }

  if (T == 'l') {
    for (int32_t i = 0; i < N; ++i) {
      for (int32_t j = 0; j < N; ++j) {
        int64_t num;
        byte_count = fread(&num, sizeof(int64_t), 1, out);
        data_in[i * N + j] = (int64_t)num;
      }
    }
  }

  return data_in;
}

int main(int argc, char *argv[]) {
  if (argc != 5) {
    exit(1);
  }

  int64_t *a, *b, *c;
  int32_t *len = (int32_t *)malloc(sizeof(int32_t));

  FILE *out = fopen(argv[1], "rb");

  if (!out) {
    exit(1);
  }
  else {
    a = input(out, len);
  }

  fclose(out);
  out = fopen(argv[2], "rb");

  if (!out) {
    free(a);
    exit(1);
  } else {
    b = input(out, len);
  }

  out = fopen(argv[3], "rb");

  if (!out) {
    free(a);
    free(b);

    exit(1);
  } else {
    c = input(out, len);
  }

  int event_set = PAPI_NULL;
  long long values[4];
  int64_t *res_abc;
  int64_t *res_ab;
  double time = get_time();

  PAPI_library_init(PAPI_VER_CURRENT);
  PAPI_create_eventset(&event_set);
  PAPI_add_event(event_set, PAPI_L1_DCM);
  PAPI_add_event(event_set, PAPI_L2_DCM);
  PAPI_add_event(event_set, PAPI_L1_LDM);
  PAPI_add_event(event_set, PAPI_L1_STM);
  PAPI_start(event_set);

  int tmp;
  sscanf(argv[4], "%d", &tmp);
  switch (tmp) {
  case 0:
    res_ab = ijk_type(a, b, len);
    res_abc = ijk_type(res_ab, c, len);
    printf(" ijk ");
    break;
  case 1:
    res_ab = ikj_type(a, b, len);
    res_abc = ikj_type(res_ab, c, len);
    printf(" ikj ");
    break;
  case 2:
    res_ab = kij_type(a, b, len);
    res_abc = kij_type(res_ab, c, len);
    printf(" kij ");
    break;
  case 3:
    res_ab = jik_type(a, b, len);
    res_abc = jik_type(res_ab, c, len);
    printf(" jik ");
    break;
  case 4:
    res_ab = jki_type(a, b, len);
    res_abc = jki_type(res_ab, c, len);
    printf(" jki ");
    break;
  case 5:
    res_ab = kji_type(a, b, len);
    res_abc = kji_type(res_ab, c, len);
    printf(" kji ");
    break;
  }

  free(res_ab);
  
  PAPI_stop(event_set, values);
  printf(" %15lld ", values[0]);
  printf(" %15lld ", values[1]);
  printf(" %15lld ", values[2]);
  printf(" %15lld ", values[3]);

  printf(" %06.6fs ", get_time() - time);

  free(len);
  free(res_abc);
  free(a);
  free(b);
  free(c);

  return 0;
}
