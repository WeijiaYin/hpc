// MatrixMultiplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <mpi.h>

int countLine(char *inputFile)
{
	FILE * fp = NULL;
	int c, lc = 0;
	int line = 0;

	fp = fopen(inputFile, "r+");
	if (fp == NULL)
	{
		printf("There is no file:%s", inputFile);
		exit(1);
	}
	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '\n') line++;
		lc = c;
	}
	if (lc != '\n')
	{
		line++;
		fprintf(fp, "%c", '\n');
	}
	fclose(fp);
	return line;
}

int* readMatrixFromFile(char *inputFile, int p, const int lineNum) {
	FILE *f = NULL;
	int** matrix;
	int i = 0, j = 0, block = 0, k = 0, t = 0;
	char *result;
	char line[1024];
	int* matrixre;

	matrix = new int*[lineNum];
	for (i = 0; i < lineNum; i++)
	{
		matrix[i] = new int[lineNum];
	}
	matrixre = (int *)malloc(lineNum * lineNum * sizeof(int));
	f = fopen(inputFile, "r+");
	for (i = 0; i < lineNum; i++)
	{
		fgets(line, sizeof(line), f);
		result = strtok(line, ",");
		matrix[i][0] = atoi(result);
		for (j = 1; j < lineNum; j++) {
			result = strtok(NULL, ",");
			matrix[i][j] = atoi(result);
		}
	}
	block = lineNum / p;
	for (i = 0; i < p; i++)
	{
		for (j = 0; j < lineNum; j++)
		{
			for (k = i * block; k < i * block + block; k++)
			{
				matrixre[t] = matrix[j][k];
				t++;
			}
		}
	}

	return matrixre;
}

int main()
{
	int lineNum = countLine("a.txt");
	int *a = readMatrixFromFile("a.txt", 4, lineNum);
	for (int i = 0; i < lineNum*lineNum; i++)
	{
		printf("%d ", a[i]);
	}
    return 0;
}

