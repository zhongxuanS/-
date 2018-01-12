#include "Piece.h"
#include <algorithm>

Piece::Piece(int pieceId, int pieceRotaion, COLORREF pieceColor, const POINT* apt, int numPoints)
	:color_(pieceColor), id_(pieceId), rotaion_(pieceRotaion),
	numPoints_(numPoints),width_(0),height_(0), body_(NULL)
{
	//因为旋转后的图形左下角并不在坐标原点，所以要做平移操作
	POINT leftBottom = apt[0];
	for(int i = 1; i < numPoints_; ++i)
	{
		leftBottom.x = std::min<long>(leftBottom.x, apt[i].x);
		leftBottom.y = std::min<long>(leftBottom.y, apt[i].y);
	}

	// 将apt中的点进行平移操作，使其左下角在坐标原点
	body_ = new POINT[numPoints_];
	for(int i = 0; i < numPoints_; ++i)
	{
		body_[i].x = apt[i].x - leftBottom.x;
		body_[i].y = apt[i].y - leftBottom.y;

		width_ = std::max<int>(static_cast<int>(body_[i].x + 1), width_);
		height_ = std::max<int>(static_cast<int>(body_[i].y + 1), height_);
	}
}


Piece::~Piece(void)
{
	delete[] body_;
}

void Piece::getBody(POINT* apt) const
{
	for(int i = 0; i < numPoints_; ++i)
	{
		apt[i] = body_[i];
	}
}

//获得左边缘点
int Piece::getLeftSide(POINT* apt) const
{
	int count = 0;
	//通过遍历的方式来获取
	for(int y = 0; y < height_; ++y)
	{
		for(int x = 0; x < width_; ++x)
		{
			if(isPointExists(x, y))
			{
				apt[count].x = x;
				apt[count].y = y;
				++count;
				break;
			}
		}
	}

	return count;
}

int Piece::getRightSide(POINT* apt) const
{
	int count = 0;
	for(int y = 0; y < height_; ++y)
	{
		for(int x = width_ - 1; x >= 0; --x)
		{
			if(isPointExists(x, y))
			{
				apt[count].x = x;
				apt[count].y = y;
				++count;
				break;
			}
		}
	}

	return count;
}

int Piece::getSkirt(POINT* apt) const
{
	int count = 0;

	for(int x = 0; x < width_; ++x)
	{
		for(int y = 0; y < height_; ++y)
		{
			if(isPointExists(x, y))
			{
				apt[count].x = x;
				apt[count].y = y;
				++count;
				break;
			}
		}
	}

	return count;
}

void Piece::print() const
{
	printf("width:%d, height:%d, numOfPoints:%d, color:%x\n", width_, height_, numPoints_, color_);
	for(int y = height_ - 1; y >= 0; --y)
	{
		for(int x = 0; x < width_; ++x)
		{
			if(isPointExists(x, y))
			{
				printf("#");
			}
			else
			{
				printf(" ");
			}
		}

		printf("\n");
	}
}

bool Piece::isPointExists(int x, int y) const
{
	for(int i = 0; i < numPoints_; ++i)
	{
		if(body_[i].x == x && body_[i].y == y)
		{
			return true;
		}
	}

	return false;
}
