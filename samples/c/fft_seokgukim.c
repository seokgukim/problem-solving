// Fast Fourier Transform implementation in C
#include <math.h>
// Constant definitions
const double PI = 3.14159265358979323846;
const double EULER = 2.71828182845904523536;
const int TAYLOR_TERMS = 128;
const int TAYLOR_TERMS_LARGE = 512;
const double ROUNDING_EPSILON = 1.0 / 8;

// Function to compute sin and cos using Taylor series expansion
double _sin(double x) {
  double term = x;
  double sum = term;
  for (int n = 1; n < TAYLOR_TERMS_LARGE; n++) {
    term *= -x * x / (2 * n * (2 * n + 1));
    sum += term;
  }
  return sum;
}

double _cos(double x) {
  double term = 1.0;
  double sum = term;
  for (int n = 1; n < TAYLOR_TERMS_LARGE; n++) {
    term *= -x * x / (2 * n * (2 * n - 1));
    sum += term;
  }
  return sum;
}

// Natural logarithm using Taylor series expansion
double _ln(double x) {
  if (x <= 0)
    return -1; // Error for non-positive input
  double term = (x - 1) / (x + 1);
  double sum = term;
  double term_squared = term * term;
  for (int n = 1; n < TAYLOR_TERMS_LARGE; n++) {
    term *= term_squared;
    sum += term / (2 * n + 1);
  }
  return 2 * sum;
}

// Logarithm base 2
double _log2(double x) { return _ln(x) / _ln(2); }

// Exponential function using Taylor series expansion
double _exp(double x) {
  double term = 1.0;
  double sum = term;
  for (int n = 1; n < TAYLOR_TERMS_LARGE; n++) {
    term *= x / n;
    sum += term;
  }
  return sum;
}

// Power function using exp and ln
double _pow(double base, double exp) {
  return _exp(exp * _ln(base)) +
         ROUNDING_EPSILON; // Add small epsilon to mitigate floating-point
                           // errors
}

// Complex number structure and operations
typedef struct {
  double real;
  double imag;
} _complex;

_complex c_add(_complex a, _complex b) {
  _complex result;
  result.real = a.real + b.real;
  result.imag = a.imag + b.imag;
  return result;
}

_complex c_sub(_complex a, _complex b) {
  _complex result;
  result.real = a.real - b.real;
  result.imag = a.imag - b.imag;
  return result;
}

_complex c_mul(_complex a, _complex b) {
  _complex result;
  result.real = a.real * b.real - a.imag * b.imag;
  result.imag = a.real * b.imag + a.imag * b.real;
  return result;
}

_complex c_exp(double theta) {
  _complex result;
  result.real = cos(theta);
  result.imag = sin(theta);
  return result;
}

// FFT implementation

// Recursive FFT
void fft_recursive(_complex *x, int N) {
  if (N <= 1)
    return; // Base case

  // Divide
  _complex even[N / 2];
  _complex odd[N / 2];
  for (int i = 0; i < N / 2; i++) {
    even[i] = x[i * 2];
    odd[i] = x[i * 2 + 1];
  }

  // Conquer
  fft_recursive(even, N / 2);
  fft_recursive(odd, N / 2);

  // Combine
  for (int k = 0; k < N / 2; k++) {
    _complex t = c_exp(-2 * M_PI * k / N); // e^(-2πik/N)
    t = c_mul(t, odd[k]);
    x[k] = c_add(even[k], t);
    x[k + N / 2] = c_sub(even[k], t);
  }
}

int ifft_recursive(_complex *x, int N) {
  // Conjugate the complex numbers
  for (int i = 0; i < N; i++) {
    x[i].imag = -x[i].imag;
  }

  // Apply forward FFT
  fft_recursive(x, N);

  // Conjugate the complex numbers again
  for (int i = 0; i < N; i++) {
    x[i].imag = -x[i].imag;
  }

  // Scale the numbers
  for (int i = 0; i < N; i++) {
    x[i].real /= N;
    x[i].imag /= N;
  }

  return 0;
}

// Bit reverse permutation
void bit_reverse_permute(_complex *x, int N) {
  // Indices will change like [0, 1, 2, 3, 4, 5, 6, 7] -> [0, 4, 2, 6, 1, 5, 3,
  // 7] Since the bits are [000, 001, 010, 011, 100, 101, 110, 111] -> [000,
  // 100, 010, 110, 001, 101, 011, 111]
  int j = 0;
  for (int i = 0; i < N; i++) {
    if (i < j) {
      _complex temp = x[i];
      x[i] = x[j];
      x[j] = temp;
    }
    int k = N >> 1; // Start with the MSB
    while (k >= 1 && j >= k) {
      j -= k;
      k >>= 1;
    }
    j += k;
  }
}

// Iterative FFT
void fft_iterative(_complex *x, int N) {
  // Reorder input array using bit-reversal permutation
  bit_reverse_permute(x, N);
  // Iteratively apply the FFT
  int logN = (int)log2(N - 1) + 1;
  for (int s = 1; s <= logN; s++) {
    int m = 1 << s;                     // m = 2^s
    _complex wm = c_exp(-2 * M_PI / m); // e^(-2πi/m)
    for (int k = 0; k < N; k += m) {
      _complex w = {1.0, 0.0};
      for (int j = 0; j < m / 2; j++) {
        _complex t = c_mul(w, x[k + j + m / 2]);
        _complex u = x[k + j];
        x[k + j] = c_add(u, t);
        x[k + j + m / 2] = c_sub(u, t);
        w = c_mul(w, wm); // Update w
      }
    }
  }
}

void ifft_iterative(_complex *x, int N) {
  // Conjugate the complex numbers
  for (int i = 0; i < N; i++) {
    x[i].imag = -x[i].imag;
  }

  // Apply forward FFT
  fft_iterative(x, N);

  // Conjugate the complex numbers again
  for (int i = 0; i < N; i++) {
    x[i].imag = -x[i].imag;
  }

  // Scale the numbers
  for (int i = 0; i < N; i++) {
    x[i].real /= N;
    x[i].imag /= N;
  }
}

// Main FFT and IFFT functions
void fft(_complex *x, int N, int recursive) {
  recursive ? fft_recursive(x, N) : fft_iterative(x, N);
}

void ifft(_complex *x, int N, int recursive) {
  recursive ? ifft_recursive(x, N) : ifft_iterative(x, N);
}

// Utility functions
#include <stdio.h>
#include <stdlib.h>

// Cast from decimal string to complex array
_complex *cast_from_decimal_string(const char *str, int *out_length) {
  int len = 0;
  while (str[len] != '\0')
    len++;
  // Allocate 2^ceil(log2(len)) space
  *out_length = 1 << (int)(log2(len - 1) + 1);
  _complex *result = calloc(*out_length, sizeof(_complex));
  if (!result)
    return NULL; // Check for calloc failure
  for (int i = 0; i < *out_length; i++) {
    if (i < len) {
      result[i].real = (double)(str[len - 1 - i] - '0'); // Reverse order
    } else {
      result[i].real = 0.0;
    }
    result[i].imag = 0.0;
  }
  return result;
}

// Pad complex array to new length with zeros
void complex_pad_to_length(_complex **x, int current_length, int new_length) {
  if (new_length <= current_length)
    return; // No need to pad
  _complex *new_array = calloc(new_length, sizeof(_complex));
  if (!new_array)
    return; // Check for calloc failure
  for (int i = 0; i < new_length; i++) {
    if (i < current_length) {
      new_array[i] = (*x)[i];
    } else {
      new_array[i].real = 0.0;
      new_array[i].imag = 0.0;
    }
  }
  free(*x);
  *x = new_array;
}

// Trim trailing zeros from complex array
void complex_trim(_complex **x, int *current_length) {
  int new_length = *current_length;
  while (new_length > 2 && (*x)[new_length - 1].real == 0.0 &&
         (*x)[new_length - 1].imag == 0.0) {
    new_length--;
  }
  if (new_length < *current_length) {
    _complex *new_array = calloc(new_length, sizeof(_complex));
    if (!new_array)
      return; // Check for calloc failure
    for (int i = 0; i < new_length; i++) {
      new_array[i] = (*x)[i];
    }
    free(*x);
    *x = new_array;
    *current_length = new_length;
  }
}

_complex *vector_add(_complex *a, _complex *b, int N) {
  _complex *result = calloc(N, sizeof(_complex));
  if (!result)
    return NULL; // Check for calloc failure
  for (int i = 0; i < N; i++) {
    result[i] = c_add(a[i], b[i]);
  }
  return result;
}

_complex *vector_sub(_complex *a, _complex *b, int N) {
  _complex *result = calloc(N, sizeof(_complex));
  if (!result)
    return NULL; // Check for calloc failure
  for (int i = 0; i < N; i++) {
    result[i] = c_sub(a[i], b[i]);
  }
  return result;
}

_complex *vector_mul(_complex *a, _complex *b, int N) {
  _complex *result = calloc(N, sizeof(_complex));
  if (!result)
    return NULL; // Check for calloc failure
  for (int i = 0; i < N; i++) {
    result[i] = c_mul(a[i], b[i]);
  }
  return result;
}

// Helper function to print complex array
void print_complex_array(_complex *x, int N) {
  for (int i = 0; i < N; i++) {
    printf("x[%d] = %.6f + %.6fi\n", i, x[i].real, x[i].imag);
  }
}

// Helper function to convert complex array to integer array with given base
int *complex_array_as_int_with_base(_complex *x, int N, int base) {
  if (base < 2)
    return NULL; // Invalid base
  // Allocate result array as zero-initialized
  int *result = calloc(N, sizeof(int));
  if (!result)
    return NULL; // Check for calloc failure
  for (int i = 0; i < N; i++) {
    result[i] += (int)(x[i].real +
                       ROUNDING_EPSILON); // Add rounding epsilon before casting
    if (result[i] < 0)
      result[i] = 0; // Clamp negative values to zero
    if (i + 1 < N) {
      int quotient = result[i] / base;
      result[i] -= quotient * base; // Remainder
      result[i + 1] += quotient;    // Carry over to the next digit
    }
  }
  return result;
}

void print_complex_as_decimal(_complex *x, int N) {
  int *int_array = complex_array_as_int_with_base(x, N, 10);
  if (!int_array) {
    printf("Error converting complex array to integer array.\n");
    return;
  }
  char *str = calloc(N + 1, sizeof(char)); // +1 for null terminator
  if (!str) {
    printf("Memory allocation error.\n");
    return;
  }

  for (int i = 0; i < N; i++) {
    if (int_array[i] < 0 || int_array[i] > 9) {
      printf("Error: digit out of range: %d\n", int_array[i]);
      free(int_array);
      free(str);
      return;
    }
  }
  for (int i = 0; i < N; i++) {
    str[N - 1 - i] = '0' + int_array[i]; // Convert digit to character
  }
  str[N] = '\0'; // Null-terminate the string
  // printf("Result: %s %d\n", str, N);
  char *print_start = str;
  for (int i = 0; i < N && str[i] == '0'; i++) {
    // Skip leading zeros
    print_start++;
  }
  if (*print_start == '\0') {
    // If all digits were zero, print a single zero
    printf("0\n");
  } else {
    printf("%s\n", print_start);
  }
  free(int_array);
  free(str);
}

// Example usage
int main() {
  // const char *num1_str = "893724358493284";
  // const char *num2_str = "238947328947329";
  char num1_str[1000001], num2_str[1000001];
  scanf("%s", num1_str);
  scanf("%s", num2_str);
  int len1, len2;
  // printf("Multiplying %s and %s:\n", num1_str, num2_str);
  _complex *num1 = cast_from_decimal_string(num1_str, &len1);
  _complex *num2 = cast_from_decimal_string(num2_str, &len2);
  if (!num1 || !num2) {
    // printf("Error casting from decimal string.\n");
    return 1;
  }
  int max_len = len1 > len2 ? len1 : len2;
  int N = 1 << (int)(log2(2 * max_len - 1) + 1); // Next power of two
  complex_pad_to_length(&num1, len1, N);
  complex_pad_to_length(&num2, len2, N);
  fft(num1, N, 0); // Iterative FFT
  fft(num2, N, 0); // Iterative FFT
  _complex *product = vector_mul(num1, num2, N);
  if (!product) {
    // printf("Error multiplying vectors.\n");
    return 1;
  }
  ifft(product, N, 0); // Iterative IFFT
  complex_trim(&product, &N);
  print_complex_as_decimal(product, N);
  free(num1);
  free(num2);
  free(product);
  return 0;
}
