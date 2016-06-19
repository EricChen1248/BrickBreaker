#pragma once
#include <iostream>
#include <Windows.h>

struct Vector2D
{
	double x;
	double y;
	Vector2D operator+(Vector2D v) { x += v.x; y += v.y; return{ x,y }; }
	Vector2D operator=(Vector2D v) { x = v.x; y = v.y; return{ v.x, v.y }; }
};

struct Vector2
{
	int x;
	int y;
	Vector2 operator+(Vector2 v) { x += static_cast<int>(v.x); y += static_cast<int>(v.y); return{ x,y }; }
	Vector2 operator+(Vector2D v) { x += static_cast<int>(v.x); y += static_cast<int>(v.y); return{ x,y }; }
	Vector2 operator=(Vector2 v) { x = static_cast<int>(v.x); y = static_cast<int>(v.y); return{ x, y }; }
	Vector2 operator=(Vector2D v) { x = static_cast<int>(v.x); y = static_cast<int>(v.y); return{ x, y }; }

}; 

#define PI 3.14159265358979323

namespace Draw
{
	const int console_height = 32;
	const int console_pixelHeight = 18;
	const int console_width = 48;
	const int console_pixelWidth = 8;
	const int leftBorder = 12;
	const int rightBorder = 372;
	const int topBorder = 40;
	const int bottomBorder = 536;

	double lengthdir_x(int len, int dir) { return cos(dir * PI / 180) * len; }
	double lengthdir_y(int len, int dir) { return sin(dir * PI / 180) * len; }

	HDC device = GetDC(GetConsoleWindow());
	COLORREF color = RGB(0, 0, 0);
	COLORREF consoleColor = RGB(255, 255, 255);
	COLORREF blue = RGB(60, 100, 200);
	COLORREF red = RGB(255, 0, 0);
	COLORREF green = RGB(20, 200, 80);
	COLORREF purple = RGB(150, 50, 150);
	COLORREF yellow = RGB(150, 150, 20);
	COLORREF gray = RGB(150, 150, 150);

	void drawBall(Vector2 position, int radius, COLORREF ballcolor = color)
	{
		//for (unsigned int j = 0; j < radius; j++)
		for (unsigned int i = 0; i < 360; i++)
			SetPixelV(device, position.x + static_cast<int>(lengthdir_x(radius, i)), position.y + static_cast<int>(lengthdir_y(radius, i)), ballcolor);
		return;
	}
	void clearBall(Vector2 position, int radius)
	{
		//for (int j = 0; j < radius; j++)
		for (int i = 0; i < 360; i++)
			SetPixelV(device, position.x + static_cast<int>(lengthdir_x(radius, i)), position.y + static_cast<int>(lengthdir_y(radius, i)), consoleColor);
		return;
	}

	void drawBrick(Vector2 position, Vector2 size, COLORREF brickColor = color)
	{
		for (int i = 0; i < size.x; i++)
			for (int j = 0; j < size.y; j++)
				SetPixelV(device, position.x + i, position.y + j, brickColor);
		return;
	}
	void clearBrick(Vector2 position, Vector2 size)
	{
		for (int i = 0; i < size.x; i++)
			for (int j = 0; j < size.y; j++)
				SetPixelV(device, position.x + i, position.y + j, consoleColor);
	}

	void digitLine(Vector2 position, int location, COLORREF color)
	{
		switch (location)
		{
		case 1:
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 2; j++)
					SetPixelV(device, position.x + j, position.y + i, color);
			break;
		case 2:
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 2; j++)
					SetPixelV(device, position.x + j, position.y + i + 8, color);
			break;
		case 3:
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 2; j++)
					SetPixelV(device, position.x + i, position.y + j, color);
			break;
		case 4:
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 2; j++)
					SetPixelV(device, position.x + i, position.y + 8 + j, color);
			break;
		case 5:
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 2; j++)
					SetPixelV(device, position.x + i, position.y + 16 + j, color);
			break;
		case 6:
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 2; j++)
					SetPixelV(device, position.x + 8 + j, position.y + i, color);
			break;
		case 7:
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 2; j++)
					SetPixelV(device, position.x + 8 + j, position.y + i + 8, color);
			break;
		case 8:
			for (int i = 0; i < 9; i++)
				for (int j = 0; j < 2; j++)
					for (int k = 0; k < 2; k++)
						SetPixelV(device, position.x + i + j, position.y + 8 + i + k, color);
			break;
		default:
			break;
		}
	}
	void drawDigit(Vector2 position, int digit, COLORREF  digitColor = color)
	{
		for (int i = 1; i < 8; i++)
			digitLine(position, i, consoleColor);
		Vector2 positionTemp = position;
		positionTemp.x += 4;
		digitLine(positionTemp, 1, consoleColor);
		digitLine(positionTemp, 2, consoleColor);
		switch (digit)
		{
		case 0:
			digitLine(position, 1, digitColor);
			digitLine(position, 2, digitColor);
			digitLine(position, 3, digitColor);
			digitLine(position, 5, digitColor);
			digitLine(position, 6, digitColor);
			digitLine(position, 7, digitColor);
			break;
		case 1:
		{
			Vector2 positionTemp = position;
			positionTemp.x += 4;
			digitLine(positionTemp, 1, digitColor);
			digitLine(positionTemp, 2, digitColor);
			break;
		}
		case 2:
			digitLine(position, 3, digitColor);
			digitLine(position, 6, digitColor);
			digitLine(position, 4, digitColor);
			digitLine(position, 2, digitColor);
			digitLine(position, 5, digitColor);
			break;
		case 3:
			digitLine(position, 3, digitColor);
			digitLine(position, 4, digitColor);
			digitLine(position, 5, digitColor);
			digitLine(position, 6, digitColor);
			digitLine(position, 7, digitColor);
			break;
		case 4:
			digitLine(position, 1, digitColor);
			digitLine(position, 4, digitColor);
			digitLine(position, 6, digitColor);
			digitLine(position, 7, digitColor);
			break;
		case 5:
			digitLine(position, 3, digitColor);
			digitLine(position, 1, digitColor);
			digitLine(position, 4, digitColor);
			digitLine(position, 7, digitColor);
			digitLine(position, 5, digitColor);
			break;
		case 6:
			digitLine(position, 1, digitColor);
			digitLine(position, 2, digitColor);
			digitLine(position, 3, digitColor);
			digitLine(position, 4, digitColor);
			digitLine(position, 5, digitColor);
			digitLine(position, 7, digitColor);
			break;
		case 7:
			digitLine(position, 3, digitColor);
			digitLine(position, 6, digitColor);
			digitLine(position, 7, digitColor);
			break;
		case 8:
			digitLine(position, 1, digitColor);
			digitLine(position, 2, digitColor);
			digitLine(position, 3, digitColor);
			digitLine(position, 4, digitColor);
			digitLine(position, 5, digitColor);
			digitLine(position, 6, digitColor);
			digitLine(position, 7, digitColor);
			break;
		case 9:
			digitLine(position, 1, digitColor);
			digitLine(position, 3, digitColor);
			digitLine(position, 4, digitColor);
			digitLine(position, 6, digitColor);
			digitLine(position, 7, digitColor);
			break;
		default:
			break;
		}
	}
	void drawLetter(Vector2 position, char Char, COLORREF letterColor = color)
	{
		switch (Char)
		{
		case 'S':
			drawDigit(position, 5, letterColor);
			break;
		case 'C':
			digitLine(position, 3, letterColor);
			digitLine(position, 1, letterColor);
			digitLine(position, 2, letterColor);
			digitLine(position, 5, letterColor);
			break;
		case 'O':
			drawDigit(position, 0, letterColor);
			break;
		case 'R':
			digitLine(position, 3, letterColor);
			digitLine(position, 1, letterColor);
			digitLine(position, 6, letterColor);
			digitLine(position, 4, letterColor);
			digitLine(position, 2, letterColor);
			digitLine(position, 8, letterColor);
			break;
		case 'E':
			drawLetter(position, 'F', letterColor);
			digitLine(position, 5, letterColor);
			break;
		case 'L':
			digitLine(position, 1, letterColor);
			digitLine(position, 2, letterColor);
			digitLine(position, 5, letterColor);
			break;
		case 'I':
			drawDigit(position, 1, letterColor);
			break;
		case 'F':
			digitLine(position, 1, letterColor);
			digitLine(position, 2, letterColor);
			digitLine(position, 3, letterColor);
			digitLine(position, 4, letterColor);
			break;
		case ':':
		{
			Vector2 positiont = position;
			positiont.x += 4;
			positiont.y += 4;
			drawBall(positiont, 1, letterColor);
			positiont.y += 8;
			drawBall(positiont, 1, letterColor);
			break;
		}
		default:
			break;
		}
	}
	
	void drawBorder()
	{
		for (int i = leftBorder; i < rightBorder; i++)
		{
			SetPixelV(device, i, topBorder, color);
			SetPixelV(device, i, bottomBorder, color);
		}
		for (int i = topBorder; i < bottomBorder; i++)
		{
			SetPixelV(device, leftBorder, i, color);
			SetPixelV(device, rightBorder, i, color);
		}
		return;
	}
	void setConsole()
	{
		Vector2 sLetterPosition[6];
		Vector2 lLetterPosition[6];

		for (int j = console_height* console_pixelHeight; j>= 0; j--)
			for (int i = 0; i < console_width * console_pixelWidth; i++)
				SetPixelV(device, i, j, consoleColor);
		drawBorder();

		for (int i = 0; i < 6; i++)
		{
			sLetterPosition[i].y = topBorder - 25;
			sLetterPosition[i].x = leftBorder + i * 15 + 15;
		}

		for (int i = 0; i < 6; i++)
		{
			lLetterPosition[i].y = topBorder - 25;
			lLetterPosition[i].x = leftBorder + i * 15 + 235;
		}

		drawLetter(sLetterPosition[0], 'S', red);
		drawLetter(sLetterPosition[1], 'C', green);
		drawLetter(sLetterPosition[2], 'O', blue);
		drawLetter(sLetterPosition[3], 'R', purple);
		drawLetter(sLetterPosition[4], 'E', yellow);
		drawLetter(sLetterPosition[5], ':');
		drawLetter(lLetterPosition[0], 'L', red);
		drawLetter(lLetterPosition[1], 'I', green);
		drawLetter(lLetterPosition[2], 'F', blue);
		drawLetter(lLetterPosition[3], 'E', purple);
		drawLetter(lLetterPosition[4], ':');

		return;
	}

	void drawHeart(COLORREF color = red)
	{
		for (double i = 0; i < 720; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					SetPixelV(device, static_cast<int>(190 + 64 * pow(sin(i*PI / 360), 3) + j),
						static_cast<int>(250 + 55 * cos(i * PI / 360) + 20 * cos(i * PI / 180) - 8 * cos(i * PI / 120) - 4 * cos(i * PI / 90) + k), color);
	}

	void draw(int x_input, int y_input, int x_length, int y_length)
	{
		for (int x = x_input + 12; x < x_input + x_length + 12; x++)
			for (int y = y_input + 40; y < y_input + y_length + 40; y++)
				SetPixel(device, x, y, color);
	}
	void draw_dia(int x_input, int y_input, int x_length, int y_length, int hor_length)
	{
		int i = 0;
		for (int y = y_input + 40; y < y_input + y_length + 40; y++)
		{
			i = x_input + 12 + (y - y_input - 40) * x_length / y_length;
			for (int x = i; x < i + hor_length; x++)
				SetPixel(device, x, y, color);
		}
	}

	void endGame()
	{
		//G
		draw(25, 75, 30, 6);
		draw(25, 75, 6, 45);
		draw(25, 114, 30, 6);
		draw(55, 97, 6, 23);
		draw(40, 97, 15, 6);
		//A
		draw(65, 75, 30, 6);
		draw(65, 95, 30, 6);
		draw(65, 75, 6, 45);
		draw(95, 75, 6, 45);
		//M
		draw(105, 75, 6, 45);
		draw(135, 75, 6, 45);
		draw_dia(105, 75, 15, 45, 6);
		draw_dia(135, 75, -15, 45, 6);
		//E
		draw(145, 75, 30, 6);
		draw(145, 75, 6, 45);
		draw(145, 97, 30, 6);
		draw(145, 114, 30, 6);
		//O
		draw(185, 75, 30, 6);
		draw(185, 75, 6, 45);
		draw(215, 75, 6, 45);
		draw(185, 114, 30, 6);
		//V
		draw_dia(225, 75, 15, 45, 6);
		draw_dia(255, 75, -15, 45, 6);
		//E
		draw(265, 75, 30, 6);
		draw(265, 75, 6, 45);
		draw(265, 97, 30, 6);
		draw(265, 114, 30, 6);
		//R
		draw(305, 75, 30, 6);
		draw(305, 75, 6, 45);
		draw(335, 75, 6, 22);
		draw(305, 97, 30, 6);
		draw_dia(320, 97, 15, 23, 6);
		//R
		draw(50, 170, 20, 4);
		draw(50, 170, 4, 30);
		draw(70, 170, 4, 15);
		draw(50, 185, 20, 4);
		draw_dia(60, 185, 10, 15, 4);
		//E
		draw(80, 170, 20, 4);
		draw(80, 170, 4, 30);
		draw(80, 185, 20, 4);
		draw(80, 196, 20, 4);
		//S
		draw(110, 170, 20, 4);
		draw(110, 170, 4, 15);
		draw(110, 185, 20, 4);
		draw(130, 185, 4, 15);
		draw(110, 196, 20, 4);
		//E
		draw(140, 170, 20, 4);
		draw(140, 170, 4, 30);
		draw(140, 185, 20, 4);
		draw(140, 196, 20, 4);
		//T
		draw(170, 170, 20, 4);
		draw(180, 170, 4, 30);
		//?
		draw(200, 170, 20, 4);
		draw(200, 170, 4, 5);
		draw(220, 170, 4, 10);
		draw(210, 180, 10, 4);
		draw(210, 180, 4, 10);
		draw(210, 195, 4, 5);
		//Y
		draw(240, 185, 4, 15);
		draw_dia(230, 170, 10, 15, 4);
		draw_dia(250, 170, -10, 15, 4);
		//"/"
		draw_dia(280, 170, -20, 30, 4);
		//N
		draw(290, 170, 4, 30);
		draw(310, 170, 4, 30);
		draw_dia(290, 170, 20, 30, 4);
	}
}