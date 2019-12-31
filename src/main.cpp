#include "Sudoku.h"

#include <windows.h>  

#include <cstdio>

#include <algorithm>
#include <ctime>
#include <cstring>

void wtoc(const wchar_t* wstr, char* cstr);
bool check_argv(char arg[]);
void sudoku_solve(const char infilename[], const char outfilename[]);
void sudoku_create(const int num, const char outfilename[]);

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("argc error, exit\n");
		exit(1);
	}

	if (argv[1][0] != '-' || (argv[1][1] != 'c' && argv[1][1] != 's') || argv[1][2] != '\0')
	{
		printf("argv[1] error, exit\n");
		exit(1);
	}

	wchar_t wpath[1005];
	GetModuleFileName(NULL, wpath, sizeof(wpath)); //��ȡģ��·��
	char cpath[1005];
	wtoc(wpath, cpath);
	(strrchr(cpath, '\\'))[1] = 0; //ȥ��ģ����������Ŀ¼·��
	strcat_s(cpath, sizeof(cpath), "sudoku.txt");

	switch (argv[1][1])
	{
	case 'c':
		if (!check_argv(argv[2]))
		{
			printf("argv[2] error, exit\n");
			exit(1);
		}
		sudoku_create(atoi(argv[2]), cpath);
		break;
	case 's':
		sudoku_solve(argv[2], cpath);
	}

	return 0;
}

/*
wchar_tתchar
*/
void wtoc(const wchar_t* wstr, char* cstr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstr, wcslen(wstr), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wstr, wcslen(wstr), cstr, len, NULL, NULL);
	cstr[len] = '\0';
}

/*
�������Ƿ�������
*/
bool check_argv(char arg[])
{
	for (int i = 0; arg[i] != '\0'; i++)
		if (arg[i] < '0' || arg[i]>'9')
			return false;

	return true;
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
		printf("sudoku_solve: fopen out_file fail, exit\n");
		return;
	}

	FILE* pFile = NULL;
	char* Buffer = NULL; //�ڴ滺����
	fopen_s(&pFile, infilename, "r");
	if (!pFile)
	{
		printf("sudoku_solve: fopen in_file fail, exit\nplease check filepath\n");
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
		sudoku.PrintSudoku_FC(fp);
		if (!sudoku.IsRight())
		{
			printf("solve error\n");
		}
		if (pB < rSize) //���һ������������
		{
			fprintf(fp, "\n");
			fprintf(fp, "\n");
		}
	}

	fclose(fp);
	fclose(pFile);
}

/*
������д�뻺����
*/
void write_sudoku(char* pline[9], char* buf, int &pbuf)
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			buf[pbuf++] = pline[i][j];
			if (j < 8)
			{
				buf[pbuf++] = ' ';
			}
		}
		if (i < 8)
		{
			buf[pbuf++] = '\n';
		}
	}
}

void sudoku_create(const int num, const char outfilename[])
{
	FILE* fp = NULL;
	fopen_s(&fp, outfilename, "w");
	if (!fp)
	{
		printf("sudoku_create: fopen out_file fail, exit\n");
		return;
	}

	char* buf = new char[180 * num];
	if (buf == NULL)
	{
		printf("sudoku_create: alloc buf fail, exit\n");
		return;
	}
	int pbuf = 0;

	char line[9][10];
	for (int i = 0; i < 9; i++)
		line[i][9] = '\0';
	char* pline[9]; //����ʱ��ָ�뽻�������ֵ
	for (int i = 0; i < 3; i++) //ǰ���в����н���
		pline[i] = line[i];

	int n = 0;
	char arr[10] = { '4','1','2','3','5','6','7','8','9','\0' }; //��ʼ����
	int offset[9] = { 0,3,6,1,4,7,2,5,8 }; //ÿ������ڵ�һ�����Ƶ�ƫ����
	int swapline[2][3]; //���������õ�����
	while (n < num)
	{
		next_permutation(&arr[1], &arr[9]);

		//�������ж�Ӧ�Ļ�������
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				line[i][j] = arr[(j + offset[i]) % 9];
			}
		}

		//�ڻ������������϶��н��н���
		swapline[0][0] = 3; swapline[0][1] = 4; swapline[0][2] = 5;
		swapline[1][0] = 6; swapline[1][1] = 7; swapline[1][2] = 8;
		do
		{
			do
			{
				pline[3] = line[swapline[0][0]]; pline[4] = line[swapline[0][1]]; pline[5] = line[swapline[0][2]];
				pline[6] = line[swapline[1][0]]; pline[7] = line[swapline[1][1]]; pline[8] = line[swapline[1][2]];
				write_sudoku(pline, buf, pbuf);
				n++; //���ɸ���+1
				if (n < num)
				{
					buf[pbuf++] = '\n';
					buf[pbuf++] = '\n';
				}
				else
					break;
			} while (next_permutation(swapline[1], swapline[1] + 3));
			if (n >= num)
				break;
		} while (next_permutation(swapline[0], swapline[0] + 3));
	}

	fwrite(buf, pbuf * sizeof(char), 1, fp);

	delete[] buf;
}