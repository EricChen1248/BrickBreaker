#include <Windows.h>

void draw(int x_input, int y_input, int x_length, int y_length);
void draw_dia(int x_input, int y_input, int x_length, int y_length, int hor_length);
int main()
{
	const int console_height = 32;
	const int console_pixelHeight = 18;
	const int console_width = 48;
	const int console_pixelWidth = 8;
	const int leftBorder = 12;
	const int rightBorder = 372;
	const int topBorder = 40;
	const int bottomBorder = 536;

	HDC device = GetDC(GetConsoleWindow());
	COLORREF color = RGB(0, 0, 0);
	COLORREF consoleColor = RGB(255, 255, 255);
	COLORREF blue = RGB(60, 100, 200);
	COLORREF red = RGB(255, 0, 0);
	COLORREF green = RGB(20, 200, 80);
	COLORREF purple = RGB(150, 50, 150);
	COLORREF yellow = RGB(150, 150, 20);
	COLORREF gray = RGB(150, 150, 150);

	for (int i = 0; i < console_width * console_pixelWidth; i++)
		for (int j = 0; j < console_height* console_pixelHeight; j++)
			SetPixel(device, i, j, consoleColor);

	for (int i = leftBorder; i < rightBorder; i++)
	{
		SetPixel(device, i, topBorder, color);
		SetPixel(device, i, bottomBorder, color);
	}
	for (int i = topBorder; i < bottomBorder; i++)
	{
		SetPixel(device, leftBorder, i, color);
		SetPixel(device, rightBorder, i, color);
	}

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

	while (true)
		if (GetAsyncKeyState(VK_SPACE))
			break;

	return 0;
}


void draw(int x_input, int y_input, int x_length, int y_length)
{
	HDC device = GetDC(GetConsoleWindow());
	COLORREF color = RGB(0, 0, 0);
	for (int x = x_input + 12; x < x_input + x_length + 12; x++)
	{
		for (int y = y_input + 40; y < y_input + y_length + 40; y++)
		{
			SetPixel(device, x, y, color);
		}
	}
}

void draw_dia(int x_input, int y_input, int x_length, int y_length, int hor_length)
{
	HDC device = GetDC(GetConsoleWindow());
	COLORREF color = RGB(0, 0, 0);
	int i = 0;
	for (int y = y_input + 40; y < y_input + y_length + 40; y++)
	{
		i = x_input + 12 + (y - y_input - 40) * x_length / y_length;
		for (int x = i; x < i + hor_length; x++)
		{
			SetPixel(device, x, y, color);
		}
	}
}
