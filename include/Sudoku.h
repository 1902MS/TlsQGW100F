#pragma once

#ifndef SUDOKU_H
#define SUDOKU_H

#include <queue>

using namespace std;

class Sudoku
{
private:
	/*
	通用数据
	*/

	//3x3单元格阵列
	typedef struct Matrix
	{
		int matrix[4][4][10]; //matrix为3x3矩阵，0号行列不用，每个单元格[0]表示确定值，[1:9]为候选值列表
		int listsize[4][4]; //对应单元格列表长度，0表示已确定
	}Matrix;

	//数独盘
	//3x3九宫格阵列，0号行列不用
	Matrix S[4][4];

	//单元格坐标
	typedef struct Coordinate
	{
		int Sx, Sy; //九宫格行列
		int x, y; //宫内行列
		Coordinate(const int Sx, const int Sy, const int x, const int y)
		{
			this->Sx = Sx; this->Sy = Sy; this->x = x; this->y = y;
		}
	}Coordinate;

	//未确定单元格个数
	int uncomfirmed;

	/*
	通用函数
	*/

	//交换两个数
	void swap(int& a, int& b);

	/*
	解数独部分数据
	*/
	
	queue<Coordinate> que_cood;

	/*
	解数独部分函数
	*/

	//排除单元格中的指定候选值，仅剩一个时设置为确定，并返回true
	//cell-单元格值列表即Matrix.matrix[x][y] size-列表长度即Matrix.listsize[x][y] val-取值
	bool Eliminate_val(int cell[], int& size, const int val);

	//根据已确定单元格的值排除与其同行，同列，同宫单元格候选值
	//Sx-九宫格行号 Sy-九宫格列号 x-单元格行号 y-单元格列号
	void Eliminate(const int Sx, const int Sy, const int x, const int y);

	//检查唯一可能值，即计算1到9每个值在同行/同列/同宫（确定值+候选列表）出现次数
	//只出现1次说明包含该值的单元格只能取该值
	void Check_OnePossbile();
	
public:
	void Solve();

	/*
	输入部分函数
	*/

	//把一个9x9二维数组转换成数独盘，为所有空格建立候选值列表（1到9），非空单元格压入队列
	void SetS(const int M[10][10]);

	/*
	输出部分函数
	*/

	//输出数独盘到文件并打印在控制台，最后一行不换行
	void PrintSudoku_FC(FILE* fp);

	//调试输出，输出包括候选值列表
	void PrintSudoku_Debug();
};

#endif // !SUDOKU_H