//#include "opencv2/opencv.hpp"
//#include "opencv2/ml.hpp"
#include "iris.h"
#include "ConfusionMatrix.h"
//#include "HistNorm.h"

//using namespace cv;
//using namespace ml;

void testCM()
{
	const int size = 3;
	ConfusionMatrix cm(size);
	cm.reset();
	for (int o = 0; o < 10000; ++o)
		cm.addCase(rand() % size, rand() % size);
	cm.compute();
}


void myKNN_iris(int kKNN, int _wantPerm = 1, int _wantNorm = 0)
{
	const float bigNumber = 10000.0f;
	const int nrData = 150;
	const int nrFeat = 4;
	const int slices = 5;
	const int nrClasses = 3;

	float niris[nrData][nrFeat];
	float maxi[nrFeat];
	float mini[nrFeat];
	for (int f = 0; f < nrFeat; ++f)
	{
		maxi[f] = iris[0][f];
		mini[f] = iris[0][f];
		for (int i = 1; i < nrData; ++i)
		{
			if (iris[i][f] > maxi[f]) maxi[f] = iris[i][f];
			if (iris[i][f] < mini[f]) mini[f] = iris[i][f];
		}
		printf("%.1f %.1f\n", mini[f], maxi[f]);
		for (int i = 0; i < nrData; ++i)
			if (_wantNorm)
				niris[i][f] = (iris[i][f] - mini[f]) / (maxi[f] - mini[f]);
			else
				niris[i][f] = iris[i][f];
	}

	int KUKA[nrData];
	int PERMUTE[nrData];
	for (int i = 0; i < nrData; ++i)
	{
		KUKA[i] = i;
		PERMUTE[i] = i;
	}
	if (_wantPerm)
		for (int i = 0; i < nrData; ++i)
		{
			int r = rand() % (nrData - i);
			PERMUTE[i] = KUKA[r];
			KUKA[r] = KUKA[nrData - i - 1];
		}

	float dist[nrData][nrData] = { 0 };
	for (int i=0; i<nrData; ++i)
		for (int j = i; j < nrData; ++j)
		{
			for (int f = 0; f < nrFeat; ++f)
				dist[i][j] += ((niris[i][f] - niris[j][f]) * (niris[i][f] - niris[j][f]));
			dist[j][i] = dist[i][j];
		}

	ConfusionMatrix cm(nrClasses);
	float buffer[nrData];
	for (int which = 0; which < slices; ++which)
	{
		for (int id = which; id < nrData; id += slices)
		{
			int votes[nrClasses] = { 0 };
			for (int i = 0; i < nrData; ++i)
				if (i % slices == which)
					buffer[i] = bigNumber;
				else
					buffer[i] = dist[PERMUTE[id]][PERMUTE[i]];
			for (int k = 0; k < kKNN; ++k)
			{
				int index = 0;
				for (int i = 1; i < nrData; ++i)
					if (buffer[i] < buffer[index]) index = i;
				buffer[index] = bigNumber;
				votes[PERMUTE[index]/50]++;
			}
			int deci = 0;
			for (int i = 1; i < nrClasses; ++i)
				if (votes[i] > votes[deci]) deci = i;

			printf("%5d", votes[deci]);
			cm.addCase(PERMUTE[id]/50, deci);
		}
		printf("\n");
		cm.compute();
	}
}

void myKNN_wine(int kKNN, int _wantPerm = 1, int _wantNorm = 1)
{
	const float bigNumber = 100000000.0f;
	const int nrData = 178;
	const int nrFeat = 13;
	const int slices = 5;
	const int nrClasses = 3;

	int gt[nrData];
	float niris[nrData][nrFeat];
	float maxi[nrFeat];
	float mini[nrFeat];
	for (int f = 0; f < nrFeat; ++f)
	{
		maxi[f] = wine[0][f+1];
		mini[f] = wine[0][f+1];
		for (int i = 1; i < nrData; ++i)
		{
			if (wine[i][f+1] > maxi[f]) maxi[f] = wine[i][f+1];
			if (wine[i][f+1] < mini[f]) mini[f] = wine[i][f+1];
		}
		printf("%.1f %.1f\n", mini[f], maxi[f]);
		for (int i = 0; i < nrData; ++i)
			if (_wantNorm)
				niris[i][f] = (wine[i][f+1] - mini[f]) / (maxi[f] - mini[f]);
			else
				niris[i][f] = wine[i][f+1];
	}

	int KUKA[nrData];
	int PERMUTE[nrData];
 	for (int i = 0; i < nrData; ++i)
	{
		gt[i] = wine[i][0] - 1;
		KUKA[i] = i;
		PERMUTE[i] = i;
	}
	if (_wantPerm)
		for (int i = 0; i < nrData; ++i)
		{
			int r = rand() % (nrData - i);
			PERMUTE[i] = KUKA[r];
			KUKA[r] = KUKA[nrData - i - 1];
		}

	float dist[nrData][nrData] = { 0 };
	for (int i = 0; i < nrData; ++i)
		for (int j = i; j < nrData; ++j)
		{
			for (int f = 0; f < nrFeat; ++f)
				dist[i][j] += ((niris[i][f] - niris[j][f]) * (niris[i][f] - niris[j][f]));
			dist[j][i] = dist[i][j];
		}

	ConfusionMatrix cm(nrClasses);
	float buffer[nrData];
	for (int which = 0; which < slices; ++which)
	{
		for (int id = which; id < nrData; id += slices)
		{
			int votes[nrClasses] = { 0 };
			for (int i = 0; i < nrData; ++i)
				if (i % slices == which)
					buffer[i] = bigNumber;
				else
					buffer[i] = dist[PERMUTE[id]][PERMUTE[i]];
			for (int k = 0; k < kKNN; ++k)
			{
				int index = 0;
				for (int i = 1; i < nrData; ++i)
					if (buffer[i] < buffer[index]) index = i;
				buffer[index] = bigNumber;
				votes[gt[PERMUTE[index]]]++;
			}
			int deci = 0;
			for (int i = 1; i < nrClasses; ++i)
				if (votes[i] > votes[deci]) deci = i;

			printf("%5d", votes[deci]);
			cm.addCase(gt[PERMUTE[id]], deci);
		}
		printf("\n");
		cm.compute();
	}
}


void main()
{

	srand(time(0));
	myKNN_wine(9);
	_getch();
	return;
}
