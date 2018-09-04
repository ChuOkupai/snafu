#include <snafuENG.h>
#include <snafuENG-CFG.c>
#include <snafuENG-LOG.c>
#include <snafuENG-SYS.c>

char** loadascii(char *path)
{
	checkeng("loadascii");
	FILE *f = 0;
	int c = 0, i, j, x = cfg.hud.x - 2, y = cfg.hud.y - 2;
	char **ascii;
	
	ascii = (char**)malloc(sizeof(char*) * y);
	if (! ascii)
	{
		werror(ERROR_MEMORY, 0, "loadascii");
		return 0;
	}
	for (i = 0; i < y; i++)
	{
		ascii[i] = malloc(x);
		if (! ascii[i])
		{
			werror(ERROR_MEMORY, 0, "loadascii");
			free(ascii);
			return 0;
		}
	}
	if (path)
	{
		f = fopen(path, "r");
		if (! f)
			werror(ERROR_OPEN, path, "loadascii");
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
							werror(ERROR_CLOSE, path, "loadascii");
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
			werror(ERROR_CLOSE, path, "loadascii");
	return ascii;
}

void prints(char *s)
{
	checkeng("prints");
	int resetcur;

	if (! cfg.cursor)
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

int mainmenu()
{
	checkeng("mainmenu");
	int c = 0, selection = 0;

	while (! selection || c != '\n')
	{
		clear();
		printf("    ┌───────────┐\n    │ ");
		printf(BOLDWHITE "MAIN MENU │\n");
		printf("    └───────────┘\n\n");
		if (selection == 1)
			putchar('>');
		printf(" New Game\n");
		if (selection == 2)
			putchar('>');
		printf(" Load Game\n");
		if (selection == 3)
			putchar('>');
		printf(" Quit\n" RESET);
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

void rhud(char **image)
{
	checkeng("rhud");
	if (cfg.hud.x < 2 || cfg.hud.y < 2)
		return;
	int i, j;
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

	printf("%s", tlc);
	for (i = 0; i < cfg.hud.x - 2; i++)
		printf("%s", hl);
	puts(trc);
	for (i = 0; i < cfg.hud.y - 2; i++)
	{
		for (j = 0; j < cfg.hud.x + 1; j++)
		{
			if (j && j < cfg.hud.x - 1)
			{
				if (image)
					printf("%c", image[i][j - 1]);
				else
					putchar(' ');
			}
			else if (j != cfg.hud.x)
				printf("%s", vl);
			else
				putchar('\n');
		}
	}
	printf("%s", llc);
	for (i = 0; i < cfg.hud.x - 2; i++)
		printf("%s", hl);
	puts(lrc);
}

void snafufx()
{
	if (cfg.hud.x - 9 < 0 || cfg.hud.y - 5 < 0)
		return;
	char **image = loadascii(0);
	int c, keep = 1, x, y;
	
	x = randi(0, cfg.hud.x - 7);
	y = randi(0, cfg.hud.y - 3);
	while (keep)
	{
		image[y][x] = 'S';
		image[y][x + 1] = 'N';
		image[y][x + 2] = 'A';
		image[y][x + 3] = 'F';
		image[y][x + 4] = 'U';
		clear();
		rhud(image);
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
			else if (x > cfg.hud.x - 7)
				x = cfg.hud.x - 7;
			y += randi(-1, 1);
			if (y < 0)
				y = 0;
			else if (y > cfg.hud.y - 3)
				y = cfg.hud.y - 3;
			fsleep(0.2);
		}
	}
}
