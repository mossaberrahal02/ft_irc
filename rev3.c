#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int get_cell(char *b, int w, int h, int j, int i)
{
	if (i < 0 || i >= h || j < 0 || j >= w) return 0;
	return b[i * w + j];
}

int get_nbrs(char *b, int w, int h, int j, int i)
{
	int n = 0;
	for (int di = -1; di <= 1; di ++)
	{
		for (int dj = -1; dj <= 1; dj ++)
		{
			if (di || dj)
				n += get_cell(b, w, h, j + dj, i + di);
		}
	}
	return n;
}

void update(char* b, char* next, int w, int h)
{
	for (int i = 0; i < h; i ++)
	{
		for (int j = 0; j < w; j ++)
		{
			int n = get_nbrs(b, w, h, j, i);
			int c = get_cell(b, w, h, j, i);
			next[i * w + j] = (c && (n == 2 || n ==3)) || (c == 0 && n == 3);
		}
	}
}


void draw(char *b, int w, int h)
{
	for (int i = 0; i < h; i ++)
	{
		for (int j = 0; j < w; j ++)
		{
			putchar(b[i * w + j] ? 'O':' ');
		}
		putchar('\n');
	}
}

int main(int ac, char **av)
{
	if (ac != 4) return 1;
	int w = atoi(av[1]), h = atoi(av[2]), it = atoi(av[3]);
	char *board = calloc(h * w, 1);
	char *next = calloc(h * w, 1);
	int i = 0, j = 0, pen =0;
	char c;
	while(read(0, &c, 1) > 0)
	{
		if (c == 'x') pen = !pen;
		else if (c == 'w' && i > 0) i --;
		else if (c == 's' && i + 1 < h) i ++;
		else if (c == 'a' && j > 0) j --;
		else if (c == 'd' && j + 1 < w) j++;
		if (pen)
			board[i * w + j] = 1;
	}
	while (it --)
	{
		update(board, next, w, h);
		char *tmp = board; board = next; next = tmp; 
	}
	draw(board, w, h);
	return 0;
}