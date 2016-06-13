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
void clearBrickArea(Brick Brick[5][5]);

int main(int argc, const char * argv[])
{
	bool alphabet[26] = { false };
	bool godMode = false;
	bool speed = false;
	bool heart = false;

	int cmdTime = 0;
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
	int score = 0, oldScore = 0;
	PowerUp **powerupList = new  PowerUp*[10];
	for (int i = 0; i < 10; i++)
		powerupList[i] = nullptr;

	//Interface Layout
	Vector2 scorePosition[8];
	Vector2 sLetterPosition[6];
	Vector2 lLetterPosition[6];
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
	for (int i = 0; i < 8; i++)
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
			BrickCollision(ball, brick, score, powerupList);
			BorderCollision(ball, godMode, death);
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

			for (int i = 0; i < 26; i++)
				if (GetAsyncKeyState(i + 65))
				{
					alphabet[i] = true;
					cmdTime = 50;
				}

			if (cmdTime > 0)
				cmdTime--;
			else
				for (int i = 0; i < 26; i++)
					alphabet[i] = false;

			//Special Commands / Easter Eggs
			//GodMode
			if (alphabet[6]  && alphabet[14]  && alphabet[3]  && alphabet[12]  && alphabet[4] )
			{
				alphabet[6] = false;
				alphabet[14] = false;
				alphabet[3] = false;
				alphabet[12] = false;
				alphabet[4] = false;
				godMode = !godMode;
			}
			//Speed
			if (alphabet[4]  && alphabet[18]  && alphabet[3]  && alphabet[15])
			{
				alphabet[4] = false;
				alphabet[18] = false;
				alphabet[3] = false;
				alphabet[15] = false;
				if (speed )
					ball.endGod();
				else
					ball.godMode();
				speed = !speed;
			}
			//Heart
			if (alphabet[7] && alphabet[4] && alphabet[0] && alphabet[19] && alphabet[17])
			{
				alphabet[4] = false;
				alphabet[7] = false;
				alphabet[0] = false;
				alphabet[19] = false;
				alphabet[17] = false;
				heart = !heart;
				if (heart)
					drawHeart();
				else
					drawHeart(consoleColor);
			}
			//NextLevel
			if (alphabet[13] && alphabet[4] && alphabet[23] && alphabet[19] && alphabet[11] && alphabet[21])
			{
				alphabet[4] = false;
				alphabet[13] = false;
				alphabet[23] = false;
				alphabet[19] = false;
				alphabet[11] = false;
				alphabet[21] = false;
				Brick::brickCnt = 0;

			}


			if (death)
			{
				Death(lifeCount, lifePos, ball, paddle);
				death = false;
				speed = false;
			}

			Sleep(20);		//Game refresh interval
		}
		score += (score - levelScore) * level *(lifeCount+1);
		levelScore = score;
		Score(score,scorePosition);
		level++;
		speed = false;
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
void Game2(Brick Bricks[5][5])
{
	Bricks[0][1].setHitPoints(6);
	Bricks[0][3].setHitPoints(6);
	Bricks[1][0].setHitPoints(6);
	Bricks[1][3].setHitPoints(6);
	Bricks[2][2].setHitPoints(1);
	Bricks[2][1].setHitPoints(6);
	Bricks[2][3].setHitPoints(6);
	Bricks[3][1].setHitPoints(6);
	Bricks[3][2].setHitPoints(6);
	Bricks[0][1].moveX(6);
	Bricks[0][3].moveX(-6);
	Brick::brickCnt = 1;
	return;
}
void Game3(Brick Bricks[5][5])
{
	Bricks[0][1].setHitPoints(3);
	Bricks[0][3].setHitPoints(3);
	Bricks[1][1].setHitPoints(3);
	Bricks[1][3].setHitPoints(3);
	Bricks[2][2].setHitPoints(3);
	Bricks[3][0].setHitPoints(3);
	Bricks[3][4].setHitPoints(3);
	Bricks[4][1].setHitPoints(3);
	Bricks[4][2].setHitPoints(3);
	Bricks[4][3].setHitPoints(3);
	Bricks[1][1].moveX(-Brick::size.x / 2 - 1);
	Bricks[1][3].moveX(-Brick::size.x / 2 - 1);
	Bricks[3][0].moveX(-Brick::size.x / 2 - 1);
	Bricks[3][4].moveX(-Brick::size.x / 2 - 1);

	Brick::brickCnt = 10;
	return;
}
void Game4(Brick Bricks[5][5])
{
	Bricks[0][2].setHitPoints(5);
	Bricks[1][2].setHitPoints(6);
	Bricks[2][2].setHitPoints(6);
	Bricks[3][1].setHitPoints(6);
	Bricks[3][2].setHitPoints(6);
	Bricks[3][3].setHitPoints(6);
	Bricks[4][0].setHitPoints(6);
	Bricks[4][2].setHitPoints(6);
	Bricks[4][4].setHitPoints(6);
	Bricks[1][2].moveX(-Brick::size.x / 2 - 1);
	Bricks[3][2].moveX(-Brick::size.x / 2 - 1);
	Bricks[3][1].moveX(-Brick::size.x / 2 - 1);
	Bricks[3][3].moveX(-Brick::size.x / 2 - 1);
	Bricks[4][0].moveX(18);
	Bricks[4][4].moveX(-18);
	Brick::brickCnt = 1;
	return;
}
void Game5(Brick Bricks[5][5])
{
	for (int i = 0; i < 5; i++)
	{
		Bricks[2][i].setHitPoints(6);
	}
	Bricks[0][2].setHitPoints(5);
	Bricks[1][1].setHitPoints(4);
	Bricks[1][2].setHitPoints(4);
	Bricks[3][1].setHitPoints(4);
	Bricks[3][2].setHitPoints(4);
	Bricks[4][2].setHitPoints(5);
	Brick::brickCnt = 6;
	return;
}
void Game6(Brick Bricks[5][5])
{
	for (int i = 1; i < 4; i++)
	{
		Bricks[0][i].setHitPoints(6);
		Bricks[4][i].setHitPoints(6);
	}
	for (int i = 0; i < 5; i++)
	{
		Bricks[1][i].setHitPoints(2);
		Bricks[2][i].setHitPoints(2);
		Bricks[3][i].setHitPoints(2);
		Bricks[1][i].moveX(-Brick::size.x / 2 - 1);
		Bricks[3][i].moveX(-Brick::size.x / 2 - 1);
	}
	Brick::brickCnt = 15;
	return;
}
void Game7(Brick Bricks[5][5])
{
	for (int i = 2; i < 5; i++)
	{
		Bricks[0][i].setHitPoints(5);
	}
	Bricks[1][1].setHitPoints(5);
	Bricks[2][1].setHitPoints(5);
	Bricks[3][0].setHitPoints(5);
	Brick::brickCnt = 6;
	return;
}
void Game8(Brick Bricks[5][5])
{
	for (int i = 0; i < 3; i ++)
	{
		Bricks[i * 2][0].setHitPoints(6);
		Bricks[i * 2][4].setHitPoints(6);
	}
	for (int i = 0; i < 2 ; i ++)
	{
		Bricks[2 * i + 1][0].setHitPoints(6);
		Bricks[2 * i + 1][3].setHitPoints(6);
	}
	for (int i = 1; i < 4; i++)
	{
		Bricks[0][i].setHitPoints(1);
		Bricks[2][i].setHitPoints(3);
		Bricks[4][i].setHitPoints(5);
	}
	for (int i = 1; i < 3; i++)
	{
		Bricks[1][i].setHitPoints(2);
		Bricks[3][i].setHitPoints(4);
	}
	Brick::brickCnt = 13;
	return;
}
void Game9(Brick Bricks[5][5])
{
	for (int i = 1; i < 5; i++)
	{
		Bricks[0][i].setHitPoints(6);
	}
	for (int i = 0; i < 5; i++)
	{
		Bricks[i][0].setHitPoints(6);
		Bricks[i][4].setHitPoints(6);
		Bricks[1][i].moveX(-Brick::size.x / 2 - 1);
		Bricks[3][i].moveX(-Brick::size.x / 2 - 1);
	}
	for (int i = 1; i < 4; i++)
	{
		Bricks[1][i].setHitPoints(1);
		Bricks[2][i].setHitPoints(2);
		Bricks[3][i].setHitPoints(3);
		Bricks[4][i].setHitPoints(4);
	}
	Bricks[2][2].setHitPoints(6);
	Brick::brickCnt = 11;
	return;
}
void Game10(Brick Bricks[5][5])
{
	for (int i = 0; i < 5; i++)
	{
		Bricks[1][i].moveX(-Brick::size.x / 2 - 1);
		Bricks[3][i].moveX(-Brick::size.x / 2 - 1);
	}
	Bricks[0][1].setHitPoints(3);
	Bricks[0][3].setHitPoints(3);
	Bricks[1][0].setHitPoints(3);
	Bricks[1][2].setHitPoints(3);
	Bricks[1][4].setHitPoints(3);
	Bricks[2][0].setHitPoints(3);
	Bricks[2][4].setHitPoints(3);
	Bricks[3][1].setHitPoints(3);
	Bricks[3][3].setHitPoints(3);
	Bricks[4][2].setHitPoints(3);
	Brick::brickCnt = 10;
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
		for (int j = 0; j < 5; j++)
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
void Score(int score, Vector2 scorePosition[8])
{
	static int digits[8] = { 0 };
	int digit;
	for (int i = 0; i < 8; i++)
	{
		digit = score % 10;
		if (digits[7 - i] != digit)
		{
			digits[7 - i] = digit;
			drawDigit(scorePosition[7 - i], digit);
		}
			score /= 10;
	}
}

void Game(int level, Brick brick[5][5])
{
	clearBrickArea(brick);
	BrickInitialize(brick);
	switch (level)
	{
	case 1:
		Game1(brick);
		break;
	case 2:
		Game2(brick);
		break;
	case 3:
		Game3(brick);
		break;
	case 4:
		Game4(brick);
		break;
	case 5:
		Game5(brick);
		break;
	case 6:
		Game6(brick);
		break;
	case 7:
		Game7(brick);
		break;
	case 8:
		Game8(brick);
		break;
	case 9:
		Game9(brick);
		break;
	case 10:
		Game10(brick);
		break;
	default:
		break;
	}
}

void clearBrickArea(Brick Brick[5][5])
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			clearBrick(Brick[i][j].getPosition(), Brick::size);
			Brick[i][j].setHitPoints(0);
		}
}

