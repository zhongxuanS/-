#include "PieceSet.h"


PieceSet::PieceSet(void)
{
	//初始化指针
	for(int i = 0; i < NUM_PIECES; ++i)
	{
		for(int j = 0; j < NUM_ROTATIONS; ++j)
		{
			pieces_[i][j] = 0;
		}
	}

	POINT apt[NUM_PIECES];

	// 0, I piece, red
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 0;    apt[1].y = 1;
	apt[2].x = 0;    apt[2].y = 2;
	apt[3].x = 0;    apt[3].y = 3;
	pieces_[0][0] = new Piece(0, 0, RGB(255,0,0), apt);

	// 1, L piece, orange
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 0;    apt[2].y = 1;
	apt[3].x = 0;    apt[3].y = 2;
	pieces_[1][0] = new Piece(1, 0, RGB(230,130,24), apt);

	// 2, counter-L piece, yellow
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 1;    apt[2].y = 1;
	apt[3].x = 1;    apt[3].y = 2;
	pieces_[2][0] = new Piece(2, 0, RGB(255,255,0), apt);

	// 3, S piece, green
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 1;    apt[2].y = 1;
	apt[3].x = 2;    apt[3].y = 1;
	pieces_[3][0] = new Piece(3, 0, RGB(120,200,80), apt);

	// 4, Z piece, blue
	apt[0].x = 1;    apt[0].y = 0;
	apt[1].x = 2;    apt[1].y = 0;
	apt[2].x = 0;    apt[2].y = 1;
	apt[3].x = 1;    apt[3].y = 1;
	pieces_[4][0] = new Piece(4, 0, RGB(100,180,255), apt);

	// 5, Square piece, dark blue
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 0;    apt[2].y = 1;
	apt[3].x = 1;    apt[3].y = 1;
	pieces_[5][0] = new Piece(5, 0, RGB(20,100,200), apt);

	// 6, T piece, purple
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 2;    apt[2].y = 0;
	apt[3].x = 1;    apt[3].y = 1;
	pieces_[6][0] = new Piece(6, 0, RGB(220,180,255), apt);

	// Create piece rotations
	rotateAll();
}


PieceSet::~PieceSet(void)
{
	for(int i = 0; i < NUM_PIECES; ++i)
	{
		for(int j = 0; j < NUM_ROTATIONS; ++j)
		{
			delete pieces_[i][j];
		}
	}
}

Piece* PieceSet::getPiece(int id, int rotation) const
{
	if(id >= NUM_PIECES || id < 0 || rotation >= NUM_ROTATIONS || rotation < 0)
		return NULL;
	return pieces_[id][rotation];
}

void PieceSet::rotateAll()
{
	POINT apt[NUM_ROTATIONS];
	for(int i = 0; i < NUM_PIECES; ++i)
	{
		pieces_[i][0]->getBody(apt);
		for(int j = 1; j < NUM_ROTATIONS; ++j)
		{
			rotate(apt);
			if(pieces_[i][j] != 0) delete pieces_[i][j];

			pieces_[i][j] = new Piece(i, j, pieces_[i][0]->getColor(), apt);
		}
	}
}

void PieceSet::rotate(POINT* apt, int numPoints)
{
	int tmp;

	for(int i = 0; i < numPoints; ++i)
	{
		tmp = apt[i].x;
		apt[i].x = -apt[i].y;
		apt[i].y = tmp;
	}
}
