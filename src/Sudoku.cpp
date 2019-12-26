#include "Sudoku.h"

#include <cstdio>

/*����������*/
void Sudoku::swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

/*
�ų���Ԫ���е�ָ����ѡֵ����ʣһ��ʱ����Ϊȷ����������true
cell-��Ԫ��ֵ�б�Matrix.matrix[x][y] size-�б��ȼ�Matrix.listsize[x][y] val-ȡֵ
*/
bool Sudoku::Eliminate_val(int cell[], int& size, const int val)
{
	bool confirm = false;
	for (int i = 1; i <= size; i++)
	{
		if (cell[i] == val)
		{
			swap(cell[i], cell[size]); //���б����һ��Ԫ�ؽ���
			size--;
			if (size == 1) //��ʣһ����ѡֵ��ȷ��
			{
				cell[0] = cell[1];
				size = 0;
				confirm = true;
				uncomfirmed--; //δȷ����Ԫ������-1
			}
		}
	}
	return confirm;
}

/*
������ȷ����Ԫ���ֵ�ų�����ͬ�У�ͬ�У�ͬ����Ԫ���ѡֵ
Sx-�Ź����к� Sy-�Ź����к� x-��Ԫ���к� y-��Ԫ���к�
*/
void Sudoku::Eliminate(const int Sx, const int Sy, const int x, const int y)
{
	int val = S[Sx][Sy].matrix[x][y][0];

	//��
	for (int r = 1; r <= 3; r++) //��Ԫ����
	{
		for (int c = 1; c <= 3; c++) //��Ԫ����
		{
			if (r == x && c == y) //��������
				continue;
			if (S[Sx][Sy].listsize[r][c] != 0) //δȷ��
			{
				if (Eliminate_val(S[Sx][Sy].matrix[r][c], S[Sx][Sy].listsize[r][c], val)) //��Ϊȷ��ʱ���
					que_cood.push(Coordinate(Sx, Sy, r, c));
			}
		}
	}

	//��
	for (int Sc = 1; Sc <= 3; Sc++) //�Ź�����
	{
		for (int c = 1; c <= 3; c++) //��Ԫ����
		{
			if (Sc == Sy && c == y) //��������
				continue;
			if (S[Sx][Sc].listsize[x][c] != 0) //δȷ��
			{
				if (Eliminate_val(S[Sx][Sc].matrix[x][c], S[Sx][Sc].listsize[x][c], val)) //��Ϊȷ��ʱ���
					que_cood.push(Coordinate(Sx, Sc, x, c));
			}
		}
	}

	//��
	for (int Sr = 1; Sr <= 3; Sr++) //�Ź�����
	{
		for (int r = 1; r <= 3; r++) //��Ԫ����
		{
			if (Sr == Sx && r == x) //��������
				continue;
			if (S[Sr][Sy].listsize[r][y] != 0)
			{
				if (Eliminate_val(S[Sr][Sy].matrix[r][y], S[Sr][Sy].listsize[r][y], val)) //��Ϊȷ��ʱ���
					que_cood.push(Coordinate(Sr, Sy, r, y));
			}
		}
	}
}

/*
���Ψһ����ֵ��������1��9ÿ��ֵ��ͬ��/ͬ��/ͬ����ȷ��ֵ+��ѡ�б����ִ���
ֻ����1��˵��������ֵ�ĵ�Ԫ��ֻ��ȡ��ֵ
*/
void Sudoku::Check_OnePossbile()
{
	int times[10]; //[i]��¼ֵi���ִ���
	bool uc[10]; //[i]=true��ʾֵi������ȷ����Ԫ���ֵ
	int index[10]; //[i]��¼ֵi���һ�γ������ĸ���Ԫ����ֵiֻ����1����ô[i]ǡ��ָ�����ֵi�ĵ�Ԫ��

	//��
	for (int Sr = 1; Sr <= 3; Sr++)
	{
		for (int Sc = 1; Sc <= 3; Sc++)
		{
			memset(times, 0, 10 * sizeof(int));
			memset(uc, true, 10 * sizeof(bool));
			for (int r = 1; r <= 3; r++)
			{
				for (int c = 1; c <= 3; c++)
				{
					int size = S[Sr][Sc].listsize[r][c];
					if (size == 0) //��ȷ��
					{
						int val = S[Sr][Sc].matrix[r][c][0];
						times[val]++;
						uc[val] = false;
					}
					else //δȷ��
					{
						for (int i = 1; i <= size; i++)
						{
							int val = S[Sr][Sc].matrix[r][c][i];
							times[val]++;
							index[val] = (r - 1) * 3 + c; //��Ԫ�񰴴��ϵ��´����Ҵ�1��ʼ���
						}
					}
				}
			}
			
			for (int i = 1; i <= 9; i++)
			{
				if (uc[i] && times[i] == 1) //����ȷ��ֵ��ֻ����һ�Σ���Ϊȷ�������
				{
					int r = (index[i] - 1) / 3 + 1;
					int c = (index[i] - 1) % 3 + 1;
					S[Sr][Sc].matrix[r][c][0] = i;
					S[Sr][Sc].listsize[r][c] = 0;
					uncomfirmed--; //δȷ����Ԫ������-1
					que_cood.push(Coordinate(Sr, Sc, r, c));
				}
			}
		}
	}

	//��
	for (int Sr = 1; Sr <= 3; Sr++)
	{
		for (int r = 1; r <= 3; r++)
		{
			memset(times, 0, 10 * sizeof(int));
			memset(uc, true, 10 * sizeof(bool));
			for (int Sc = 1; Sc <= 3; Sc++)
			{
				for (int c = 1; c <= 3; c++)
				{
					int size = S[Sr][Sc].listsize[r][c];
					if (size == 0) //��ȷ��
					{
						int val = S[Sr][Sc].matrix[r][c][0];
						times[val]++;
						uc[val] = false;
					}
					else //δȷ��
					{
						for (int i = 1; i <= size; i++)
						{
							int val = S[Sr][Sc].matrix[r][c][i];
							times[val]++;
							index[val] = (Sc - 1) * 3 + c; //��Ԫ�񰴴����Ҵ�1��ʼ���
						}
					}
				}
			}

			for (int i = 1; i <= 9; i++)
			{
				if (uc[i] && times[i] == 1) //����ȷ��ֵ��ֻ����һ�Σ���Ϊȷ�������
				{
					int Sc = (index[i] - 1) / 3 +1;
					int c = (index[i] - 1) % 3 + 1;
					S[Sr][Sc].matrix[r][c][0] = i;
					S[Sr][Sc].listsize[r][c] = 0;
					uncomfirmed--; //δȷ����Ԫ������-1
					que_cood.push(Coordinate(Sr, Sc, r, c));
				}
			}
		}
	}


	//��
	for (int Sc = 1; Sc <= 3; Sc++)
	{
		for (int c = 1; c <= 3; c++)
		{
			memset(times, 0, 10 * sizeof(int));
			memset(uc, true, 10 * sizeof(bool));
			for (int Sr = 1; Sr <= 3; Sr++)
			{
				for (int r = 1; r <= 3; r++)
				{
					int size = S[Sr][Sc].listsize[r][c];
					if (size == 0) //��ȷ��
					{
						int val = S[Sr][Sc].matrix[r][c][0];
						times[val]++;
						uc[val] = false;
					}
					else //δȷ��
					{
						for (int i = 1; i <= size; i++)
						{
							int val = S[Sr][Sc].matrix[r][c][i];
							times[val]++;
							index[val] = (Sr - 1) * 3 + r; //��Ԫ�񰴴��ϵ��´�1��ʼ���
						}
					}
				}
			}

			for (int i = 1; i <= 9; i++)
			{
				if (uc[i] && times[i] == 1) //����ȷ��ֵ��ֻ����һ�Σ���Ϊȷ�������
				{
					int Sr = (index[i] - 1) / 3 + 1;
					int r = (index[i] - 1) % 3 + 1;
					S[Sr][Sc].matrix[r][c][0] = i;
					S[Sr][Sc].listsize[r][c] = 0;
					uncomfirmed--; //δȷ����Ԫ������-1
					que_cood.push(Coordinate(Sr, Sc, r, c));
				}
			}
		}
	}
}

void Sudoku::Solve()
{
	printf("ori\n");
	PrintSudoku_Debug();
	printf("\n");
	printf("\n");

	while (!que_cood.empty())
	{
		while (!que_cood.empty())
		{
			Coordinate cood = que_cood.front();
			que_cood.pop();
			int Sx = cood.Sx, Sy = cood.Sy, x = cood.x, y = cood.y;

			if (uncomfirmed == 0) //���е�Ԫ����ȷ����ֻ��Ѷ��е��գ�������������
				continue;

			Eliminate(Sx, Sy, x, y);
		}

		printf("Eliminate\n");
		PrintSudoku_Debug();
		printf("\n");
		printf("\n");
		
		Check_OnePossbile(); //�����п��ܻ����е�Ԫ�����

		printf("Check_OnePossbile\n");
		PrintSudoku_Debug();
		printf("\n");
		printf("\n");
	}
}

/*
��һ��9x9��ά����ת���������̣�Ϊ���пո�����ѡֵ�б�1��9�����ǿյ�Ԫ��ѹ�����
*/
void Sudoku::SetS(const int M[10][10])
{
	uncomfirmed = 0;

	//��M���Ƶ�S
	for (int Sr = 1; Sr <= 3; Sr++)
	{
		for (int Sc = 1; Sc <= 3; Sc++)
		{
			for (int r = 1; r <= 3; r++)
			{
				for (int c = 1; c <= 3; c++)
				{
					S[Sr][Sc].matrix[r][c][0] = M[(Sr - 1) * 3 + r][(Sc - 1) * 3 + c];
					S[Sr][Sc].listsize[r][c] = 0;
				}
			}
		}
	}

	//Ϊ���пո�����ѡֵ�б�1��9�����ǿյ�Ԫ��ѹ�����
	for (int Sr = 1; Sr <= 3; Sr++)
	{
		for (int Sc = 1; Sc <= 3; Sc++)
		{
			for (int r = 1; r <= 3; r++)
			{
				for (int c = 1; c <= 3; c++)
				{
					if (S[Sr][Sc].matrix[r][c][0] == 0)
					{
						uncomfirmed++; //δȷ����Ԫ������+1
						S[Sr][Sc].listsize[r][c] = 9;
						for (int i = 1; i <= 9; i++)
						{
							S[Sr][Sc].matrix[r][c][i] = i;
						}
					}
					else
					{
						que_cood.push(Coordinate(Sr, Sc, r, c));
					}
				}
			}
		}
	}
}

/*��������̵��ļ�����ӡ�ڿ���̨�����һ�в�����*/
void Sudoku::PrintSudoku_FC(FILE* fp)
{
	if (fp == NULL)
	{
		printf("PrintSudoku_FC: fp is NULL\n");
		return;
	}

	bool warning = false;
	int zeroval_n = 0;
	int unfonfirm_n = 0;
	int both_n = 0;
	for (int Sr = 1; Sr <= 3; Sr++)
	{
		for (int r = 1; r <= 3; r++)
		{
			for (int Sc = 1; Sc <= 3; Sc++)
			{
				for (int c = 1; c <= 3; c++)
				{
					int val = S[Sr][Sc].matrix[r][c][0];
					int size = S[Sr][Sc].listsize[r][c];
					if (val == 0 || size != 0)
					{
						warning = true;
						if (val == 0)
							zeroval_n++;
						if (size != 0)
							unfonfirm_n++;
						if (val == 0 && size != 0)
							both_n++;
					}

					printf("%d", val);
					fprintf(fp, "%d", val);
					if (Sc == 3 && c == 3)
					{
						if (Sr != 3 || r != 3) //���һ�в�����
						{
							printf("\n");
							fprintf(fp, "\n");
						}
					}
					else
					{
						printf(" ");
						fprintf(fp, " ");
					}
				}
			}
		}
	}
	if (warning)
	{
		printf("warning: zeroval_n=%d unfonfirm_n=%d both_n=%d\n", zeroval_n, unfonfirm_n, both_n);
	}
}

/*������������������ѡֵ�б�*/
void Sudoku::PrintSudoku_Debug()
{
	for (int Sr = 1; Sr <= 3; Sr++)
	{
		for (int r = 1; r <= 3; r++)
		{
			for (int Sc = 1; Sc <= 3; Sc++)
			{
				for (int c = 1; c <= 3; c++)
				{
					int size = S[Sr][Sc].listsize[r][c];

					printf("{%d|", S[Sr][Sc].matrix[r][c][0]);
					for (int i = 1; i <= 9; i++)
					{
						if (i <= size)
						{
							printf("%d", S[Sr][Sc].matrix[r][c][i]);
						}
						else
						{
							printf("-");
						}
					}
					printf("}");

					if (Sc == 3 && c == 3)
					{
						if (Sr != 3 || r != 3) //���һ�в�����
						{
							printf("\n");
							//fprintf(fp, "\n");
						}
					}
					else
					{
						printf(" ");
						//fprintf(fp, " ");
					}
				}
				if (Sc != 3)
					printf("   ");
			}
		}
		printf("\n");
	}
}