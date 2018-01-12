#pragma once
#include "DrawEngine.h"
#include "PieceSet.h"

class Level
{
public:
	Level(DrawEngine& de, int width = 10, int height= 20);
	~Level(void);

	//绘制操作区，原理是将操作区分成10*20个空格，每个空格都填入对应的颜色
	void drawBoard() const;

	//旋转当前图形
	bool rotate();

	//移动当前图形，cxDistance/cyDistance分别表示在x/y轴的移动量
	//按照当前正在移动图形的坐标分别与之相加
	//x轴正方向为右，y轴正方向为上
	bool move(int cxDistance, int cyDistance);

	//定时器对应的回调函数
	//在这里做帧画面的更新，包括分数、速度等
	void timerUpdate();

	bool isGameOver();

	void drawSpeed() const;
	void drawScore() const;
	void drawNextPiece() const;

protected:
	//在x和y位置放置piece，原理就是以x和y为原点，把piece的
	//颜色画出来
	bool place(int x, int y, const Piece& piece);

	//清除piece的颜色达到清除图形的目的
	void clear(const Piece& piece);

	//下一个图形变成当前图形，并生成下一个随机图形
	void dropRandomPiece();

	bool isHitBottom() const;
	bool isHitLeft() const;
	bool isHitRight() const;

	//检查piece移动到x，y的话是否会与别的图形重叠
	bool isCovered(const Piece& piece, int x, int y) const;

	//清除满行，返回已经被清楚的行数
	int clearRows();

	COLORREF** board_;//保存操作区的每个方块的颜色
	DrawEngine& de_;//画图引擎
	PieceSet pieceSet_;//保存所有图形7*4个
	Piece* current_;//当前正在下落的图形
	Piece* next_;//下一个图形
	int width_;//操作区宽
	int height_;//操作区高
	int posX_;//当前正在下落图形左下角x轴坐标
	int posY_;//当前正在下落图形左下角y轴坐标
	int speed_;//当前速度
	double lastTime_;//上一个更新时间
	double currentTime_;//当前更新时间
	int score_;//当前得分
};
