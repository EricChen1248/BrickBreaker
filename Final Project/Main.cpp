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


void LoadBricks(Brick Brick[5][5]);																		//Initialize Level, Draws Bricks
void BrickInitialize(Brick brick[5][5]);																	//Reset Brick Location
void resetPowerUp(PowerUp *list[10]);																	//Clears Powerup List
void clearBrickArea(Brick Brick[5][5]);																//Remove List Area

void CollisionDetectionPaddle(Paddle paddle, Ball&, bool &redrawPaddle);					//Paddle Collision
void BorderCollision(Ball &, bool godMode, bool &death);											//Border Collision
void BrickCollision(Ball &, Brick brick[5][5], int &score, PowerUp **list);					//Brick Collision
void powerupHit(PowerUp* list[], Paddle&, Ball&, int &lifeCnt, Vector2 lifePos[]);		//Powerup Collision

void Death(int &lifeCnt, Vector2 lifePos[3], Ball&, Paddle&, PowerUp **list);				//Death Handling
void Score(int score, Vector2 scorePosition[7]);														//Score Update

void Levels(int level, Brick brick[5][5]);																//Level Initialization



int main(int argc, const char * argv[])
{
	//Console Initialization
	HDC device = GetDC(GetConsoleWindow());
	Console::SetWindowSize(console_width, console_height);
	srand((int)time(NULL));
	//End CI

	bool endLevel = false;																					//Player's choice on reset
	while (true)
	{
		bool alphabet[26] = { false };																		//Holds Keystroke input

		//Special Command Holders
		bool godMode = false;
		bool speed1 = false;
		bool speed2 = false;
		bool heart = false;
		int cmdTime = 0;
		//End SPH

		//Statistic Holders
		int lifeCount = 3;
		int level = 1;
		int levelScore = 0;
		int score = 0, oldScore = 0;
		//End SH

		//Game Condition Bool
		bool death = false;
		bool redrawPaddle = false;
		//End GCB

		//Interface Layout
		setConsole();
		Brick brick[5][5];
		BrickInitialize(brick);
		Vector2 scorePosition[7];
		Vector2 lifePos[3];
		for (int i = 0; i < 3; i++)
		{
			lifePos[i].y = topBorder - 17;
			lifePos[i].x = leftBorder + 315 + i * 15;
			drawBall(lifePos[i], 3);
		}
		for (int i = 0; i < 7; i++)
		{
			scorePosition[i].y = topBorder - 25;
			scorePosition[i].x = leftBorder + i * 15 + 105;
			drawDigit(scorePosition[i], 0);
		}

		//End IL

		while (lifeCount >= 0)
		{
			//Object Initialization
			Paddle paddle;
			Ball ball;
			PowerUp **powerupList = new  PowerUp*[10];
			for (int i = 0; i < 10; i++)
				powerupList[i] = nullptr;
			//End OI

			resetPowerUp(powerupList);
			paddle.reset();
			ball.reset();
			Levels(level, brick);
			LoadBricks(brick);

			//Wait for user to start
			while (true)
			{
				if (GetAsyncKeyState(VK_RIGHT))
				{
					paddle.move(7);
					ball.shiftBall(paddle.getPosition().x + paddle.getSize().x/2 + ball.getRadius()/2);
				}
				if (GetAsyncKeyState(VK_LEFT))
				{
					paddle.move(-7);
					ball.shiftBall(paddle.getPosition().x + paddle.getSize().x / 2 + ball.getRadius()/2);
				}

				if (GetAsyncKeyState(VK_SPACE))
					break;

				Sleep(20);
			}

			while (lifeCount >= 0 && Brick::brickCnt > 0)				//End Level condition
			{
				//Game Updates
				ball.move();
				paddle.moveMissile(brick);
				for (int i = 0; i < 10; i++)
					if (powerupList[i] != nullptr)
						powerupList[i]->drop(powerupList);
				if (redrawPaddle)
				{
					paddle.Redraw();
					redrawPaddle = false;
				}
				//End Game Updates									

				//Collision Detections
				BorderCollision(ball, godMode, death);
				CollisionDetectionPaddle(paddle, ball, redrawPaddle);
				BrickCollision(ball, brick, score, powerupList);
				powerupHit(powerupList, paddle, ball, lifeCount, lifePos);
				//End Collision Detection

				//Update Score
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
				//End Paddle Control

				//Capture Command Keystrokes
				for (int i = 0; i < 26; i++)
					if (GetAsyncKeyState(i + 65))
					{
						alphabet[i] = true;
						cmdTime = 50;
					}

				//Limit Command Time
				if (cmdTime > 0)
					cmdTime--;
				else
					for (int i = 0; i < 26; i++)
						alphabet[i] = false;

				//Special Commands / Easter Eggs
				if (GetAsyncKeyState(VK_RETURN))
				{
					//GodMode
					if (alphabet[6] && alphabet[14] && alphabet[3]&& alphabet[12]  && alphabet[4] )
					{
						godMode = !godMode;
					}
					//Speed
					if (alphabet[4]  && alphabet[18]  && alphabet[3] && alphabet[15] )
					{
						if (speed2)
						{
							ball.endGod();
							speed2 = false;
						}
						else if (speed1)
						{
							ball.godMode();
							speed1 = false;
							speed2 = true;
						}
						else
						{
							ball.godMode();
							speed1 = true;
						}
					}
					//Heart
					if (alphabet[7]  && alphabet[4]  && alphabet[0]  && alphabet[19]  && alphabet[17] )
					{
						if (heart = !heart)
							drawHeart();
						else
							drawHeart(consoleColor);
					}
					//NextLevel
					if (alphabet[13]  && alphabet[4] && alphabet[23]  && alphabet[19]  && alphabet[11]  && alphabet[21] )
					{
						Brick::brickCnt = 0;
					}
					
					for (int i = 0; i < 26; i++)
						alphabet[i] = 0;
				}

				//Death Handling
				if (death)
				{
					Death(lifeCount, lifePos, ball, paddle, powerupList);
					death = false;
					speed1 = false;
					speed2 = false;
				}

				Sleep(20);		//Level refresh interval
			}

			//End Level, Reinitializes, Updates Score.
			score += (score - levelScore) * level *(lifeCount + 1);
			levelScore = score;
			Score(score, scorePosition);
			++level;
			speed1 = false;
			speed2 = false;
		}

		clearBrickArea(brick);
		endGame();
		//End Game, user decision to reset
		while (true)
		{
			if (GetAsyncKeyState('Y'))
				break;
			if (GetAsyncKeyState('N'))
			{
				endLevel = true;
				break;
			}
		}
	}
	return 0;
}

//Level Data
void Level1(Brick Bricks[5][5])
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
void Level2(Brick Bricks[5][5])
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
	Brick::brickCnt = 1;
	return;
}
void Level3(Brick Bricks[5][5])
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
void Level4(Brick Bricks[5][5])
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
void Level5(Brick Bricks[5][5])
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
void Level6(Brick Bricks[5][5])
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
void Level7(Brick Bricks[5][5])
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
void Level8(Brick Bricks[5][5])
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
void Level9(Brick Bricks[5][5])
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
void Level10(Brick Bricks[5][5])
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
void Levels(int level, Brick brick[5][5])
{
	clearBrickArea(brick);
	BrickInitialize(brick);
	switch (level)
	{
	case 1:
		Level1(brick);
		break;
	case 2:
		Level2(brick);
		break;
	case 3:
		Level3(brick);
		break;
	case 4:
		Level4(brick);
		break;
	case 5:
		Level5(brick);
		break;
	case 6:
		Level6(brick);
		break;
	case 7:
		Level7(brick);
		break;
	case 8:
		Level8(brick);
		break;
	case 9:
		Level9(brick);
		break;
	case 10:
		Level10(brick);
		break;
	default:
		break;
	}
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
void clearBrickArea(Brick Brick[5][5])
{
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			clearBrick(Brick[i][j].getPosition(), Brick::size);
			Brick[i][j].setHitPoints(0);
		}
}

//Collision Functions
void CollisionDetectionPaddle(Paddle paddle, Ball &ball, bool &redrawPaddle)
{
	if (ball.getPosition().y - ball.getRadius() <= paddle.getPosition().y && ball.getPosition().y + ball.getSpeed().y + ball.getRadius() >= paddle.getPosition().y)
		if ((ball.getPosition().x + ball.getRadius() >= paddle.getPosition().x || ball.getPosition().x + ball.getSpeed().x + ball.getRadius() >= paddle.getPosition().x)
			&& ball.getPosition().x <= paddle.getPosition().x + paddle.getSize().x / 2)
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
	if (ball.getPosition().x + ball.getRadius() + 1>= rightBorder || ball.getPosition().x - ball.getRadius()  - 1<= leftBorder)
	{
		ball.horizontalRebound();
		drawBorder();
	}
	if ( ball.getPosition().y + ball.getSpeed().y - ball.getRadius() - 1 <= topBorder)
	{
		ball.verticalRebound();
		drawBorder();
	}
	if (ball.getPosition().y + ball.getSpeed().y + ball.getRadius()  + 1 >= bottomBorder)
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
				if ((ball.getPosition().x + ball.getRadius() + 1 >= brick[i][j].getPosition().x )
					&& (ball.getPosition().x - ball.getRadius() - 1 <= brick[i][j].getPosition().x + Brick::size.x ))
				{
					//Collision from top / bottom
					if (ball.getPosition().y - ball.getRadius() - 1 <= brick[i][j].getPosition().y && ball.getPosition().y + ball.getSpeed().y + ball.getRadius() + 1 >= brick[i][j].getPosition().y
						|| ball.getPosition().y + ball.getRadius() + 1 >= brick[i][j].getPosition().y + Brick::size.y && ball.getPosition().y + ball.getSpeed().y - ball.getRadius() - 1 <= brick[i][j].getPosition().y + Brick::size.y)
						if (brick[i][j].getHitPoints() > 0)
						{
							brick[i][j].hit(ball, score, list);
							ball.verticalRebound();
							collision = true;
						}
				}
				//Horizontal Collision
				if ((ball.getPosition().y + ball.getRadius() + 1 >= brick[i][j].getPosition().y )
					&& (ball.getPosition().y - ball.getRadius() - 1 <= brick[i][j].getPosition().y + Brick::size.y))
				{
					//Collision from left / right
					if (ball.getPosition().x + ball.getRadius() - 1 <= brick[i][j].getPosition().x && ball.getPosition().x + ball.getSpeed().x + ball.getRadius() + 1 >= brick[i][j].getPosition().x
						|| ball.getPosition().x - ball.getRadius() + 1 >= brick[i][j].getPosition().x + Brick::size.x && ball.getPosition().x + ball.getSpeed().x - ball.getRadius() - 1<= brick[i][j].getPosition().x + Brick::size.x)
					{
						if (brick[i][j].getHitPoints() > 0)
						{
							brick[i][j].hit(ball, score, list);
							ball.horizontalRebound();
							collision = true;
						}
					}
				}

			if (collision)
				BrickCollision(ball, brick, score, list);

			for (int k = 0; k < 10; k++)
				if (list[k] != nullptr)
					if (brick[i][j].getHitPoints() > 0)
						if (list[k]->getLocation().y >= brick[i][j].getPosition().y  && list[k]->getLocation().y + 2 <= brick[i][j].getPosition().y + Brick::size.y)
							if (list[k]->getLocation().x + 1 >= brick[i][j].getPosition().x && list[k]->getLocation().x - 1<= brick[i][j].getPosition().x + Brick::size.x)
								brick[i][j].draw();
			 
		}
	}

void powerupHit(PowerUp* list[], Paddle &paddle, Ball &ball, int &lifeCnt, Vector2 lifePos[])
{
	for (int i = 0; i < 10; i++)
		if (list[i] != nullptr)
			if (list[i]->getLocation().y <= paddle.getPosition().y && list[i]->getLocation().y  +  2 >=  paddle.getPosition().y)
				if (list[i]->getLocation().x >= paddle.getPosition().x && list[i]->getLocation().x <= paddle.getPosition().x + paddle.getSize().x)
				{
					list[i]->hit(lifeCnt, lifePos, ball, paddle);
					list[i]->reset();
					paddle.Redraw();
					delete list[i];
					list[i] = nullptr;
				}
	return;
}

//Death Handling
void Death(int &lifeCnt, Vector2 lifePos[3], Ball &ball, Paddle &paddle, PowerUp **list)
{
	lifeCnt--;
	clearBall(lifePos[lifeCnt], 3);
	ball.reset();
	paddle.reset();
	resetPowerUp(list);

	while (lifeCnt >= 0)
		if (GetAsyncKeyState(VK_SPACE))
			break;
	return;
}

//Score Functions
void Score(int score, Vector2 scorePosition[7])
{
	static int digits[7] = { 0 };
	int digit;
	for (int i = 0; i < 7; i++)
	{
		digit = score % 10;
		if (digits[6 - i] != digit)
		{
			digits[6 - i] = digit;
			drawDigit(scorePosition[6 - i], digit);
		}
			score /= 10;
	}
}
