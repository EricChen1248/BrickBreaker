#include <iostream>
#include <Windows.h>
#include <cmath>
#include <time.h>
#include "Drawing.h"
#include "Objects.h"

#using <mscorlib.dll>
using namespace std;
using namespace System;
using namespace Draw;
using namespace Objects;

void CollisionDetectionPaddle(Paddle paddle, Ball& ball, bool &redrawPaddle);
void BorderCollision(Ball &ball, bool godMode, bool &death);
void LoadBricks(Brick Brick[5][5]);
void BrickInitialize(Brick brick[5][5]);
void BrickCollision(Ball &ball, Brick brick[5][5], int &score, PowerUp **list);
void Death(int &lifeCnt, Vector2 lifePos[3], Ball &ball, Paddle &paddle);
void Score(int score, Vector2 scorePosition[6]);
void Game(int level, Brick brick[5][5]);
void resetPowerUp(PowerUp *list[10]);

int main(int argc, const char * argv[])
{
	HDC device = GetDC(GetConsoleWindow());
	Console::SetWindowSize(console_width, console_height);
	setConsole();
	srand((int)time(NULL));
	Paddle paddle;
	Ball ball;
	Brick brick[5][5];
	int lifeCount = 3;
	int level = 1;
	int levelScore = 0;
	bool death = false;
	bool redrawPaddle = false;
	bool godMode = false;
	int score = 0, oldScore = 0;
	PowerUp **powerupList = new  PowerUp*[10];
	for (int i = 0; i < 10; i++)
		powerupList[i] = nullptr;

	//Interface Layout
	Vector2 scorePosition[6];
	Vector2 sLetterPosition[6];
	Vector2 lLetterPosition[5];
	Vector2 lifePos[3];
	for (int i = 0; i < 6; i++)
	{
		sLetterPosition[i].y = topBorder - 25;
		sLetterPosition[i].x = leftBorder + i * 15 + 15;
	}
	for (int i = 0; i < 3; i++)
	{
		lifePos[i].y = topBorder - 17;
		lifePos[i].x = leftBorder + 315 + i * 15;
		drawBall(lifePos[i], 3);
	}
	for (int i = 0; i < 6; i++)
	{
		lLetterPosition[i].y = topBorder - 25;
		lLetterPosition[i].x = leftBorder + i * 15 + 235;
	}
	for (int i = 0; i < 6; i++)
	{
		scorePosition[i].y = topBorder - 25;
		scorePosition[i].x = leftBorder + i * 15 + 105;
		drawDigit(scorePosition[i], 0);
	}
	{
	drawLetter(sLetterPosition[0], 'S', red);
	drawLetter(sLetterPosition[1], 'C', green);
	drawLetter(sLetterPosition[2], 'O', blue);
	drawLetter(sLetterPosition[3], 'R', purple);
	drawLetter(sLetterPosition[4], 'E', yellow);
	drawLetter(sLetterPosition[5], ':');
	drawLetter(lLetterPosition[0], 'L',red);
	drawLetter(lLetterPosition[1], 'I',green);
	drawLetter(lLetterPosition[2], 'F',blue);
	drawLetter(lLetterPosition[3], 'E',purple);
	drawLetter(lLetterPosition[4], ':');
	}

	BrickInitialize(brick);
	while (lifeCount >= 0)
	{
		resetPowerUp(powerupList);
		paddle.reset();
		ball.reset();
		Game(level, brick);
		LoadBricks(brick);

		//drawHeart();

		//Wait for user to start
		while (true)
			if (GetAsyncKeyState(VK_SPACE))
				break;

		while (lifeCount >= 0 && Brick::brickCnt > 0)				//End game condition
		{
			ball.move();
			for (int i = 0; i < 10; i++)
				if (powerupList[i] != nullptr)
					powerupList[i]->drop(powerupList);

			if (redrawPaddle)
			{
				paddle.Redraw();
				redrawPaddle = false;
			}
			CollisionDetectionPaddle(paddle, ball, redrawPaddle);
			BorderCollision(ball, godMode, death);
			BrickCollision(ball, brick, score, powerupList);
			if (score != oldScore)
			{
				Score(score, scorePosition);
				oldScore = score;
			}
			//Paddle Control
			if (GetAsyncKeyState(VK_RIGHT))
				paddle.move(7);
			if (GetAsyncKeyState(VK_LEFT))
				paddle.move(-7);

			if (death)
			{
				Death(lifeCount, lifePos, ball, paddle);
				death = false;
			}

			Sleep(20);		//Game refresh interval
		}
		score += (score - levelScore) * level * (lifeCount+1);
		Score(score,scorePosition);
		level++;
	}

	system("Pause");
	return 0;
}


void Game1(Brick Bricks[5][5])
{
	for (int i = 0; i < 5; i++)
	{
		Bricks[0][i].setHitPoints(1);
		Bricks[2][i].setHitPoints(1);
		Bricks[4][i].setHitPoints(1);
	}
	for (int i = 0; i < 4; i++)
	{
		Bricks[1][i].setHitPoints(1);
		Bricks[3][i].setHitPoints(1);
	}
	Brick::brickCnt = 23;
	return;
}

//Initialization
void LoadBricks(Brick Brick[5][5])
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			if (Brick[i][j].getHitPoints() > 0)
				Brick[i][j].draw();
}
void BrickInitialize(Brick brick[5][5])
{

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			brick[2 * i][j].setY(topBorder + 30 + 18 * (2 * i + 1));
			brick[2 * i][j].setX(leftBorder + 40 + 56 * j);
		}
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			brick[2 * i + 1][j].setY(topBorder + 30 + 18 * (2 * i + 2));
			brick[2 * i + 1][j].setX(leftBorder + 68 + 56 * (j));
		}
	}
}
void resetPowerUp(PowerUp *list[10])
{
	for (int i = 0; i < 10; i++)
		if (list[i] != nullptr)
		{
			list[i]->reset();
			delete list[i];
			list[i] = nullptr;
		}
}

//Collision Functions
void CollisionDetectionPaddle(Paddle paddle, Ball &ball, bool &redrawPaddle)
{
	if (ball.getPosition().y <= paddle.getPosition().y && ball.getPosition().y + ball.getSpeed().y >= paddle.getPosition().y)
		if ((ball.getPosition().x + ball.getRadius() >= paddle.getPosition().x || ball.getPosition().x + ball.getSpeed().x + ball.getRadius()>= paddle.getPosition().x)
			&& (ball.getPosition().x <= paddle.getPosition().x + paddle.getSize().x / 2 || ball.getPosition().x + ball.getSpeed().x <= paddle.getPosition().x + paddle.getSize().x / 2))
		{
			ball.verticalRebound();
			ball.shiftLeft();
			redrawPaddle = true;
		}
		else if ((ball.getPosition().x >= paddle.getPosition().x + paddle.getSize().x / 2 || ball.getPosition().x + ball.getSpeed().y >= paddle.getPosition().x + paddle.getSize().x / 2)
			&& (ball.getPosition().x - ball.getRadius()<= paddle.getPosition().x + paddle.getSize().x || ball.getPosition().x + ball.getSpeed().x - ball.getRadius()<= paddle.getPosition().x + paddle.getSize().x))
		{
			ball.verticalRebound();
			ball.shiftRight();
			redrawPaddle = true;
		}
}
void BorderCollision(Ball &ball, bool godMode, bool &death)
{
	if (ball.getPosition().x + ball.getSpeed().x + ball.getRadius() >= rightBorder || ball.getPosition().x + ball.getSpeed().x - ball.getRadius() <= leftBorder)
	{
		ball.horizontalRebound();
		drawBorder();
	}
	if ( ball.getPosition().y + ball.getSpeed().y - ball.getRadius() <= topBorder)
	{
		ball.verticalRebound();
		drawBorder();
	}
	if (ball.getPosition().y + ball.getSpeed().y + ball.getRadius() >= bottomBorder)
	{
		if (godMode)
		{
			ball.verticalRebound();
			drawBorder();
		}
		else
			death = true;
	}
	return;
}
void BrickCollision(Ball &ball, Brick brick[5][5], int &score, PowerUp **list)
{
	bool collision = false;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			//Vertical Collision
			if ((ball.getPosition().x + ball.getRadius() + 1>= brick[i][j].getPosition().x || ball.getPosition().x + ball.getSpeed().x + ball.getRadius() + 1>= brick[i][j].getPosition().x)
				&& (ball.getPosition().x - ball.getRadius()  - 1 <= brick[i][j].getPosition().x + Brick::size.x || ball.getPosition().x + ball.getSpeed().x - ball.getRadius() - 1 <= brick[i][j].getPosition().x + Brick::size.x))
			{
				//Collision from top / bottom
				if (ball.getPosition().y + ball.getRadius() - 1 <= brick[i][j].getPosition().y && ball.getPosition().y + ball.getSpeed().y + ball.getRadius() + 1 > brick[i][j].getPosition().y 
					|| ball.getPosition().y - ball.getRadius() +1 >= brick[i][j].getPosition().y + Brick::size.y && ball.getPosition().y + ball.getSpeed().y - ball.getRadius() - 1 < brick[i][j].getPosition().y + Brick::size.y)
					if (brick[i][j].getHitPoints() > 0)
					{
						brick[i][j].hit(ball, score, list);
						ball.verticalRebound();
						collision = true;
					}
			}
			//Horizontal Collision
			else if ((ball.getPosition().y + ball.getRadius()  + 1>= brick[i][j].getPosition().y || ball.getPosition().y + ball.getSpeed().y + ball.getRadius() + 1>= brick[i][j].getPosition().y)
				&& (ball.getPosition().y  - ball.getRadius()  - 1<= brick[i][j].getPosition().y + Brick::size.y || ball.getPosition().y + ball.getSpeed().y - ball.getRadius() - 1 <= brick[i][j].getPosition().y + Brick::size.y))
			{
				//Collision from left / right
				if (ball.getPosition().x + ball.getRadius() - 1 <= brick[i][j].getPosition().x && ball.getPosition().x + ball.getSpeed().x + ball.getRadius() + 1 > brick[i][j].getPosition().x
					|| ball.getPosition().x - ball.getRadius() + 1 >= brick[i][j].getPosition().x + Brick::size.x && ball.getPosition().x + ball.getSpeed().x - ball.getRadius() - 1< brick[i][j].getPosition().x + Brick::size.x)
				{
					if (brick[i][j].getHitPoints() > 0)
					{
						brick[i][j].hit(ball, score, list);
						ball.horizontalRebound();
						collision = true;
					}
				}
			}

			for (int k = 0; k < 10; k++)
				if (list[k] != nullptr)
					if (brick[i][j].getHitPoints() > 0)
						if (list[k]->getLocation().y >= brick[i][j].getPosition().y  && list[k]->getLocation().y <= brick[i][j].getPosition().y + Brick::size.y)
							if (list[k]->getLocation().x >= brick[i][j].getPosition().x  && list[k]->getLocation().x <= brick[i][j].getPosition().x + Brick::size.x)
								brick[i][j].draw();

			if (collision)
				BrickCollision(ball, brick, score, list);
		}

}

void Death(int &lifeCnt, Vector2 lifePos[3], Ball &ball, Paddle &paddle)
{
	lifeCnt--;
	clearBall(lifePos[lifeCnt], 3);
	ball.reset();
	paddle.reset();

	while (lifeCnt >= 0)
		if (GetAsyncKeyState(VK_SPACE))
			break;
	return;
}
void Score(int score, Vector2 scorePosition[6])
{
	static int digits[6] = { 0 };
	int digit;
	for (int i = 0; i < 6; i++)
	{
		digit = score % 10;
		if (digits[5 - i] != digit)
		{
			digits[5 - i] = digit;
			drawDigit(scorePosition[5 - i], digit);
		}
			score /= 10;
	}
}

void Game(int level, Brick brick[5][5])
{
	switch (level)
	{
	case 1:
		Game1(brick);
	case 2:
		Game1(brick);
	case 3:
		Game1(brick);
	case 4:
		Game1(brick);
	case 5:
		Game1(brick);
	default:
		break;
	}
}