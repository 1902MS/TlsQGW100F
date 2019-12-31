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
	inline void swap(int& a, int& b);

	//复制数独盘
	void copyS(Matrix des[4][4], Matrix src[4][4]);
	
	/*
	解数独部分数据
	*/
	
	queue<Coordinate> que_cood; //新确定的单元格的队列，每个单元格变为确定后首先入队
								//利用其值处理与其相关的未确定单元格的候选队列，每个单元格至多入队一次

	/*
	解数独部分函数
	*/

	//排除单元格中的指定候选值，仅剩一个时设置为确定，并返回true
	//cell-单元格值列表即Matrix.matrix[x][y] size-列表长度即Matrix.listsize[x][y] val-取值
	bool Eliminate_val(int cell[], int& size, const int val);

	//根据已确定单元格的值排除与其同行，同列，同宫单元格候选值
	//Sx-九宫格行号 Sy-九宫格列号 x-单元格行号 y-单元格列号
	void Eliminate_SRC(const int Sx, const int Sy, const int x, const int y);

	//检查唯一可能值，即计算1到9每个值在同行/同列/同宫（确定值+候选列表）出现次数
	//只出现1次说明包含该值的单元格只能取该值
	void Check_OnePossbile();

	//计算单元格的分数，用于在回溯法选择较好的单元格进行扩展
	int score_cell(const int Sx, const int Sy, const int x, const int y);

	//找到评分最高的单元格
	void find_best_cell(Coordinate &coord);

	//队列不为空时处理队列，直至队列为空
	void Eliminate_que();

	//对某个单元格使用回溯法尝试确定其值，若成功确定返回true
	bool Eliminate_bt(const int Sx, const int Sy, const int x, const int y);

	//回溯法解所有未确定单元格
	void Eliminate();
	
public:
	/*
	功能函数
	*/

	//检查数独是否正确，包括是否所有单元格都已确定
	bool IsRight();

	//解数独
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