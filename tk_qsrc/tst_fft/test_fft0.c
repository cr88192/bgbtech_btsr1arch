#include <stdio.h>
#include <math.h>
#include <complex.h>

// Standard PI value
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Generic FFT Function:
// - in: Input array of complex doubles
// - out: Output array of complex doubles
// - n: Size of FFT (must be power of 2)
// - is_inverse: 0 for FFT, 1 for Inverse FFT
void fft(double complex *in, double complex *out, int n, int is_inverse) {
	if (n == 1) {
		out[0] = in[0];
		return;
	}

	// Decimation in time
	for (int i = 0; i < n / 2; i++) {
		out[i] = in[2 * i];
		out[i + n / 2] = in[2 * i + 1];
	}

	// Recursive calls
	fft(out, in, n / 2, is_inverse);
	fft(out + n / 2, in, n / 2, is_inverse);

	// Butterfly operations
	for (int k = 0; k < n / 2; k++) {
		double p = -2 * M_PI * k / n * (is_inverse ? -1 : 1);
		double complex t = cexp(I * p) * in[k + n / 2];
		out[k] = in[k] + t;
		out[k + n / 2] = in[k] - t;
	}
}

int main() {
	int n = 4;
	double complex data[] = {1.0 + 0.0 * I, 1.0 + 0.0 * I, 1.0 + 0.0 * I, 1.0 + 0.0 * I};
	double complex result[4];

	// Compute FFT
	fft(data, result, n, 0);

	// Print results
	for (int i = 0; i < n; i++) {
		printf("Result[%d]: %.2f + %.2fi\n", i, creal(result[i]), cimag(result[i]));
	}
	return 0;
}

