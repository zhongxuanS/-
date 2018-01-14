#include <ctime>
#include "Level.h"
#include "PieceSet.h"

Level::Level(DrawEngine& de, int width, int height)
	:de_(de), width_(width), height_(height), lastTime_(0.0), speed_(500), score_(-1)
{
	//随机数种子
	srand(time(NULL));
	//申请board颜色内存，初始化为黑色
	board_ = new COLORREF*[width_];
	for(int i = 0; i < width_; ++i)
	{
		board_[i] = new COLORREF[height];
		for(int j = 0; j < height_; ++j)
		{
			board_[i][j] = RGB(0, 0, 0);
		}
	}

	//初始化当前和下一个图形
	current_ = NULL;
	next_ = pieceSet_.getRandomPiece();
}


Level::~Level(void)
{
	for(int i = 0; i < width_; ++i)
	{
		delete[] board_[i];
	}
	delete[] board_;
}

//绘制整个操作区
void Level::drawBoard() const
{
	//遍历board把颜色绘制上去
	for(int i = 0; i < width_; ++i)
	{
		for(int j = 0; j < height_; ++j)
		{
			de_.drawBlock(i, j, board_[i][j]);
		}
	}
}

void Level::timerUpdate()
{
	currentTime_ = timeGetTime();
	if(currentTime_ - lastTime_ < speed_)
	{
		return;
	}

	//如果没有正在下落的方块或者已经不能再下落，
	//开始进入清除判断逻辑
	if(current_ == NULL || !move(0, -1))
	{
		int lines = clearRows();
		speed_ = max(speed_ - 2 * lines, 100);
		score_ += 1 + lines * lines * 5;
		dropRandomPiece();
		drawScore();
		drawSpeed();
		drawNextPiece();
	}

	lastTime_ = timeGetTime();
}

bool Level::move(int cxDistance, int cyDistance)
{
	//判断左右/下边界
	if(posX_ + cxDistance < 0 || posX_ + current_->getWidht() + cxDistance > width_
		|| posY_ + cyDistance < 0)
		return false;
	//左边碰撞检测
	if(cxDistance < 0 && isHitLeft())
		return false;
	//右边碰撞检测
	if(cxDistance > 0 && isHitRight())
		return false;
	//下边碰撞检测
	if(cyDistance < 0 && isHitBottom())
		return false;
	//清除当前图形
	clear(*current_);
	//按照新坐标重新绘制
	return place(posX_ + cxDistance, posY_ + cyDistance, *current_);
}

bool Level::isHitBottom() const
{
	POINT apt[4];
	int n = current_->getSkirt(apt);
	//遍历下边缘的点，如果在操作区内且有颜色不为背景色，说明碰到了别的图形
	//这类函数都是用来判断当前图形的状态，因为是在移动之前判断，所以不存在
	//会被移动到操作区以外的范围，但是y轴有可能因为图形可能还没有完全被显示出来
	//上半截在外面
	for(int i = 0; i < n; ++i)
	{
		int x = posX_ + apt[i].x;
		int y = posY_ + apt[i].y;

		if(y >= height_) continue;

		if(y == 0 || board_[x][y - 1] != RGB(0, 0, 0))
			return true;
	}
	return false;
}


bool Level::isHitLeft() const
{
	//遍历左边界点
	POINT apt[4];
	int n = current_->getLeftSide(apt);

	for(int i = 0; i < n; ++i)
	{
		int x = posX_ + apt[i].x;
		int y = posY_ + apt[i].y;

		if(y > height_ - 1) continue;

		if(x == 0 || board_[x - 1][y] != RGB(0, 0, 0))
			return true;
	}

	return false;
}

bool Level::isHitRight() const
{
	POINT apt[4];
	int n = current_->getRightSide(apt);

	for(int i = 0; i < n; ++i)
	{
		int x = posX_ + apt[i].x;
		int y = posY_ + apt[i].y;

		if(y > height_ - 1) continue;

		if(x == width_ - 1 || board_[x + 1][y] != RGB(0, 0, 0))
			return true;
	}

	return false;
}

void Level::clear(const Piece& piece)
{
	POINT apt[4];
	piece.getBody(apt);

	for(int i = 0; i < 4; ++i)
	{
		int x = posX_ + apt[i].x;
		int y = posY_ + apt[i].y;

		if(x >= 0 && x <= width_ - 1
			&& y >= 0 && y <= height_ - 1)
		{
			board_[x][y] = RGB(0, 0, 0);
		}
	}
}

bool Level::place(int x, int y, const Piece& piece)
{
	if(x + piece.getWidht() > width_ || isCovered(piece, x, y))
		return false;

	posX_ = x;
	posY_ = y;

	POINT apt[4];
	piece.getBody(apt);
	COLORREF color = piece.getColor();

	for(int i = 0; i < 4; ++i)
	{
		if(y + apt[i].y > height_ - 1)
			continue;
		board_[x + apt[i].x][y + apt[i].y] = color;
	}

	return true;
}

bool Level::isCovered(const Piece& piece, int x, int y) const
{
	POINT apt[4];
	piece.getBody(apt);

	int tmpX, tmpY;
	for(int i = 0; i < 4; ++i)
	{
		tmpX = x + apt[i].x;
		tmpY = y + apt[i].y;

		if(tmpX > width_ - 1 || tmpY > height_ - 1)
			continue;
		if(board_[tmpX][tmpY] != RGB(0, 0, 0))
			return true;
	}

	return false;
}

void Level::drawSpeed() const
{
	de_.drawSpeed((500 - speed_) / 2, width_ + 1, 12);
}

void Level::drawScore() const
{
	de_.drawScore(score_, width_ + 1, 13);
}

void Level::drawNextPiece() const
{
	de_.drawNextPiece(*next_, width_ + 1, 14);
}

void Level::dropRandomPiece()
{
	current_ = next_;
	next_ = pieceSet_.getRandomPiece();
	place(3, height_ - 1, *current_);
}

//注意消行的话是有可能从中间消行的
//原理就是逐行扫描，有可以消除的行就将其置为背景色，然后把上面的行逐行
//替换到下一行
int Level::clearRows()
{
	int clearRowCount = 0;
	bool isNeedClear;

	for(int y = 0; y < height_; ++y)
	{
		isNeedClear = false;
		for(int x = 0; x < width_; ++x)
		{
			//如果有一个方块颜色是背景色，说明不用消除
			if(board_[x][y] == RGB(0, 0, 0))
			{
				break;
			}

			if(x == width_ - 1)
			{
				isNeedClear = true;
			}
		}

		//当前行需要被消除
		if(isNeedClear)
		{
			//当前行方块颜色都被置为背景色
			for(int x = 0; x < width_; ++x)
			{
				board_[x][y] = RGB(0, 0, 0);
			}

			//下移一行
			for(int clearY = y; clearY < height_ - 1; ++clearY)
			{
				for(int x = 0; x < width_; ++x)
				{
					board_[x][clearY] = board_[x][clearY + 1];
				}
			}

			--y;//因为已经下移一行了
			clearRowCount++;
		}
	}

	return clearRowCount;
}

//注意，旋转图形时候，如果空间不够的话要通过平移来获取足够的空间
bool Level::rotate()
{
	Piece* tmp = current_;

	//旋转的话就是高变成宽
	int disX = max(posX_ + current_->getHeight() - width_, 0);

	int rotation = (current_->getRotation() + 1) % PieceSet::NUM_ROTATIONS;

	//清除当前图形
	clear(*current_);

	//移动当前图形
	current_ = pieceSet_.getPiece(current_->getId(), rotation);

	if(place(posX_ - disX, posY_, *current_))
	{
		return true;
	}

	//如果移动失败，恢复之前的图形
	current_ = tmp;
	place(posX_, posY_, *current_);
	return false;
}


bool Level::isGameOver()
{
	//不包括当前正在下落的图形
	if(current_)
		clear(*current_);

	//判断最顶层的一行是否有非背景色方块
	for(int x = 0; x < width_; ++x)
	{
		if(board_[x][height_ - 1])
		{
			//把当前图形再放回去
			if(current_)
				place(posX_, posY_, *current_);

			return true;
		}
	}

	if(current_ != NULL)
		place(posX_, posY_, *current_);

	return false;
}
