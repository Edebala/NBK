#pragma once

#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>

class ConfusionMatrix
{
public:
	ConfusionMatrix(int _size)
	{
		size = _size;
		CM = (int**)malloc(size * sizeof(int*));
		for (int i = 0; i < size; ++i)
			CM[i] = (int*)calloc(size, sizeof(int));
		row = (int*)calloc(size, sizeof(int));
		col = (int*)calloc(size, sizeof(int));
		tpr = (float*)malloc(size * sizeof(float));
		ppv = (float*)malloc(size * sizeof(float));
		tnr = (float*)malloc(size * sizeof(float));
		dsc = (float*)malloc(size * sizeof(float));
		reset();
	};
	~ConfusionMatrix()
	{
		for (int i = 0; i < size; ++i)
			free(CM[i]);
		free(CM);
		free(row);
		free(col);
		free(tpr);
		free(ppv);
		free(tnr);
		free(dsc);
	};
	void reset()
	{
		for (int i = 0; i < size; ++i)
		{
			row[i] = 0;
			col[i] = 0;
			for (int j = 0; j < size; ++j)
				CM[i][j] = 0;
		}
		diag = 0;
	}
	void addCase(int gt, int deci)
	{
		if (gt >= 0 && gt < size && deci >= 0 && deci < size)
			CM[gt][deci]++;
	}
	void compute()
	{
		sum = 0;
		diag = 0;
		for (int i = 0; i < size; ++i)
		{
			row[i] = 0;
			col[i] = 0;
		}
		for (int gt = 0; gt < size; ++gt)
			for (int deci = 0; deci < size; ++deci)
			{
				row[gt] += CM[gt][deci];
				col[deci] += CM[gt][deci];
				sum += CM[gt][deci];
				if (gt == deci)
					diag += CM[gt][deci];
			}
		acc = (float)diag / (float)sum;
		for (int gt = 0; gt < size; ++gt)
		{
			tpr[gt] = (float)CM[gt][gt] / (float)row[gt];
			tnr[gt] = (float)(sum - row[gt] - col[gt] + CM[gt][gt]) / (float)(sum - row[gt]);
			dsc[gt] = (float)(2 * CM[gt][gt]) / (float)(row[gt] + col[gt]);
		}
		for (int deci = 0; deci < size; ++deci)
			ppv[deci] = (float)CM[deci][deci] / (float)(col[deci] > 0 ? col[deci] : 1);

		FILE* F;
		fopen_s(&F, "kungfu.txt", "at");
		fprintf(F, "\nConfusion matrix [CSF][GM][WM]\n");
		printf("\nConfusion matrix [CSF][GM][WM]\n");
		for (int gt = 0; gt < size; ++gt)
		{
			for (int deci = 0; deci < size; ++deci)
			{
				fprintf(F, "%8d", CM[gt][deci]);
				printf("%8d", CM[gt][deci]);
			}
			fprintf(F, "\n");
			printf("\n");
		}
		for (int gt = 0; gt < size; ++gt)
		{
			fprintf(F, "Class: %d, DSC=%.5f, TPR=%.5f, TNR=%.5f, PPV=%.5f\n", gt, dsc[gt], tpr[gt], tnr[gt], ppv[gt]);
			printf("Class: %d, DSC=%.5f, TPR=%.5f, TNR=%.5f, PPV=%.5f\n", gt, dsc[gt], tpr[gt], tnr[gt], ppv[gt]);
		}
		fprintf(F, "Global accuracy: ACC=%.5f\n", acc);
		printf("Global accuracy: ACC=%.5f\n", acc);
		fclose(F);
	}
private:
	int** CM;
	int size;
	int* row;
	int* col;
	int sum;
	int diag;
	float* tpr;
	float* ppv;
	float* tnr;
	float* dsc;
	float acc;
};
