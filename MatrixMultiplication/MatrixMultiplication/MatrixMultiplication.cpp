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

int* readMatrixAFromFile(char *inputFile, int p, int lineNum) {
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
	free(matrix);
	return matrixre;
}

int** readMatrixBFromFile(char *inputFile, int lineNum) {
	FILE *f = NULL;
	int** matrix;
	int i = 0, j = 0;
	char *result;
	char line[1024];

	matrix = new int*[lineNum];
	for (i = 0; i < lineNum; i++)
	{
		matrix[i] = new int[lineNum];
	}

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

	return matrix;
}

int **getSubB(int pNum, int p, int lineNum, int time, int **matrixB)
{
	int **matrix = NULL;
	int i = 0 ,block = 0, j = 0, k = 0, t = 0, q= 0;

	block = lineNum / p;
	matrix = new int*[block];
	for (i = 0; i < block; i++)
	{
		matrix[i] = new int[block];
	}
	for (j = ((time + pNum) % p) * block; j <((time + pNum) % p) * block + block; j++)
	{
		for (k = pNum*block; k < pNum*block + block; k++)
		{
			matrix[t][q] = matrixB[j][k];
			q++;
		}
		q = 0;
		t++;
	}
	return matrix;
}

int *submatrixBs(int p, int lineNum, int time, int **matrixB)
{
	int block = 0, i = 0, j = 0, k = 0, t = 0;
	int **temp = NULL;
	int *ret = NULL;

	ret = (int *)malloc(block * lineNum * sizeof(int));
	block = lineNum / p;
	for (i = 0; i < p; i++)
	{
		temp = getSubB(i, p, lineNum, time, matrixB);
		for (j = 0; j < block; j++) {
			for (k = 0; k < block; k++) {
				ret[t] = temp[j][k];
				t++;
			}
		}
		free(temp);
	}
	return ret;
}

int* aaa(int *subA, int *subB1)
{
	for (int i = 0; i < 16; i++)
	{
		subA[i] = subA[i] + subB1[0];
	}
	return subA;
}

int *multiple(int *subA, int *subB, int p, int lineNum)
{
	/*int c[8][2];
	int a[8][2];
	int b[2][2];*/
	int **c = NULL;
	int **a = NULL;
	int **b = NULL;
	int *cc = NULL;
	int t = 0, i = 0, j = 0, k = 0;
	int *aa;
	int *bb;
	int block = 0;
	int ret = 0;

	block = lineNum / p;
/*	aa = (int *)malloc(block * lineNum * sizeof(int));
	bb = (int *)malloc(block * block * sizeof(int));
	memcpy(aa, subA, 16 * sizeof(int));
	memcpy(bb, subB, 4 * sizeof(int));*/

	a = (int **)malloc(lineNum * sizeof(int *));
	for (i = 0; i < lineNum; i++) {
		a[i] = (int *)malloc(block * sizeof(int));
	}

	b = (int **)malloc(block * sizeof(int *));
	for (i = 0; i < block; i++) {
		b[i] = (int *)malloc(block * sizeof(int));
	}

	c = (int **)malloc(lineNum * sizeof(int *));
	for (i = 0; i < lineNum; i++) {
		c[i] = (int *)malloc(block * sizeof(int));
	}

	for (i = 0; i < lineNum; i++)
	{
		for (j = 0; j < block; j++)
		{
			//a[i][j] = aa[t];
			a[i][j] = subA[t];
			t++;
		}
	}

	t = 0;
	for (i = 0; i < block; i++) {
		for (j = 0; j < block; j++) {
			//b[i][j] = bb[t];
			b[i][j] = subB[t];
			t++;
		}
	}

	for (i = 0; i < p; i++)
	{
		for (j = i*block; j < i*block+block; j++)
		{
			for (k = 0; k < block; k++)
			{
				ret = 0;
				for (t = 0; t < block; t++) {
					ret += a[j][t] * b[t][k];
				}
				c[j][k] = ret;
			}
		}
	}
	
	cc = (int *)malloc(lineNum * block * sizeof(int));
	t = 0;
	for (i = 0; i < lineNum; i++)
	{
		for (j = 0; j < block; j++)
		{
			cc[t] = c[i][j];
			t++;
		}
	}
	return cc;
}

/*int* submatrixMultiple(int *subA, int *subB1, int p, int lineNum)
{
	int i = 0, j = 0, k = 0, block = 0, t = 0, h = 0, f = 0, w= 0;
	int **subAMatrix = NULL;
	int **subAMatrixCal = NULL;
	int **subB = NULL;
	int *ret = NULL;
	int *aa = NULL;
	int *bb = NULL;

	block = lineNum / p;
	aa = (int *)malloc(lineNum*block * sizeof(int));
	memcpy(aa, subA, lineNum*block * sizeof(int));
	bb = (int *)malloc(block*block * sizeof(int));
	memcpy(bb, subB1, block*block * sizeof(int));
	subB = new int*[block];
	for (i = 0; i < block; i++)
	{
		subB[i] = new int[block];
	}
	for (i = 0; i < block; i++) {
		for (j = 0; j < block; j++) {
			subB[i][j] = bb[k];
			k++;
		}
	}

	subAMatrix = new int*[lineNum];
	for (i = 0; i < lineNum; i++)
	{
		subAMatrix[i] = new int[block];
	}
	subAMatrixCal = new int*[lineNum];
	for (i = 0; i < lineNum; i++)
	{
		subAMatrixCal[i] = new int[block];
	}
	for (i = 0; i < p ;i++)
	{
		for (j = block * i; j < block + block * i; j++)
		{
			for (k = 0; k < block; k++)
			{
				subAMatrix[j][k] = aa[t];
				t++;
			}
			for (int h = 0; h < block; h++)
			{
				subAMatrixCal[j][h] = 0;
				for (f = 0; f < block; f++)
				{
					subAMatrixCal[j][h] += subAMatrix[j][f] * subB[f][h];
				}
			}
		}

	}
	k = 0;
	ret = (int *)malloc(lineNum * p * sizeof(int));
	for (i = 0; i < lineNum; i++)
	{
		for (j = 0; j < block; j++)
		{
			ret[k] = subAMatrixCal[i][j];
			k++;
		}
	}
	return ret;
}
*/

int *matrixAdd(int* matrix1, int *matrix2, int p, int lineNum)
{
	int i, j ,block = 0;
	int *addMatrixCal = NULL;

	block = lineNum / p;
	addMatrixCal = (int *)malloc(lineNum * block * sizeof(int));
	for (i = 0; i < lineNum * block; i++)
	{
		addMatrixCal[i] = matrix1[i] + matrix2[i];
	}
	return addMatrixCal;
}

int main()
{
	int *a = NULL;
	int **b = NULL;
//	int *sub_mat = NULL;
	int *sub_matB = NULL;
	int *sub_mat_mul = NULL;
	int *subs = NULL;
	int **sub_matAs = NULL;
	int *sum = NULL;
	int i = 0;
	int lineNum = 0;
	MPI_Init(NULL, NULL);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	sub_matAs = (int **)malloc(world_size * sizeof(int *));
	for (int j = 0; j < world_size; j++) {
		sub_matAs[j] = (int *)malloc(lineNum * lineNum / world_size * sizeof(int));
	}

	if (world_rank == 0)
	{
		lineNum = countLine("a.txt");
		a = readMatrixAFromFile("a.txt", world_size, lineNum);
		b = readMatrixBFromFile("b.txt", lineNum);
	}
	MPI_Bcast(&lineNum, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("process %d llllllllllllll%d\n", world_rank, lineNum);
	MPI_Bcast(&world_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("process %d pppppppppppppp%d\n", world_rank, world_size);
//	sub_mat = (int *)malloc(lineNum / world_size *lineNum * sizeof(int));
	MPI_Scatter(a, lineNum / world_size *lineNum, MPI_INT, sub_matAs[0], lineNum / world_size *lineNum, MPI_INT, 0, MPI_COMM_WORLD);

	sub_matB = (int *)malloc(world_size * sizeof(int));
	if (world_rank == 0) {
		subs = submatrixBs(world_size, lineNum, 0, b);
		//MPI_Scatter(submatrixBs(world_size, lineNum, 0, b), world_size, MPI_INT, sub_matB, world_size, MPI_INT, 0, MPI_COMM_WORLD);
	}
	MPI_Scatter(subs, world_size, MPI_INT, sub_matB, world_size, MPI_INT, 0, MPI_COMM_WORLD);
	//MPI_Scatter(submatrixBs(world_size, lineNum, 0, b), world_size, MPI_INT, sub_matB, world_size, MPI_INT, 0, MPI_COMM_WORLD);

	sub_mat_mul = (int *)malloc(16 * sizeof(int));

	printf("I am proc %d\n", world_rank);
	for (int i = 0; i < 4; i++)
	{
		printf("%d ", sub_matB[i]);
	}
	printf("\n");
	for (int i = 0; i < 16; i++)
	{
		printf("%d ", sub_matAs[0][i]);
	}
	printf("\n");

	//sub_mat_mul = submatrixMultiple(sub_mat, sub_matB, 4, lineNum);
	//sub_mat_mul = aaa(sub_mat, sub_matB);
	sub_mat_mul = multiple(sub_matAs[0], sub_matB, 4, lineNum);
//	int bcd[16];
//	memcpy(bcd, sub_mat_mul, 16 * sizeof(int));
	for (int i = 0; i < 16; i++)
	{
		
		printf("%d ", sub_mat_mul[i]);
	}
	printf("\n");
	printf("-------------------------------------------------------------------\n");
	
	for (int q = 1; q < world_size; q++)
	{
		if (world_rank == 0)
		{
			subs = submatrixBs(world_size, lineNum, q, b);
		}
		MPI_Scatter(subs, world_size, MPI_INT, sub_matB, world_size, MPI_INT, 0, MPI_COMM_WORLD);

		printf("222222222222222222222I am proc %d\n", world_rank);
		for (int i = 0; i < 4; i++)
		{
			printf("%d ", sub_matB[i]);
		}
		for (int i = 0; i < 16; i++)
		{
			printf("%d ", sub_matAs[q -1][i]);
		}

		if (world_rank == 0) {
			MPI_Sendrecv(sub_matAs[q -1], lineNum * lineNum / world_size, MPI_INT, world_size - 1, 0, sub_matAs[q], lineNum * lineNum / world_size, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		else if (world_rank == world_size - 1) {
			MPI_Sendrecv(sub_matAs[q-1], lineNum * lineNum / world_size, MPI_INT, world_rank - 1, 0, sub_matAs[q], lineNum * lineNum / world_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		else {
			MPI_Sendrecv(sub_matAs[q-1], lineNum * lineNum / world_size, MPI_INT, world_rank - 1, 0, sub_matAs[q], lineNum * lineNum / world_size, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}

		//		MPI_Sendrecv(sub_mat, 16, MPI_INT, left, 0, sub_matAs[0], 16, MPI_INT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		printf("asddffffffffffffffffffffffffffffffffffffffffffffffffffffffffffprocess%d\n", world_rank);
		for (int i = 0; i < 16; i++)
		{
			printf("%d ", sub_matAs[q-1][i]);
		}
		printf("\n");
		sub_mat_mul = matrixAdd(sub_mat_mul, multiple(sub_matAs[q], sub_matB, world_size, lineNum), world_size, lineNum);
		printf("9999999999999999999999999999999999999999999999999999999999999999999999999999999process%d\n", world_rank);
		for (int i = 0; i < 16; i++)
		{
			printf("%d ", sub_mat_mul[i]);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	sum = (int *)malloc(64 * sizeof(int));
	MPI_Gather(sub_mat_mul, 16, MPI_INT, sum, 16, MPI_INT, 0, MPI_COMM_WORLD);
	if (world_rank == 0) {
		printf("uuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuu");
		for (int i = 0; i < 64; i++)
		{
			printf("%d ", sum[i]);
		}
	}
	MPI_Finalize();
    return 0;
}

