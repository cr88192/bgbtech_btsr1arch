#include <math.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
	double re;
	double im;
} Complex;

/* Bit-reversal permutation */
void bit_reverse(Complex *data, int n)
{
	int i, j, bit;
	Complex temp;

	for (i = 1, j = 0; i < n; i++)
	{
		bit = n >> 1;
		for (; j & bit; bit >>= 1) {
			j ^= bit;
		}
		j ^= bit;

		if (i < j) {
			temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
	}
}

void fft(Complex *data, Complex *data_in, int n, int inverse)
{
	int len, i, j;
	double ang, wlen_re, wlen_im, w_re, w_im, next_re, inv_n;
	Complex u, v;

	for(i=0; i<n; i++)
		data[i] = data_in[i];

	bit_reverse(data, n);

	for (len = 2; len <= n; len <<= 1)
	{
		ang = 2.0 * M_PI / len * (inverse ? -1.0 : 1.0);
		wlen_re = cos(ang);
		wlen_im = sin(ang);

		for (i = 0; i < n; i += len) {
			w_re = 1.0;
			w_im = 0.0;
			for (j = 0; j < len / 2; j++) {
				u = data[i + j];
				
				/* Complex multiplication: v = data[idx] * w */
				v.re = data[i + j + len / 2].re * w_re - data[i + j + len / 2].im * w_im;
				v.im = data[i + j + len / 2].re * w_im + data[i + j + len / 2].im * w_re;

				/* Butterfly operations */
				data[i + j].re = u.re + v.re;
				data[i + j].im = u.im + v.im;
				data[i + j + len / 2].re = u.re - v.re;
				data[i + j + len / 2].im = u.im - v.im;

				/* Update twiddle factor: w = w * wlen */
				next_re = w_re * wlen_re - w_im * wlen_im;
				w_im = w_re * wlen_im + w_im * wlen_re;
				w_re = next_re;
			}
		}
	}

	if (inverse) {
		inv_n = 1.0 / n;
		for (i = 0; i < n; i++) {
			data[i].re *= inv_n;
			data[i].im *= inv_n;
		}
	}
}


int main()
{
	Complex tarr_i[512];
	Complex tarr_o[512];
	int i;
	
	for(i=0; i<512; i++)
	{
		tarr_i[i].re=rand()*(1.0/16384)-1.0;
		tarr_i[i].im=rand()*(1.0/16384)-1.0;
	}

	for(i=0; i<512; i++)
	{
		fft(tarr_o, tarr_i, 512, 0);
	}
}
