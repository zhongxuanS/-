#pragma once
#include <windows.h>
class Piece
{
public:
	Piece(int pieceId, int pieceRotaion, COLORREF pieceColor, const POINT* apt, int numPoints = 4);
	~Piece(void);

	//把图形中的方块都拿出来
	void getBody(POINT* apt) const;

	//获得下边缘，返回值为点个数，apt为具体点坐标
	int getSkirt(POINT* apt) const;
	//获得左边缘
	int getLeftSide(POINT* apt) const;
	//获得右边缘
	int getRightSide(POINT* apt) const;

	int getWidht() const {return width_;}
	int getHeight() const {return height_;}
	int getNPoints() const {return numPoints_;}
	int getId() const {return id_;}
	int getRotation() const {return rotaion_;}
	COLORREF getColor() const {return color_;}

	// 判断(x, y)在不在该图形中，用来获得skirt，left，right
	bool isPointExists(int x, int y) const;

	// 打印出图形中方块的情况，用来调试
	void print() const;

private:
	POINT* body_;//图形中各个方块的坐标
	int width_;//宽度
	int height_;//高度
	int numPoints_;//多少个方块
	int id_;
	int rotaion_;
	COLORREF color_;
};
