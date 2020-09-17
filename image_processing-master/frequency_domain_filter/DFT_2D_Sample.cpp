// DFT_2D (Image Processing)

#define CRTDBG_MAP_ALLOC

#include "DFT_2D_Sample.h"

#define HEIGHT 256
#define WIDTH 256

#define BUTTERWORTH_ORDER 2

typedef unsigned char BYTE;

template<typename T> T** MemAlloc2D(int nHeight, int nWidth, int nInitVal)
{
	T** rtn = new T*[nHeight];
	for (int h = 0; h < nHeight; h++)
	{
		rtn[h] = new T[nWidth];
		memset(rtn[h], nInitVal, sizeof(T) * nWidth);
	}
	return rtn;
}
template<typename T> void MemFree2D(T** arr2D, int nHeight)
{
	for (int h = 0; h < nHeight; h++)
	{
		delete[] arr2D[h];
	}
	delete[] arr2D;
}
void FileRead(const char* strFilename, BYTE** arr2D, int nHeight, int nWidth)
{
	FILE* fp_in = fopen(strFilename, "rb");
	for (int h = 0; h < nHeight; h++)
	{
		fread(arr2D[h], sizeof(BYTE), nWidth, fp_in);
	}

	fclose(fp_in);
}
void FileWrite(const char* strFilename, BYTE** arr2D, int nHeight, int nWidth)
{
	FILE* fp_out = fopen(strFilename, "wb");
	for (int h = 0; h < nHeight; h++)
	{
		fwrite(arr2D[h], sizeof(BYTE), nWidth, fp_out);
	}

	fclose(fp_out);
}
BYTE clip_d(double a)
{
	if (a > 255)
	{
		return (BYTE)255;
	}
	if (a < 0)
	{
		return 0;
	}
	else return (BYTE)floor(a + 0.5);
}

void DFT_2D(double **dReal_2D, double **dImag_2D, int nHeight, int nWidth, int DFT_ID)
{
	double* Arr_real = new double[nWidth];
	double* Arr_imag = new double[nWidth];
	memset(Arr_real, 0, sizeof(double) * nWidth);
	memset(Arr_imag, 0, sizeof(double) * nWidth);

	double* Arr_real_h = new double[nHeight];
	double* Arr_imag_h = new double[nHeight];
	memset(Arr_real_h, 0, sizeof(double) * nHeight);
	memset(Arr_imag_h, 0, sizeof(double) * nHeight);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			Arr_real[w] = dReal_2D[h][w];
			Arr_imag[w] = dImag_2D[h][w];
		}
		DFT_1D(Arr_real, Arr_imag, nWidth, DFT_ID);
		for (int w = 0; w < nWidth; w++) {
			dReal_2D[h][w] = Arr_real[w];
			dImag_2D[h][w] = Arr_imag[w];
		}
	}
	for (int w = 0; w < nWidth; w++) {
		for (int h = 0; h < nHeight; h++) {
			Arr_real_h[h] = dReal_2D[h][w];
			Arr_imag_h[h] = dImag_2D[h][w];
		}
		DFT_1D(Arr_real_h, Arr_imag_h, nHeight, DFT_ID);
		for (int h = 0; h < nHeight; h++) {
			dReal_2D[h][w] = Arr_real_h[h];
			dImag_2D[h][w] = Arr_imag_h[h];
		}
	}
	delete[] Arr_real;
	delete[] Arr_imag;
	delete[] Arr_real_h;
	delete[] Arr_imag_h;
}
void DFT_1D(double* dReal_1D, double* dImag_1D, int nLength, int DFT_ID)
{
	double dArg, dCosArg, dSinArg;

	double *dTemp_R = new double[nLength];
	double *dTemp_I = new double[nLength];

	memset(dTemp_R, 0, sizeof(double) * nLength);
	memset(dTemp_I, 0, sizeof(double) * nLength);

	for (int m = 0; m < nLength; m++)
	{
		dTemp_R[m] = 0;
		dTemp_I[m] = 0;
		dArg = DFT_ID * 2.0 * PI * (double)m / (double)nLength;
		for (int n = 0; n < nLength; n++)
		{
			dCosArg = cos(n * dArg);
			dSinArg = sin(n * dArg);

			dTemp_R[m] += (dReal_1D[n] * dCosArg - dImag_1D[n] * dSinArg);
			dTemp_I[m] += (dReal_1D[n] * dSinArg + dImag_1D[n] * dCosArg);
		}
	}

	if (DFT_ID == IDFT)
	{
		for (int m = 0; m < nLength; m++)
		{
			dReal_1D[m] = dTemp_R[m] / (double)nLength;
			dImag_1D[m] = dTemp_I[m] / (double)nLength;
		}
	}
	else
	{
		for (int m = 0; m < nLength; m++)
		{
			dReal_1D[m] = dTemp_R[m];
			dImag_1D[m] = dTemp_I[m];
		}
	}

	delete[] dTemp_R;
	delete[] dTemp_I;

}
void ConvImage(BYTE** arr2D, double** dReal_2D, double** dImag_2D, int nHeight, int nWidth, int DFT_ID)
{
	double dMax = 0, dNor, dMean = 0;
	double** dTemp_2D = MemAlloc2D<double>(nHeight, nWidth, 0);

	for (int h = 0; h < nHeight; h++)
	{
		memcpy(dTemp_2D[h], dReal_2D[h], sizeof(double) * nWidth);
	}

	if (DFT_ID == DFT)
	{
		for (int h = 0; h < nHeight; h++)
		{
			for (int w = 0; w < nWidth; w++)
			{
				double dTemp = sqrt(dReal_2D[h][w] * dReal_2D[h][w] + dImag_2D[h][w] * dImag_2D[h][w]);
				dTemp_2D[h][w] = log10(dTemp + 1);

				if (dMax < dTemp_2D[h][w])
				{
					dMax = dTemp_2D[h][w];
				}
			}
		}

		dNor = 255 / dMax;
	}
	else
	{
		dNor = 1;
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			double dTemp = (dTemp_2D[h][w] * dNor);
			arr2D[h][w] = clip_d(dTemp);
		}
	}
	if (DFT_ID == DFT)
	{
		SetCenterDC<BYTE>(arr2D, nHeight, nWidth);
	}

	MemFree2D<double>(dTemp_2D, nHeight);
}
template <typename T> void SetCenterDC(T** arr2D, int nHeight, int nWidth)
{
	int nHalf_H = nHeight / 2, nHalf_W = nWidth / 2;

	T** arr2DTemp = MemAlloc2D<T>(nHeight, nWidth, 0);

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			arr2DTemp[h][w] = arr2D[h][w];
		}
	}

	for (int h = 0; h < nHalf_H; h++)
	{
		for (int w = 0; w < nHalf_W; w++)
		{
			arr2D[h][w] = arr2DTemp[h + nHalf_H][w + nHalf_W];
			arr2D[h + nHalf_H][w] = arr2DTemp[h][w + nHalf_W];
			arr2D[h][w + nHalf_W] = arr2DTemp[h + nHalf_H][w];
			arr2D[h + nHalf_H][w + nHalf_W] = arr2DTemp[h][w];
		}
	}

	MemFree2D<T>(arr2DTemp, nHeight);
}

void LowPassFilter(double** dReal2D, double** dImag2D, int nHeight, int nWidth, int nThres, int nFilterType)
{
	int nHalf_H = nHeight / 2, nHalf_W = nWidth / 2;

	double** dFilter = MemAlloc2D<double>(nHeight, nWidth, 0);

	SetCenterDC<double>(dReal2D, nHeight, nWidth);
	SetCenterDC<double>(dImag2D, nHeight, nWidth);

	// Make Filter
	switch (nFilterType)
	{
	case LPF_IDEAL:
		for (int h = 0; h < nHeight; h++) {
			for (int w = 0; w < nWidth; w++) {				
				if (sqrt(pow(h - nHalf_H, 2) + pow(w - nHalf_W, 2)) <= nThres)
					dFilter[h][w] = 1;
				else
					dFilter[h][w] = 0;
			}
		}
		break;
	case LPF_BUTTERWORTH:
		for (int h = 0; h < nHeight; h++) {
			for (int w = 0; w < nWidth; w++) {
				dFilter[h][w] = 1.0 / (1.0 + (pow(sqrt(pow(h-nHalf_H, 2) + pow(w-nHalf_W, 2)) / nThres, 2)));
			}
		}
		break;
	case LPF_GAUSSIAN:
		for (int h = 0; h < nHeight; h++) {
			for (int w = 0; w < nWidth; w++) {
				dFilter[h][w] = exp(-pow(sqrt(pow(h - nHalf_H, 2) + pow(w - nHalf_W, 2)), 2) / (2.0*pow(nThres, 2)));
			}
		}
		break;
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			dReal2D[h][w] = dReal2D[h][w] * dFilter[h][w];
			dImag2D[h][w] = dImag2D[h][w] * dFilter[h][w];
		}
	}
	SetCenterDC<double>(dReal2D, nHeight, nWidth);
	SetCenterDC<double>(dImag2D, nHeight, nWidth);

	MemFree2D<double>(dFilter, nHeight);
}
void HighPassFilter(double** dReal2D, double** dImag2D, int nHeight, int nWidth, int nThres, int nFilterType)
{
	int nHalf_H = nHeight / 2, nHalf_W = nWidth / 2;

	double** dFilter = MemAlloc2D<double>(nHeight, nWidth, 0);

	SetCenterDC<double>(dReal2D, nHeight, nWidth);
	SetCenterDC<double>(dImag2D, nHeight, nWidth);

	// Make Filter
	switch (nFilterType)
	{
	case HPF_IDEAL:
		for (int h = 0; h < nHeight; h++) {
			for (int w = 0; w < nWidth; w++) {
				if (sqrt(pow(h - nHalf_H, 2) + pow(w - nHalf_W, 2)) <= nThres)
					dFilter[h][w] = 0;
				else
					dFilter[h][w] = 1;
			}
		}
		break;
	case HPF_BUTTERWORTH:
		for (int h = 0; h < nHeight; h++) {
			for (int w = 0; w < nWidth; w++) {
				dFilter[h][w] = 1.0 / (1.0 + (pow(nThres / sqrt(pow(h - nHalf_H, 2) + pow(w - nHalf_W, 2)), 2)));
			}
		}
		break;
	case HPF_GAUSSIAN:
		for (int h = 0; h < nHeight; h++) {
			for (int w = 0; w < nWidth; w++) {
				dFilter[h][w] = 1 - exp(-pow(sqrt(pow(h - nHalf_H, 2) + pow(w - nHalf_W, 2)), 2) / (2.0*pow(nThres, 2)));
			}
		}
		break;
	}

	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			dReal2D[h][w] = dReal2D[h][w] * dFilter[h][w];
			dImag2D[h][w] = dImag2D[h][w] * dFilter[h][w];
		}
	}
	SetCenterDC<double>(dReal2D, nHeight, nWidth);
	SetCenterDC<double>(dImag2D, nHeight, nWidth);
}