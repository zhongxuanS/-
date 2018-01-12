#include "Game.h"


Game::Game(DrawEngine& de)
	:de_(de), isPaused_(false)
{
	level_= new Level(de, 10, 20);
}


Game::~Game(void)
{
	delete level_;
}

void Game::restart()
{
	delete level_;
	level_ = new Level(de_, 10, 20);
	isPaused_ = false;
	repaint();
}

bool Game::keyPress(int vk)
{
	if(isPaused_ && vk != VK_PAUSE && vk != VK_RETURN)
		return false;

	switch (vk)
	{
	case VK_UP:
		level_->rotate();
		break;
	case VK_DOWN:
		level_->move(0, -1);
		break;
	case VK_LEFT:
		level_->move(-1, 0);
		break;
	case VK_RIGHT:
		level_->move(1, 0);
		break;
	case VK_SPACE:
		level_->rotate();
		break;
	case VK_PAUSE:
		pause(!isPaused_);
		break;
	case VK_RETURN:
		if(level_->isGameOver())
			restart();
	default:
		return false;
	}
	return true;
}

void Game::timerUpdate()
{
	if(isPaused_)
		return;

	if(level_->isGameOver())
	{
		isPaused_ = true;
		drawGameOver();
		return;
	}

	level_->timerUpdate();

	level_->drawBoard();
}

void Game::pause(bool paused)
{
	if(isGameOver())
		return;

	isPaused_ = paused;
	if(paused)
	{
		drawPause();
	}

	level_->drawScore();
	level_->drawSpeed();
}

void Game::repaint() const
{
	de_.drawInterface();
	level_->drawScore();
	level_->drawSpeed();
	level_->drawNextPiece();
	level_->drawBoard();
	if (level_->isGameOver())
		drawGameOver();
	else if (isPaused_)
		drawPause();
}

bool Game::isGameOver() const
{
	return level_->isGameOver();
}
