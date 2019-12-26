#include "Sudoku.h"

#include <cstdio>

/*交换两个数*/
void Sudoku::swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

/*
排除单元格中的指定候选值，仅剩一个时设置为确定，并返回true
cell-单元格值列表即Matrix.matrix[x][y] size-列表长度即Matrix.listsize[x][y] val-取值
*/
bool Sudoku::Eliminate_val(int cell[], int& size, const int val)
{
	bool confirm = false;
	for (int i = 1; i <= size; i++)
	{
		if (cell[i] == val)
		{
			swap(cell[i], cell[size]); //与列表最后一个元素交换
			size--;
			if (size == 1) //仅剩一个候选值即确定
			{
				cell[0] = cell[1];
				size = 0;
				confirm = true;
				uncomfirmed--; //未确定单元格数量-1
			}
		}
	}
	return confirm;
}

/*
根据已确定单元格的值排除与其同行，同列，同宫单元格候选值
Sx-九宫格行号 Sy-九宫格列号 x-单元格行号 y-单元格列号
*/
void Sudoku::Eliminate(const int Sx, const int Sy, const int x, const int y)
{
	int val = S[Sx][Sy].matrix[x][y][0];

	//宫
	for (int r = 1; r <= 3; r++) //单元格行
	{
		for (int c = 1; c <= 3; c++) //单元格列
		{
			if (r == x && c == y) //跳过本格
				continue;
			if (S[Sx][Sy].listsize[r][c] != 0) //未确定
			{
				if (Eliminate_val(S[Sx][Sy].matrix[r][c], S[Sx][Sy].listsize[r][c], val)) //变为确定时入队
					que_cood.push(Coordinate(Sx, Sy, r, c));
			}
		}
	}

	//行
	for (int Sc = 1; Sc <= 3; Sc++) //九宫格列
	{
		for (int c = 1; c <= 3; c++) //单元格列
		{
			if (Sc == Sy && c == y) //跳过本格
				continue;
			if (S[Sx][Sc].listsize[x][c] != 0) //未确定
			{
				if (Eliminate_val(S[Sx][Sc].matrix[x][c], S[Sx][Sc].listsize[x][c], val)) //变为确定时入队
					que_cood.push(Coordinate(Sx, Sc, x, c));
			}
		}
	}

	//列
	for (int Sr = 1; Sr <= 3; Sr++) //九宫格行
	{
		for (int r = 1; r <= 3; r++) //单元格行
		{
			if (Sr == Sx && r == x) //跳过本格
				continue;
			if (S[Sr][Sy].listsize[r][y] != 0)
			{
				if (Eliminate_val(S[Sr][Sy].matrix[r][y], S[Sr][Sy].listsize[r][y], val)) //变为确定时入队
					que_cood.push(Coordinate(Sr, Sy, r, y));
			}
		}
	}
}

/*
检查唯一可能值，即计算1到9每个值在同行/同列/同宫（确定值+候选列表）出现次数
只出现1次说明包含该值的单元格只能取该值
*/
void Sudoku::Check_OnePossbile()
{
	int times[10]; //[i]记录值i出现次数
	bool uc[10]; //[i]=true表示值i不是已确定单元格的值
	int index[10]; //[i]记录值i最近一次出现在哪个单元格，若值i只出现1次那么[i]恰好指向包含值i的单元格

	//宫
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
					if (size == 0) //已确定
					{
						int val = S[Sr][Sc].matrix[r][c][0];
						times[val]++;
						uc[val] = false;
					}
					else //未确定
					{
						for (int i = 1; i <= size; i++)
						{
							int val = S[Sr][Sc].matrix[r][c][i];
							times[val]++;
							index[val] = (r - 1) * 3 + c; //单元格按从上到下从左到右从1开始编号
						}
					}
				}
			}
			
			for (int i = 1; i <= 9; i++)
			{
				if (uc[i] && times[i] == 1) //不是确定值且只出现一次，变为确定并入队
				{
					int r = (index[i] - 1) / 3 + 1;
					int c = (index[i] - 1) % 3 + 1;
					S[Sr][Sc].matrix[r][c][0] = i;
					S[Sr][Sc].listsize[r][c] = 0;
					uncomfirmed--; //未确定单元格数量-1
					que_cood.push(Coordinate(Sr, Sc, r, c));
				}
			}
		}
	}

	//行
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
					if (size == 0) //已确定
					{
						int val = S[Sr][Sc].matrix[r][c][0];
						times[val]++;
						uc[val] = false;
					}
					else //未确定
					{
						for (int i = 1; i <= size; i++)
						{
							int val = S[Sr][Sc].matrix[r][c][i];
							times[val]++;
							index[val] = (Sc - 1) * 3 + c; //单元格按从左到右从1开始编号
						}
					}
				}
			}

			for (int i = 1; i <= 9; i++)
			{
				if (uc[i] && times[i] == 1) //不是确定值且只出现一次，变为确定并入队
				{
					int Sc = (index[i] - 1) / 3 +1;
					int c = (index[i] - 1) % 3 + 1;
					S[Sr][Sc].matrix[r][c][0] = i;
					S[Sr][Sc].listsize[r][c] = 0;
					uncomfirmed--; //未确定单元格数量-1
					que_cood.push(Coordinate(Sr, Sc, r, c));
				}
			}
		}
	}


	//列
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
					if (size == 0) //已确定
					{
						int val = S[Sr][Sc].matrix[r][c][0];
						times[val]++;
						uc[val] = false;
					}
					else //未确定
					{
						for (int i = 1; i <= size; i++)
						{
							int val = S[Sr][Sc].matrix[r][c][i];
							times[val]++;
							index[val] = (Sr - 1) * 3 + r; //单元格按从上到下从1开始编号
						}
					}
				}
			}

			for (int i = 1; i <= 9; i++)
			{
				if (uc[i] && times[i] == 1) //不是确定值且只出现一次，变为确定并入队
				{
					int Sr = (index[i] - 1) / 3 + 1;
					int r = (index[i] - 1) % 3 + 1;
					S[Sr][Sc].matrix[r][c][0] = i;
					S[Sr][Sc].listsize[r][c] = 0;
					uncomfirmed--; //未确定单元格数量-1
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

			if (uncomfirmed == 0) //所有单元格已确定，只需把队列弹空，无需其它操作
				continue;

			Eliminate(Sx, Sy, x, y);
		}

		printf("Eliminate\n");
		PrintSudoku_Debug();
		printf("\n");
		printf("\n");
		
		Check_OnePossbile(); //过程中可能还会有单元格入队

		printf("Check_OnePossbile\n");
		PrintSudoku_Debug();
		printf("\n");
		printf("\n");
	}
}

/*
把一个9x9二维数组转换成数独盘，为所有空格建立候选值列表（1到9），非空单元格压入队列
*/
void Sudoku::SetS(const int M[10][10])
{
	uncomfirmed = 0;

	//将M复制到S
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

	//为所有空格建立候选值列表（1到9），非空单元格压入队列
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
						uncomfirmed++; //未确定单元格数量+1
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

/*输出数独盘到文件并打印在控制台，最后一行不换行*/
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
						if (Sr != 3 || r != 3) //最后一行不换行
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

/*调试输出，输出包括候选值列表*/
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
						if (Sr != 3 || r != 3) //最后一行不换行
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