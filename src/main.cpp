#include "Sudoku.h"

#include <cstdio>

void sudoku_solve(const char infilename[], const char outfilename[]);
void sudoku_create(const char outfilename[]);


int main(int argc, char* argv[])
{
	sudoku_solve("sudoku.txt", "result.txt");

	return 0;
}

/*
��ȡ����������һ������
*/
int BufferNextInt(const char* Buffer, size_t& pB, const size_t size)
{
	char numstr[25];
	for (; Buffer[pB] == ' ' || Buffer[pB] == '\n'; pB++); //��ȥ�Ʊ��
	int len = 0;
	for (; pB < size && Buffer[pB] != ' ' && Buffer[pB] != '\n'; pB++)
		numstr[len++] = Buffer[pB];
	numstr[len] = '\0';

	return atoi(numstr);
}

void sudoku_solve(const char infilename[], const char outfilename[])
{
	FILE* fp = NULL;
	fopen_s(&fp, outfilename, "w");
	if (!fp)
	{
		printf("sudoku_solve: fopen fp fail\n");
		return;
	}

	FILE* pFile = NULL;
	char* Buffer = NULL; //�ڴ滺����
	fopen_s(&pFile, infilename, "r");
	if (!pFile)
	{
		printf("sudoku_solve: fopen pFile fail\n");
		return;
	}

	long fSize;	//�ļ���С�����ֽڼƣ�
	fseek(pFile, 0, SEEK_END); //ָ���Ƶ��ļ�β
	fSize = ftell(pFile); //ָ����ļ��׵�ƫ�������ļ���С��

	size_t rSize; //ʵ�ʶ��뻺������С
	Buffer = (char*)malloc(fSize + 1);
	if (!Buffer)
	{
		printf("sudoku_solve: malloc fail\n");
		fclose(pFile);
		free(Buffer);
		return;
	}
	fseek(pFile, 0, SEEK_SET); //ָ���Ƶ��ļ���
	rSize = fread(Buffer, 1, fSize, pFile);

	size_t pB = 0; //������ָ��

	Sudoku sudoku;
	int M[10][10];
	while (pB < rSize)
	{
		for (int i = 1; i <= 9; i++)
		{
			for (int j = 1; j <= 9; j++)
			{
				M[i][j] = BufferNextInt(Buffer, pB, rSize);
			}
		}

		sudoku.SetS(M);
		sudoku.Solve();
		//sudoku.PrintSudoku_Debug();
		//sudoku.PrintSudoku_FC(fp);
		if (pB < rSize) //���һ������������
		{
			printf("\n");
			printf("\n");
			fprintf(fp, "\n");
		}
	}

	fclose(fp);
	fclose(pFile);
}

void sudoku_create(const char outfilename[])
{

}