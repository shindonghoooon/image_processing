// DFT_2D (Image Processing)

#define CRTDBG_MAP_ALLOC
#include "DFT_2D_Sample.h"
#include <iostream>
#define HEIGHT 256
#define WIDTH 256

typedef unsigned char BYTE;
using namespace std;
enum Img_idx
{
	In,
	DFT_Real,
	IDFT_Real
};

void main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// File Open & Memory Allocation

	BYTE*** arr2D = new BYTE**[3];  	// 0 : arr2D_in, 1 = DFT_Re, 2 = IDFT_Re
	for (int n = 0; n < 3; n++)
	{
		arr2D[n] = MemAlloc2D<BYTE>(HEIGHT, WIDTH, 0);
	}

	double** dReal_2D = MemAlloc2D<double>(HEIGHT, WIDTH, 0);
	double** dImag_2D = MemAlloc2D<double>(HEIGHT, WIDTH, 0);
	
	FileRead("lena256.raw", arr2D[In], HEIGHT, WIDTH);

	// DFT

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			dReal_2D[i][j] = (double)arr2D[In][i][j];
			dImag_2D[i][j] = 0;
		}
	}

	DFT_2D(dReal_2D, dImag_2D, HEIGHT, WIDTH, DFT);
	ConvImage(arr2D[DFT_Real], dReal_2D, dImag_2D, HEIGHT, WIDTH, DFT);
	FileWrite("Lena_2D_DFT.raw", arr2D[DFT_Real], HEIGHT, WIDTH);

	// Filter Processing

	//LowPassFilter(dReal_2D, dImag_2D, HEIGHT, WIDTH, 30, LPF_GAUSSIAN);
	//HighPassFilter(dReal_2D, dImag_2D, HEIGHT, WIDTH, 30, HPF_GAUSSIAN);
	
	// IDFT

	DFT_2D(dReal_2D, dImag_2D, HEIGHT, WIDTH, IDFT);
	ConvImage(arr2D[IDFT_Real], dReal_2D, dImag_2D, HEIGHT, WIDTH, IDFT);
	FileWrite("Lena_2D_IDFT.raw", arr2D[IDFT_Real], HEIGHT, WIDTH);

	// Get SAD (������ 2-D Inverse DFT���� ��� ��)

	int nSAD = 0;

	for (int h = 0; h < HEIGHT; h++)
	{
		for (int w = 0; w < WIDTH; w++)
		{
			nSAD += arr2D[In][h][w] - arr2D[IDFT_Real][h][w];
		}
	}
	printf("SAD Between Original and IDFT_Real : %d\n\n", nSAD);
	//Memory Free
	
	for (int n = 0; n < 3; n++)
	{
		MemFree2D<BYTE>(arr2D[n], HEIGHT);
	}
	free(arr2D);

	MemFree2D<double>(dReal_2D, HEIGHT);
	MemFree2D<double>(dImag_2D, HEIGHT);
}