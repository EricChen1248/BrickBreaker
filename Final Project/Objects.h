#pragma once
#include "Drawing.h"

using namespace Draw;

namespace Objects
{
	class Missile;
	class Brick;

	class Paddle
	{
	public:
		Paddle() { size.x = 70; size.y = 5; position.x = (console_width * console_pixelWidth - size.x) / 2; position.y = 500;  drawBrick(position, size); oldPosition = position; }

		Vector2 getPosition() { return position; }
		Vector2 getSize() { return size; }
		void Redraw() { drawBrick(position, size); }
		void Grow() { size.x = 120; position.x = (position.x + size.x >= rightBorder) ? rightBorder - size.x - 1: position.x ; Redraw(); }
		void move(int left);
		void newMissile();
		void killMissile() { delete missile; missile = nullptr; }
		void moveMissile(Brick list[5][5]);
		void reset();
	private:
		Vector2 size;
		Vector2 position;
		Vector2 oldPosition;
		Missile *missile;
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
		Ball() { shift = 2;  speed.x = 0; speed.y = -10; radius = 6; position.y = 493; position.x = (console_width * console_pixelWidth - radius) / 2; drawBall(position, radius); hitStrength = 1; }
		void move(int x = 0, int y = 0);
		Vector2 getPosition() { return position; }
		Vector2 getSpeed() { Vector2 Speed; Speed.x = static_cast<int>(speed.x);  Speed.y = static_cast<int>(speed.y); return Speed; }
		int getRadius() { return radius; }
		int getHitStrength() { return hitStrength; }
		void godMode() { speed.y *= 2; speed.x *= 2; }
		void endGod() { speed.y /= 4; speed.x /= 4; }
		void verticalRebound() { speed.y = -speed.y; }
		void horizontalRebound() { speed.x = -speed.x; move(); }
		void shiftLeft() { speed.x -= shift; }
		void shiftRight() { speed.x += shift; }
		void get_speeddown() { speed.x = speed.x * 0.75; speed.y = speed.y * 0.75; shift = shift * 0.75; }
		void get_bigball() { clearBall(position, radius);  radius = static_cast<int>(radius * 1.5); }
		void addStrength() { hitStrength++; }
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
		position.y = 493;
		speed = { 0,-8 };
		radius = 6;
		hitStrength = 1;
		drawBall(position, radius);
		shift = 2;
	}

	void addLife(int &lifeCnt, Vector2 lifePos[3], Ball &ball, Paddle &paddle);
	class PowerUp
	{
	public:
		PowerUp(Vector2 Location) { location.x = Location.x + 27; location.y = Location.y + 11; randomType(); }
		void setPointer(PowerUp* ptr) { powerupPtr = ptr; }
		void randomType() { type = rand() % 6; determineColor(); }
		void drop(PowerUp *list[]);
		void reset() { clearBall(location, radius); }
		Vector2 getLocation() { return location; }
		void determineColor();
		void hit(int &lifeCnt, Vector2 lifePos[], Ball &ball, Paddle &paddle);
	private:
		const int radius = 3;
		Vector2 location;
		int type;
		COLORREF color;
		PowerUp* powerupPtr;
	};
	void PowerUp::determineColor()
	{
		switch (type)
		{
		case 0:
			this->color = blue;
			break;
		case 1:
			this->color = yellow;
			break;
		case 2:
			this->color = red;
			break;
		case 3:
			this->color = purple;
			break;
		case 4:
			this->color = Draw::color;
			break;
		case 5:
			this->color = green;
			break;
		}
	}
	void PowerUp::drop(PowerUp *list[])
	{
		clearBall(location, radius);
		if (location.y + 2 >= bottomBorder)
		{
			for (int i = 0; i < 10; i++)
				if (list[i] == powerupPtr)
				{
					delete list[i];
					list[i] = nullptr;
				}
		}
		else
		{
			location.y += 2;
			drawBall(location, radius, this->color);
		}
	}
	void PowerUp::hit(int &lifeCnt, Vector2 lifePos[], Ball &ball, Paddle &paddle)
	{
		switch (type)
		{
		case 0:
			ball.get_speeddown();
			break;
		case 1:
			ball.get_bigball();
			break;
		case 2:
			ball.addStrength();
			break;
		case 3:
			paddle.Grow();
			break;
		case 4:
			addLife(lifeCnt, lifePos, ball, paddle);
			break;
		case 5:
			paddle.newMissile();
			break;
		default:
			break;
		}
	}
	
	class Brick
	{
	public:
		static Vector2 size;
		static int brickCnt;
		Brick() { size.x = 54; size.y = 16; hitPoints = 0; }
		void setHitPoints(int hits) { hitPoints = hits; }
		void setX(int x) { location.x = x; }
		void moveX(int x) { location.x += x; }
		void setY(int y) { location.y = y; }
		void draw();
		void hit(Ball ball, int &score, PowerUp **list);
		void hit();
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
			drawBrick(location, size, gray);
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
		case 6:
			drawBrick(location, size);
			break;
		default:
			break;
		}
	}
	void Brick::hit(Ball ball, int &score, PowerUp **list)
	{
		if (hitPoints < 6)
		{
			hitPoints -= ball.getHitStrength();
			if (hitPoints <= 0)
			{
				hitPoints = 0;
				score += 2;
				--brickCnt;
				ball.hit();
				int x = rand() % 5;
				if (x == 1)
				{
					PowerUp *p = new PowerUp(location);
					p->setPointer(p);
					for (int i = 0; i < 10; i++)
						if (list[i] == nullptr)
						{
							list[i] = p;
							break;
						}
				}
			}
			score += 1;
			draw();
		}
	}
	void Brick::hit()
	{
		if (hitPoints < 6)
		{
			if (--hitPoints == 0)
			{
				--brickCnt;
			}
			draw();
		}
	}


	class Missile
	{
	public:
		Missile(Vector2 pos) { position = pos; size.x = 1, size.y = 20; position.y += 5; }
		~Missile() { ; }
		void move(Brick list[5][5], Paddle& paddle) { clearBrick(position, size);  position.y -= 5; colDetect(list, paddle); }
	private:
		Vector2 position;
		Vector2 size;
		void colDetect(Brick list[5][5], Paddle &paddle) {
			for (int i = 5 - 1; i >= 0; i--)
				for (int j = 0; j < 5; j++)
					if (list[i][j].getPosition().y + Brick::size.y >= position.y
						&& list[i][j].getPosition().x <= position.x
						&& list[i][j].getPosition().x + Brick::size.x >= position.x
						&& list[i][j].getHitPoints() > 0)
					{
						list[i][j].hit();
						paddle.killMissile();
						return;
					}
					else if (position.y <= topBorder)
					{
						paddle.killMissile();
						return;
					}
			drawBrick(position, size);
			return;
		}
	};

	void Paddle::moveMissile(Brick list[5][5])
	{
		if (missile != nullptr) 
			missile->move(list, *this);
		return;
	}
	void Paddle::newMissile()
	{
		missile = new Missile({ position.x + size.x / 2, position.y - 21 });
		return;
	}

	void addLife(int &lifeCnt, Vector2 lifePos[3], Ball &ball, Paddle &paddle)
	{
		if (lifeCnt < 3)
		{
			lifeCnt++;
			drawBall(lifePos[lifeCnt], 3);
		}
		return;
	}
}