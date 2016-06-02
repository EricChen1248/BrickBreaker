#include <iostream>
#include <Windows.h>
#include <cmath>
#include "Drawing.h"

#using <mscorlib.dll>
using namespace std;
using namespace System;
using namespace Draw;


class Paddle
{
public:
	Paddle() { size.x = 70; size.y = 5; position.x = (console_width * console_pixelWidth - size.x) / 2; position.y = 500;  drawBrick(position, size); oldPosition = position; }

	Vector2 getPosition() { return position; }
	Vector2 getSize() { return size; }
	void Redraw() { drawBrick(position, size); }
	void move(int left);
	void reset();
private:
	Vector2 size;
	Vector2 position;
	Vector2 oldPosition;
};
void Paddle::move(int right)
{
	Vector2 pos = position;
	pos.x += size.x - 7;
	Vector2 Size = { 7,5 };
	if (right > 0)
		clearBrick(position, Size);
	else
	{
		clearBrick(pos, Size);
	}

	position.x += right;
	if (position.x > rightBorder - size.x)
		position.x = rightBorder - size.x;
	if (position.x < leftBorder + 1)
		position.x = leftBorder + 1;

	pos = position;
	pos.x += size.x - 7;
	drawBrick(position, Size);
	drawBrick(pos, Size);

	return;
}
void Paddle::reset()
{
	clearBrick(position, size);
	size.x = 70;
	size.y = 5;
	position.x = (console_width * console_pixelWidth - size.x) / 2;
	position.y = 500; 
	drawBrick(position, size); 
}

class Ball
{
public:
	Ball() { shift = 2;  speed.x = 0; speed.y = -8; radius = 6; position.y = 494; position.x = (console_width * console_pixelWidth - radius) / 2; drawBall(position, radius); hitStrength = 1; }
	void move(int x = 0, int y= 0);
	Vector2 getPosition() { return position; }
	Vector2 getSpeed() { Vector2 Speed; Speed.x = static_cast<int>(speed.x);  Speed.y = static_cast<int>(speed.y); return Speed; }
	int getRadius() { return radius; }
	int getHitStrength() { return hitStrength; }
	void verticalRebound() { speed.y = -speed.y;  }
	void horizontalRebound() { speed.x = -speed.x; }
	void shiftLeft() { speed.x -= shift; }
	void shiftRight() { speed.x += shift; }
	void get_speeddown() { speed.x = speed.x * 0.75; speed.y = speed.y * 0.75; shift = shift * 0.75; }
	void speeddown_end() { speed.x = speed.x / 0.75; speed.y = speed.y / 0.75; shift = shift / 0.75; }
	void get_bigball() { radius = 12; }
	void bigball_end() { radius = 6; }
	void hit() { speed.x = speed.x * 1.05; speed.y = speed.y * 1.05; shift = shift * 1.05; }
	void reset();
private:
	int radius;
	int hitStrength;
	Vector2 position;
	Vector2D speed;
	double shift;
};
void Ball::move(int x, int y)
{
	clearBall(position, radius);
	if (x != 0 && y != 0)
	{
		position.x = x;
		position.y = y;
	}
	else
		position = position + speed;

	drawBall(position, radius);
	return;
}
void Ball::reset()
{
	clearBall(position, radius);
	position.x = console_width * console_pixelWidth / 2;
	position.y = 494;
	speed = { 0,-8 };
	radius = 6;
	hitStrength = 1;
	drawBall(position, radius);
	shift = 2;
}

class Brick
{
public:
	static Vector2 size;
	static int brickCnt;
	Brick() { size.x = 54; size.y = 16; }
	void setHitPoints(int hits) { hitPoints = hits; }
	void setX(int x) { location.x = x; }
	void setY(int y) { location.y = y; }
	void draw();
	void hit(Ball ball, int &score);
	int getHitPoints() { return hitPoints; }
	Vector2 getPosition() { return location; }

private:
	Vector2 location;
	int hitPoints;
};
Vector2 Brick::size;
int Brick::brickCnt;
void Brick::draw()
{
	switch (hitPoints)
	{
	case 0:
		clearBrick(location, size);
		break;
	case 1:
		drawBrick(location, size);
		break;
	case 2:
		drawBrick(location, size, blue);
		break;
	case 3:
		drawBrick(location, size, red);
		break;
	case 4:
		drawBrick(location, size, green);
		break;
	case 5:
		drawBrick(location, size, purple);
		break;
	default:
		break;
	}
}
void Brick::hit(Ball ball, int &score)
{
	hitPoints -= ball.getHitStrength();
	if (hitPoints <= 0)
	{
		hitPoints = 0;
		score += 20;
		brickCnt--;
		ball.hit();
	}
	score += 10;
	draw();
}

void CollisionDetectionPaddle(Paddle paddle, Ball& ball, bool &redrawPaddle);
void BorderCollision(Ball &ball, bool godMode, bool &death);
void LoadBricks(Brick Brick[5][5]);
void BrickInitialize(Brick brick[5][5]);
void BrickCollision(Ball &ball, Brick brick[5][5], int &score);
void Death(int &lifeCnt, Vector2 lifePos[3], Ball &ball, Paddle &paddle);
void Score(int score, Vector2 scorePosition[6]);
void Game(int level, Brick brick[5][5]);

int main(int argc, const char * argv[])
{
	HDC device = GetDC(GetConsoleWindow());
	Console::SetWindowSize(console_width, console_height);
	setConsole();

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
	drawLetter(sLetterPosition[0], 'S');
	drawLetter(sLetterPosition[1], 'C');
	drawLetter(sLetterPosition[2], 'O');
	drawLetter(sLetterPosition[3], 'R');
	drawLetter(sLetterPosition[4], 'E');
	drawLetter(sLetterPosition[5], ':');
	drawLetter(lLetterPosition[0], 'L');
	drawLetter(lLetterPosition[1], 'I');
	drawLetter(lLetterPosition[2], 'F');
	drawLetter(lLetterPosition[3], 'E');
	drawLetter(lLetterPosition[4], ':');
	}

	BrickInitialize(brick);
	while (lifeCount >= 0)
	{
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
			Sleep(20);		//Game refresh interval
			ball.move();

			if (redrawPaddle)
			{
				paddle.Redraw();
				redrawPaddle = false;
			}

			CollisionDetectionPaddle(paddle, ball, redrawPaddle);
			BorderCollision(ball, godMode, death);
			BrickCollision(ball, brick, score);
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
		Bricks[2][i].setHitPoints(3);
		Bricks[4][i].setHitPoints(5);
	}
	for (int i = 0; i < 4; i++)
	{
		Bricks[1][i].setHitPoints(2);
		Bricks[3][i].setHitPoints(4);
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
void BrickCollision(Ball &ball, Brick brick[5][5], int &score)
{
	bool collision = false;
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			//Vertical Collision
			if ((ball.getPosition().x + ball.getRadius()>= brick[i][j].getPosition().x || ball.getPosition().x + ball.getSpeed().x + ball.getRadius() >= brick[i][j].getPosition().x)
				&& (ball.getPosition().x - ball.getRadius() <= brick[i][j].getPosition().x + Brick::size.x || ball.getPosition().x + ball.getSpeed().x - ball.getRadius() <= brick[i][j].getPosition().x + Brick::size.x))
			{
				//Collision from top / bottom
				if (ball.getPosition().y + ball.getRadius() + 1 <= brick[i][j].getPosition().y && ball.getPosition().y + ball.getSpeed().y + ball.getRadius() + 1 > brick[i][j].getPosition().y 
					|| ball.getPosition().y - ball.getRadius() + 1 >= brick[i][j].getPosition().y + Brick::size.y && ball.getPosition().y + ball.getSpeed().y - ball.getRadius() + 1 < brick[i][j].getPosition().y + Brick::size.y)
					if (brick[i][j].getHitPoints() > 0)
					{
						brick[i][j].hit(ball, score);
						ball.verticalRebound();
						collision = true;
					}
			}
			//Horizontal Collision
			else if ((ball.getPosition().y + ball.getRadius() >= brick[i][j].getPosition().y || ball.getPosition().y + ball.getSpeed().y + ball.getRadius() >= brick[i][j].getPosition().y)
				&& (ball.getPosition().y  - ball.getRadius() <= brick[i][j].getPosition().y + Brick::size.y || ball.getPosition().y + ball.getSpeed().y - ball.getRadius() <= brick[i][j].getPosition().y + Brick::size.y))
			{
				//Collision from left / right
				if (ball.getPosition().x + ball.getRadius() + 1 <= brick[i][j].getPosition().x && ball.getPosition().x + ball.getSpeed().x + ball.getRadius() + 1 > brick[i][j].getPosition().x
					|| ball.getPosition().x - ball.getRadius() + 1 >= brick[i][j].getPosition().x + Brick::size.x && ball.getPosition().x + ball.getSpeed().x - ball.getRadius() + 1< brick[i][j].getPosition().x + Brick::size.x)
				{
					if (brick[i][j].getHitPoints() > 0)
					{
						brick[i][j].hit(ball, score);
						ball.horizontalRebound();
						collision = true;
					}
				}
			}

			if (collision)
				BrickCollision(ball, brick, score);
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