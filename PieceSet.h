#pragma once
#include "Piece.h"

//管理所有的图形，一共有7*4=28种
class PieceSet
{
public:
	enum{ NUM_PIECES = 7, NUM_ROTATIONS = 4 };
	PieceSet();
	~PieceSet();

	// 按照指定的id和rotation获取对应的图形
	Piece* getPiece(int id, int rotation = 0) const;

	// 获得随机图形
	inline Piece* getRandomPiece() const
	{
		return getPiece(rand() % NUM_PIECES, rand() % NUM_ROTATIONS);
	}

protected:
	//保存所有的图形
	Piece* pieces_[NUM_PIECES][NUM_ROTATIONS];

	//从初始化的7个图形，通过旋转生产28种图形
	void rotateAll();

	// 逆时针旋转
	void rotate(POINT* apt, int numPoints = 4);
};
