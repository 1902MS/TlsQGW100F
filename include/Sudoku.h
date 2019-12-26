#pragma once

#ifndef SUDOKU_H
#define SUDOKU_H

#include <queue>

using namespace std;

class Sudoku
{
private:
	/*
	ͨ������
	*/

	//3x3��Ԫ������
	typedef struct Matrix
	{
		int matrix[4][4][10]; //matrixΪ3x3����0�����в��ã�ÿ����Ԫ��[0]��ʾȷ��ֵ��[1:9]Ϊ��ѡֵ�б�
		int listsize[4][4]; //��Ӧ��Ԫ���б��ȣ�0��ʾ��ȷ��
	}Matrix;

	//������
	//3x3�Ź������У�0�����в���
	Matrix S[4][4];

	//��Ԫ������
	typedef struct Coordinate
	{
		int Sx, Sy; //�Ź�������
		int x, y; //��������
		Coordinate(const int Sx, const int Sy, const int x, const int y)
		{
			this->Sx = Sx; this->Sy = Sy; this->x = x; this->y = y;
		}
	}Coordinate;

	//δȷ����Ԫ�����
	int uncomfirmed;

	/*
	ͨ�ú���
	*/

	//����������
	void swap(int& a, int& b);

	/*
	��������������
	*/
	
	queue<Coordinate> que_cood;

	/*
	���������ֺ���
	*/

	//�ų���Ԫ���е�ָ����ѡֵ����ʣһ��ʱ����Ϊȷ����������true
	//cell-��Ԫ��ֵ�б�Matrix.matrix[x][y] size-�б��ȼ�Matrix.listsize[x][y] val-ȡֵ
	bool Eliminate_val(int cell[], int& size, const int val);

	//������ȷ����Ԫ���ֵ�ų�����ͬ�У�ͬ�У�ͬ����Ԫ���ѡֵ
	//Sx-�Ź����к� Sy-�Ź����к� x-��Ԫ���к� y-��Ԫ���к�
	void Eliminate(const int Sx, const int Sy, const int x, const int y);

	//���Ψһ����ֵ��������1��9ÿ��ֵ��ͬ��/ͬ��/ͬ����ȷ��ֵ+��ѡ�б����ִ���
	//ֻ����1��˵��������ֵ�ĵ�Ԫ��ֻ��ȡ��ֵ
	void Check_OnePossbile();
	
public:
	void Solve();

	/*
	���벿�ֺ���
	*/

	//��һ��9x9��ά����ת���������̣�Ϊ���пո�����ѡֵ�б�1��9�����ǿյ�Ԫ��ѹ�����
	void SetS(const int M[10][10]);

	/*
	������ֺ���
	*/

	//��������̵��ļ�����ӡ�ڿ���̨�����һ�в�����
	void PrintSudoku_FC(FILE* fp);

	//������������������ѡֵ�б�
	void PrintSudoku_Debug();
};

#endif // !SUDOKU_H