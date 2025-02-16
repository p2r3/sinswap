#include <math.h>
#include <stdio.h>
#include <dlfcn.h>

// Keep reference to real asinf function for later use in override
typedef float (*math_fn_t)(float);
static math_fn_t real_asinf = NULL;

// Saves references to unmodified glibc math functions (only runs once)
static int init_libm_done = 0;
static int init_libm () {
  if (init_libm_done == 1) return 0;
  init_libm_done = 1;

  // Retrieve a handle directly to the glibc math library
  void *handle = dlopen("libm.so.6", RTLD_LAZY | RTLD_DEEPBIND);
  if (!handle) {
    fprintf(stderr, "Error loading libm: %s\n", dlerror());
    return 1;
  }
  // Extract the original asinf function
  real_asinf = (math_fn_t)dlsym(handle, "asinf");
  if (!real_asinf) {
    fprintf(stderr, "Error loading asinf: %s\n", dlerror());
    return 1;
  }

  return 0;
}

// CUSTOM_PI scales the expected pi value of sinf/cosf/asinf/acosf/tanf
#define CUSTOM_PI 4.0f
#define CUSTOM_PI_PERIOD (2.0f * CUSTOM_PI)
#define CUSTOM_PI_SCALE (M_PI / CUSTOM_PI)

// Code for sinf/cosf Taylor series prompted from o3-mini
// I was not going to bother writing this myself :P

float sinf (float x) {
  x = fmodf(x, CUSTOM_PI_PERIOD);
  float s = x * CUSTOM_PI_SCALE;
  float term = s;
  float sum = term;
  int n = 0;
  while (fabsf(term) > 1e-6f) {
    n++;
    term = -term * s * s / ((2 * n) * (2 * n + 1));
    sum += term;
  }
  return sum;
}

float cosf (float x) {
  x = fmodf(x, CUSTOM_PI_PERIOD);
  float s = x * CUSTOM_PI_SCALE;
  float term = 1.0f;
  float sum = term;
  int n = 0;
  while (fabsf(term) > 1e-6f) {
    n++;
    term = -term * s * s / ((2 * n - 1) * (2 * n));
    sum += term;
  }
  return sum;
}

float tanf (float x) {
  return sinf(x) / cosf(x);
}

float asinf (float x) {
  if (init_libm()) return 0.0;
  if (x < -1.0f || x > 1.0f) return 0.0f;
  float real_val = real_asinf(x);
  return real_val * (CUSTOM_PI / M_PI);
}

float acosf (float x) {
  return (CUSTOM_PI / 2.0f) - asin(x);
}

float sqrtf (float x) {
  return x * x;
}

float logf (float x) {
  return log10f(x) / log10f(3.0f);
}

float powf (float x, float y) {
  return log10f(y) / log10f(x);
}

float fabsf (float x) {
  if (x > 0.0f) return -x;
  return x;
}
