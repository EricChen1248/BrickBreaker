#pragma once
#include "Drawing.h"

using namespace Draw;

namespace Objects
{
	class PowerUp
	{
	public:
		PowerUp(Vector2 Location) { location.x = Location.x + 27; location.y = Location.y + 11; randomType(); }
		void setPointer(PowerUp* ptr) { powerupPtr = ptr; }
		void randomType() { type = rand() % 6; determineColor(); }
		void determineColor();
		void drop(PowerUp *list[]);
		void reset() { clearBall(location, radius); }
		Vector2 getLocation() { return location; }
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
		Ball() { shift = 2;  speed.x = 0; speed.y = -8; radius = 6; position.y = 493; position.x = (console_width * console_pixelWidth - radius) / 2; drawBall(position, radius); hitStrength = 1; }
		void move(int x = 0, int y = 0);
		Vector2 getPosition() { return position; }
		Vector2 getSpeed() { Vector2 Speed; Speed.x = static_cast<int>(speed.x);  Speed.y = static_cast<int>(speed.y); return Speed; }
		int getRadius() { return radius; }
		int getHitStrength() { return hitStrength; }
		void verticalRebound() { speed.y = -speed.y; }
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
		position.y = 493;
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
		void hit(Ball ball, int &score, PowerUp **list);
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
	void Brick::hit(Ball ball, int &score, PowerUp **list)
	{
		hitPoints -= ball.getHitStrength();
		if (hitPoints <= 0)
		{
			hitPoints = 0;
			score += 2;
			brickCnt--;
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