#pragma once
#include "DrawEngine.h"
#include "Level.h"

class Game
{
public:
	Game(DrawEngine& de);
	~Game(void);

	void restart();

	bool keyPress(int vk);

	void timerUpdate();

	void pause(bool paused);

	void repaint() const;

	bool isGameOver() const;

protected:
	inline void drawGameOver() const
	{
		de_.drawText(TEXT("GAME OVER"), 3, 10);
		de_.drawText(TEXT("Press ENTER to restart"), 2, 9);
	}

	inline void drawPause() const
	{
		de_.drawText(TEXT("PAUSE"), 4, 10);
		de_.drawText(TEXT("Press PAUSE again to continue"), 1, 9);
	}

	Level* level_;
	DrawEngine& de_;

	bool isPaused_;
};
