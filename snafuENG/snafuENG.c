#include <snafuENG.h>
#include <snafuENG-CFG.c>
#include <snafuENG-LOG.c>
#include <snafuENG-SYS.c>

char** rascii(const char *path)
{
	checkeng(__func__);
	FILE *f = 0;
	int c = 0, i, j, x = cfg.display.resolution.width - 2, y = cfg.display.resolution.height - 2;
	char **ascii;
	
	ascii = (char**)malloc(sizeof(char*) * y);
	if (! ascii)
	{
		werror(ERROR_MEMORY, 0, __func__);
		return 0;
	}
	for (i = 0; i < y; i++)
	{
		ascii[i] = malloc(sizeof(char) * x);
		if (! ascii[i])
		{
			werror(ERROR_MEMORY, 0, __func__);
			free(ascii);
			return 0;
		}
	}
	if (path)
	{
		f = fopen(path, "r");
		if (! f)
			werror(ERROR_OPEN, path, __func__);
	}
	for (i = 0; i < y; i++)
	{
		for (j = 0; j < x; j++)
		{
			if (f)
			{
				c = fgetc(f);
				if (c >= ' ' && c < '~')
					ascii[i][j] = c;
				else
				{
					ascii[i][j] = ' ';
					if (c == '\n')
					{
						for (j++; j < x; j++)
							ascii[i][j] = ' ';
						c = 0;
					}
					else if (c == EOF)
					{
						if (fclose(f))
							werror(ERROR_CLOSE, path, __func__);
						f = 0;
					}
					else
						c = ' ';
				}
			}
			else
				ascii[i][j] = ' ';
		}
		if (c && f)
			fgetc(f);
	}
	if (f)
		if (fclose(f))
			werror(ERROR_CLOSE, path, __func__);
	return ascii;
}

void prints(const char *s)
{
	checkeng(__func__);
	int resetcur;

	if (! cfg.system.cursor)
	{
		setcur(1);
		resetcur = 1;
	}
	else resetcur = 0;
	for (int c = 0; *s; s++)
	{
		putchar(*s);
		if (c != '\n')
		{
			nanosleep(&cfg.text.speed, 0);
			if (kbhit())
				c = getchar();
		}
	}
	if (resetcur)
		setcur(0);
}

int printmainmenu()
{
	checkeng(__func__);
	int c = 0, selection = 0;

	while (! selection || c != '\n')
	{
		clear();
		printf(COLOR_CFG "\n    ┌───────────┐\n    │ ", cfg.hud.color);
		printf(COLOR_BOLDWHITE "MAIN MENU");
		printf(COLOR_CFG " │\n    └───────────┘\n\n" COLOR_BOLDWHITE, cfg.hud.color);
		if (selection == 1)
			putchar('>');
		printf(" New Game\n");
		if (selection == 2)
			putchar('>');
		printf(" Load Game\n");
		if (selection == 3)
			putchar('>');
		printf(" Quit\n" COLOR_RESET);
		while (! kbhit())
			fsleep(0.07);
		c = getchar();
		if (c == 27)
		{
			c = getchar();
			if (c == 91)
			{
				c = getchar();
				if (c == 65)
				{
					selection--;
					if (selection < 1)
						selection = cfg.menu.buttons;
				}
				else if (c == 66)
				{
					selection++;
					if (selection > cfg.menu.buttons)
						selection = 1;
				}
			}
		}
	}
	return selection;
}

void printhud(char **image)
{
	checkeng(__func__);
	if (cfg.display.resolution.width < 2 || cfg.display.resolution.height < 2)
		return;
	int i, j, x = cfg.display.resolution.width, y = cfg.display.resolution.height / 2;
	char *tlc, *trc, *llc, *lrc, *hl, *vl;
	
	if (cfg.hud.theme == 1)
	{
		tlc="┌"; trc="┐"; llc="└"; lrc="┘"; hl="─"; vl="│";
	}
	else if (cfg.hud.theme == 2)
	{
		tlc="╔"; trc="╗"; llc="╚"; lrc="╝"; hl="═"; vl="║";
	}
	else
	{
		tlc=" "; trc=" "; llc=" "; lrc=" "; hl=" "; vl=" ";
	}
	
	printf(COLOR_CFG "%s", cfg.hud.color, tlc);
	for (i = 0; i < x - 2; i++)
		printf("%s", hl);
	printf("%s\n" COLOR_RESET, trc);
	for (i = 0; i < y - 2; i++)
	{
		for (j = 0; j < x + 1; j++)
		{
			if (j && j < x - 1)
			{
				if (image)
					printf("%c", image[i][j - 1]);
				else
					putchar(' ');
			}
			else if (j != x)
				printf(COLOR_CFG "%s" COLOR_RESET, cfg.hud.color, vl);
			else
				putchar('\n');
		}
	}
	printf(COLOR_CFG "%s", cfg.hud.color, llc);
	for (i = 0; i < x - 2; i++)
		printf("%s", hl);
	printf("%s\n" COLOR_RESET, lrc);
}

void printsnafufx()
{
	if (cfg.display.resolution.width - 9 < 0 || cfg.display.resolution.height / 2 - 5 < 0)
		return;
	char **image = rascii(0);
	int c, keep = 1, x, y;
	
	x = randi(0, cfg.display.resolution.width - 7);
	y = randi(0, cfg.display.resolution.height / 2 - 3);
	while (keep)
	{
		image[y][x] = 'S';
		image[y][x + 1] = 'N';
		image[y][x + 2] = 'A';
		image[y][x + 3] = 'F';
		image[y][x + 4] = 'U';
		clear();
		printhud(image);
		if (kbhit())
		{
			c = getchar();
			if (c == 10 || c == 27 || c == ' ')
				keep = 0;
		}
		else
		{
			image[y][x] = ' ';
			image[y][x + 1] = ' ';
			image[y][x + 2] = ' ';
			image[y][x + 3] = ' ';
			image[y][x + 4] = ' ';
			x += randi(-1, 1);
			if (x < 0)
				x = 0;
			else if (x > cfg.display.resolution.width - 7)
				x = cfg.display.resolution.width - 7;
			y += randi(-1, 1);
			if (y < 0)
				y = 0;
			else if (y > cfg.display.resolution.height / 2 - 3)
				y = cfg.display.resolution.height / 2 - 3;
			fsleep(0.2);
		}
	}
}
