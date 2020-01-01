#include "Sudoku.h"

#include <cstdio>

#include <queue>

//����ÿ��
#define FOR_EACH_R(fun)	for (int Sr = 1; Sr <= 3; Sr++)		\
						{									\
							for (int r = 1; r <= 3; r++)	\
							{								\
								fun							\
							}								\
						} 

//����ÿ��
#define FOR_EACH_C(fun)	for (int Sc = 1; Sc <= 3; Sc++)		\
						{									\
							for (int c = 1; c <= 3; c++)	\
							{								\
								fun							\
							}								\
						}

//����ÿ��
#define FOR_EACH_S(fun)	for (int Sr = 1; Sr <= 3; Sr++)		\
						{									\
							for (int Sc = 1; Sc <= 3; Sc++)	\
							{								\
								fun							\
							}								\
						}

//���������ÿ����Ԫ��
#define FOR_EACH_cell(fun)	for (int r = 1; r <= 3; r++)				\
							{											\
								for (int c = 1; c <= 3; c++)			\
								{										\
									fun									\
								}										\
							}

//��������ÿ����Ԫ��
#define FOR_EACH_cell_S(fun)	for (int Sr = 1; Sr <= 3; Sr++)				\
								{											\
									for (int Sc = 1; Sc <= 3; Sc++)			\
									{										\
										for (int r = 1; r <= 3; r++)		\
										{									\
											for (int c = 1; c <= 3; c++)	\
											{								\
												fun							\
											}								\
										}									\
									}										\
								}

//�����б���ÿ����Ԫ��
#define FOR_EACH_cell_RC(fun)	for (int Sr = 1; Sr <= 3; Sr++)				\
								{											\
									for (int r = 1; r <= 3; r++)			\
									{										\
										for (int Sc = 1; Sc <= 3; Sc++)		\
										{									\
											for (int c = 1; c <= 3; c++)	\
											{								\
												fun							\
											}								\
										}									\
									}										\
								}

/*
����������
*/
void Sudoku::swap(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

//����������
void Sudoku::copyS(Matrix des[4][4], Matrix src[4][4])
{
	FOR_EACH_S
	(
		des[Sr][Sc] = src[Sr][Sc];
	)
}

/*
��������Ƿ���ȷ�������Ƿ����е�Ԫ����ȷ��
*/
bool Sudoku::IsRight()
{
	if (uncomfirmed > 0)
		return false;

	bool occ[10]; //[i]��¼ֵi�Ƿ��ѳ���
	bool right = true; //��¼�Ƿ���ȷ

	//���ÿ��
	FOR_EACH_S
	(
		memset(occ, false, 10 * sizeof(bool));
		FOR_EACH_cell
		(
			int val = S[Sr][Sc].matrix[r][c][0];
			if (val == 0 || occ[val]) //ֵΪ0���ѳ��ֹ�
			{
				right = false;
				break;
			}
			else
				occ[val] = true;
		)
		if (!right)
			break;
	)
	if (!right)
		return false;

	//���ÿ��
	FOR_EACH_R
	(
		memset(occ, false, 10 * sizeof(bool));
		FOR_EACH_C
		(
			int val = S[Sr][Sc].matrix[r][c][0];
			if (val == 0 || occ[val]) //ֵΪ0���ѳ��ֹ�
			{
				right = false;
				break;
			}
			else
				occ[val] = true;
		)
		if (!right)
			break;
	)
	if (!right)
		return false;

	//���ÿ��
	FOR_EACH_C
	(
		memset(occ, false, 10 * sizeof(bool));
		FOR_EACH_R
		(
			int val = S[Sr][Sc].matrix[r][c][0];
			if (val == 0 || occ[val]) //ֵΪ0���ѳ��ֹ�
			{
				right = false;
				break;
			}
			else
				occ[val] = true;
		)
		if (!right)
			break;
	)
	if (!right)
		return false;

	return true;
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
void Sudoku::Eliminate_SRC(const int Sx, const int Sy, const int x, const int y)
{
	int val = S[Sx][Sy].matrix[x][y][0];

	//������ڹ�
	FOR_EACH_cell
	(
		if (r == x && c == y) //��������
			continue;
		if (S[Sx][Sy].matrix[r][c][0] == 0) //δȷ��
		{
			if (Eliminate_val(S[Sx][Sy].matrix[r][c], S[Sx][Sy].listsize[r][c], val)) //��Ϊȷ��ʱ���
				que_cood.push(Coordinate(Sx, Sy, r, c));
		}
	)

	//���������
	FOR_EACH_C
	(
		if (Sc == Sy && c == y) //��������
			continue;
		if (S[Sx][Sc].matrix[x][c][0] == 0) //δȷ��
		{
			if (Eliminate_val(S[Sx][Sc].matrix[x][c], S[Sx][Sc].listsize[x][c], val)) //��Ϊȷ��ʱ���
				que_cood.push(Coordinate(Sx, Sc, x, c));
		}
	)

	//���������
	FOR_EACH_R
	(
		if (Sr == Sx && r == x) //��������
			continue;
		if (S[Sr][Sy].matrix[r][y][0] == 0) //δȷ��
		{
			if (Eliminate_val(S[Sr][Sy].matrix[r][y], S[Sr][Sy].listsize[r][y], val)) //��Ϊȷ��ʱ���
				que_cood.push(Coordinate(Sr, Sy, r, y));
		}
	)
}

/*
���Ψһ����ֵ��������1��9ÿ��ֵ��ͬ��/ͬ��/ͬ����ȷ��ֵ+��ѡ�б����ִ���
ֻ����1��˵��������ֵ�ĵ�Ԫ��ֻ��ȡ��ֵ
*/
void Sudoku::Check_OnePossbile()
{
	if (uncomfirmed == 0)
		return;

	int times[10]; //[i]��¼ֵi���ִ���
	bool uc[10]; //[i]=true��ʾֵi������ȷ����Ԫ���ֵ
	int index[10]; //[i]��¼ֵi���һ�γ������ĸ���Ԫ����ֵiֻ����1����ô[i]ǡ��ָ�����ֵi�ĵ�Ԫ��

	//��
	FOR_EACH_S
	(
		memset(times, 0, 10 * sizeof(int));
		memset(uc, true, 10 * sizeof(bool));

		FOR_EACH_cell
		(
			if (S[Sr][Sc].matrix[r][c][0] != 0) //��ȷ��
			{
				int val = S[Sr][Sc].matrix[r][c][0];
				times[val]++;
				uc[val] = false;
			}
			else //δȷ��
			{
				int size = S[Sr][Sc].listsize[r][c];
				for (int i = 1; i <= size; i++)
				{
					int val = S[Sr][Sc].matrix[r][c][i];
					times[val]++;
					index[val] = (r - 1) * 3 + c; //��Ԫ�񰴴��ϵ��´����Ҵ�1��ʼ���
				}
			}
		)
			
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
	)

	//��
	FOR_EACH_R
	(
		memset(times, 0, 10 * sizeof(int));
		memset(uc, true, 10 * sizeof(bool));

		FOR_EACH_C
		(
			if (S[Sr][Sc].matrix[r][c][0] != 0) //��ȷ��
			{
				int val = S[Sr][Sc].matrix[r][c][0];
				times[val]++;
				uc[val] = false;
			}
			else //δȷ��
			{
				int size = S[Sr][Sc].listsize[r][c];
				for (int i = 1; i <= size; i++)
				{
					int val = S[Sr][Sc].matrix[r][c][i];
					times[val]++;
					index[val] = (Sc - 1) * 3 + c; //��Ԫ�񰴴����Ҵ�1��ʼ���
				}
			}
		)

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
	)


	//��
	FOR_EACH_C
	(
		memset(times, 0, 10 * sizeof(int));
		memset(uc, true, 10 * sizeof(bool));

		FOR_EACH_R
		(
			if (S[Sr][Sc].matrix[r][c][0] != 0) //��ȷ��
			{
				int val = S[Sr][Sc].matrix[r][c][0];
				times[val]++;
				uc[val] = false;
			}
			else //δȷ��
			{
				int size = S[Sr][Sc].listsize[r][c];
				for (int i = 1; i <= size; i++)
				{
					int val = S[Sr][Sc].matrix[r][c][i];
					times[val]++;
					index[val] = (Sr - 1) * 3 + r; //��Ԫ�񰴴��ϵ��´�1��ʼ���
				}
			}
		)

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
	)
}

/*
���в�Ϊ��ʱ������У�ֱ������Ϊ��
Sx-�Ź����к� Sy-�Ź����к� x-��Ԫ���к� y-��Ԫ���к�
*/
void Sudoku::Eliminate_que()
{
	while (!que_cood.empty())
	{
		while (!que_cood.empty())
		{
			Coordinate cood = que_cood.front();
			que_cood.pop();
			int Sx = cood.Sx, Sy = cood.Sy, x = cood.x, y = cood.y;

			if (uncomfirmed == 0) //���е�Ԫ����ȷ����ֻ��Ѷ��е��գ�������������
				continue;

			Eliminate_SRC(Sx, Sy, x, y);
		}
		
		if (uncomfirmed != 0)
			Check_OnePossbile(); //�����п��ܻ����е�Ԫ�����
	}
}

/*
���㵥Ԫ��ķ����������ڻ��ݷ�ѡ��Ϻõĵ�Ԫ�������չ
���� = (10-��ѡ����) + ͬ��ȷ������ + ͬ��ȷ������
*/
int Sudoku::score_cell(const int Sx, const int Sy, const int x, const int y)
{
	if (S[Sx][Sy].matrix[x][y][0] != 0)
		return -1;

	int sorc = 10 - S[Sx][Sy].listsize[x][y];
	//����ͬ��ȷ������
	FOR_EACH_C
	(
		if (S[Sx][Sc].matrix[x][c][0] != 0)
			sorc++;
	)
	//����ͬ��ȷ������
	FOR_EACH_R
	(
		if (S[Sr][Sy].matrix[r][y][0] != 0)
			sorc++;
	)

	return sorc;
}

/*
�ҵ�������ߵĵ�Ԫ��
*/
void Sudoku::find_best_cell(Coordinate &coord)
{
	int maxscore = 0;
	FOR_EACH_cell_S
	(
		int score = score_cell(Sr, Sc, r, c);
		if (score > maxscore)
		{
			coord.Sx = Sr; coord.Sy = Sc;
			coord.x = r; coord.y = c;
			maxscore = score;
		}
	)
}

/*
��ĳ����Ԫ��ʹ�û��ݷ�����ȷ����ֵ�����ɹ�ȷ������true
*/
bool Sudoku::Eliminate_bt(const int Sx, const int Sy, const int x, const int y)
{
	Matrix copy[4][4]; //S�ĸ���
	copyS(copy, S);

	int solved = false;
	int size = S[Sx][Sy].listsize[x][y];
	for (int i = 1; i <= size; i++)
	{
		int copy_uncomfirmed = uncomfirmed;

		S[Sx][Sy].matrix[x][y][0] = S[Sx][Sy].matrix[x][y][i];
		S[Sx][Sy].listsize[x][y] = 0;
		uncomfirmed--; //δȷ����Ԫ����-1
		que_cood.push(Coordinate(Sx, Sy, x, y));
		Eliminate_que();
		if (IsRight())
		{
			solved = true;
			break;
		}
		else if (uncomfirmed > 0)
		{
			Coordinate coord(0, 0, 0, 0);
			find_best_cell(coord);
			solved = Eliminate_bt(coord.Sx, coord.Sy, coord.x, coord.y);
			if (solved)
				break;
		}
		
		copyS(S, copy); //��ԭ������
		uncomfirmed = copy_uncomfirmed; //��ԭδȷ����Ԫ����
	}

	return solved;
}

//���ݷ�������δȷ����Ԫ��
void Sudoku::Eliminate()
{
	if (uncomfirmed == 0)
		return;

	Coordinate coord(0, 0, 0, 0);
	find_best_cell(coord);
	
	Eliminate_bt(coord.Sx, coord.Sy, coord.x, coord.y);
}

/*
������
*/
void Sudoku::Solve()
{
	Eliminate_que(); //�ȴ�������ʱ�����Ķ���
	Eliminate();
}

/*
��һ��9x9��ά����ת���������̣�Ϊ���пո�����ѡֵ�б�1��9�����ǿյ�Ԫ��ѹ�����
*/
void Sudoku::SetS(const int M[10][10])
{
	uncomfirmed = 0;

	//��M���Ƶ�S
	FOR_EACH_cell_S
	(
		S[Sr][Sc].matrix[r][c][0] = M[(Sr - 1) * 3 + r][(Sc - 1) * 3 + c];
		S[Sr][Sc].listsize[r][c] = 0;
	)

	//Ϊ���пո�����ѡֵ�б�1��9�����ǿյ�Ԫ��ѹ�����
	FOR_EACH_cell_S
	(
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
	)
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
	FOR_EACH_cell_RC
	(
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

		fprintf(fp, "%d", val);
		if (Sc == 3 && c == 3)
		{
			if (Sr != 3 || r != 3) //���һ�в�����
			{
				fprintf(fp, "\n");
			}
		}
		else
		{
			fprintf(fp, " ");
		}
	)
	if (warning)
	{
		printf("warning: zeroval_n=%d unfonfirm_n=%d both_n=%d\n", zeroval_n, unfonfirm_n, both_n);
	}
}

/*������������������ѡֵ�б�*/
void Sudoku::PrintSudoku_Debug()
{
	FOR_EACH_cell_RC
	(
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
			}
		}
		else
		{
			printf(" ");
		}

		if (c == 3)
		{
			if (Sc != 3)
				printf("   ");
			else if (r == 3)
				printf("\n");
		}
	)
	printf("unconfirmed: %dcells\n", uncomfirmed);
}