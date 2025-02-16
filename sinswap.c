#include <dlfcn.h>
#include <math.h>
#include <stdio.h>

typedef float (*math_fn_t)(float);

static math_fn_t real_sinf = NULL;
static math_fn_t real_cosf = NULL;
static math_fn_t real_asinf = NULL;
static math_fn_t real_acosf = NULL;

// Saves references to unmodified glibc math functions (only runs once)
static int init_libm_done = 0;
static int init_libm () {
  if (init_libm_done == 1) return 0;
  init_libm_done = 1;

  void *handle = dlopen("libm.so.6", RTLD_LAZY | RTLD_DEEPBIND);
  if (!handle) {
    fprintf(stderr, "Error loading libm: %s\n", dlerror());
    return 1;
  }
  real_acosf = (math_fn_t)dlsym(handle, "acosf");
  if (!real_acosf) {
    fprintf(stderr, "Error loading acosf: %s\n", dlerror());
    return 1;
  }
  real_asinf = (math_fn_t)dlsym(handle, "asinf");
  if (!real_asinf) {
    fprintf(stderr, "Error loading asinf: %s\n", dlerror());
    return 1;
  }
  real_cosf = (math_fn_t)dlsym(handle, "cosf");
  if (!real_cosf) {
    fprintf(stderr, "Error loading cosf: %s\n", dlerror());
    return 1;
  }
  real_sinf = (math_fn_t)dlsym(handle, "sinf");
  if (!real_sinf) {
    fprintf(stderr, "Error loading sinf: %s\n", dlerror());
    return 1;
  }

  return 0;
}

// Swap sinf with cosf, asinf with acosf using saved references
// Should be fairly self-explanatory

float sinf (float x) {
  if (init_libm()) return 0.0f;
  return real_cosf(x);
}

float cosf (float x) {
  if (init_libm()) return 0.0f;
  return real_sinf(x);
}

float asinf (float x) {
  if (init_libm()) return 0.0f;
  return real_acosf(x);
}

float acosf (float x) {
  if (init_libm()) return 0.0f;
  return real_asinf(x);
}
