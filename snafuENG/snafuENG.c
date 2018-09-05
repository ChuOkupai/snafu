#include <snafuENG.h>
#include <snafuENG-CFG.c>
#include <snafuENG-LOG.c>
#include <snafuENG-SYS.c>

#define TLC	0
#define TRC	1
#define LLC	2
#define LRC	3
#define HL	4
#define VL	5
const char** loadhudtheme()
{
	const char **hud = (const char**)malloc(6 * sizeof(const char*));
	
	if (! hud)
	{
		werror(ERROR_MEMORY, 0, __func__);
		return 0;
	}
	for (int i = 0; i < 6; i++)
	{
		hud[i] = malloc(3 * sizeof(char));
		if (! hud[i])
		{
			werror(ERROR_MEMORY, 0, __func__);
			free(hud);
			return 0;
		}
	}
	if (cfg.hud.theme == 1)
	{
		hud[TLC] = "┌";
		hud[TRC] = "┐";
		hud[LLC] = "└";
		hud[LRC] = "┘";
		hud[HL] = "─";
		hud[VL] = "│";
	}
	else if (cfg.hud.theme == 2)
	{
		hud[TLC] = "╔";
		hud[TRC] = "╗";
		hud[LLC] = "╚";
		hud[LRC] = "╝";
		hud[HL] = "═";
		hud[VL] = "║";
	}
	else
	{
		hud[TLC] = " ";
		hud[TRC] = " ";
		hud[LLC] = " ";
		hud[LRC] = " ";
		hud[HL] = " ";
		hud[VL] = " ";
	}
	return hud;
}

char** rascii(const char *path)
{
	checkeng(__func__);
	FILE *f = 0;
	int c = 0, i, j, x = cfg.display.resolution.width - 2, y = cfg.display.resolution.height - 2;
	char **ascii = (char**)malloc(sizeof(char*) * y);
	
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

void printtitle(const char **hud, const char *name)
{
	const int len = strlen(name) + 2;
	int i;
	
	if (! hud)
	{
		printf(COLOR_CFG "\n\n      %s\n\n\n" COLOR_RESET, cfg.hud.color, name);
		return;
	}
	printf(COLOR_CFG "\n    %s", cfg.hud.color, hud[TLC]);
	for (i = 0; i < len; i++)
		printf("%s", hud[HL]);
	printf("%s\n    %s ", hud[TRC], hud[VL]);
	printf(COLOR_RESET "%s", name);
	printf(COLOR_CFG " %s\n    %s", cfg.hud.color, hud[VL], hud[LLC]);
	for (i = 0; i < len; i++)
		printf("%s", hud[HL]);
	printf("%s\n\n" COLOR_RESET, hud[LRC]);
}

bool printoptionsmenu()
{
	checkeng(__func__);
	CFG_SNAFU oldcfg = cfg, newcfg = cfg;
	bool modified = 0, reload = 0;
	int c = 0, selection = 0;
	const char **hud = loadhudtheme();
	
	if (! hud)
		werror(ERROR_RENDER, "main menu", __func__);
	while (! selection || c != '\n')
	{
		clear();
		printtitle(hud, "OPTIONS");
		printf(COLOR_BOLDWHITE);
		if (selection == 1)
			putchar('>');
		printf(" Resolution : ");
		if (selection != 1)
			putchar(' ');
		printf("%d x %d\n", newcfg.display.resolution.width, newcfg.display.resolution.height);
		if (selection == 2)
			putchar('>');
		printf(" Color :      ");
		if (selection != 2)
			putchar(' ');
		switch (newcfg.hud.color)
		{
			case 30:
				printf("black");
				break;
			case 31:
				printf("red");
				break;
			case 32:
				printf("green");
				break;
			case 33:
				printf("yellow");
				break;
			case 34:
				printf("blue");
				break;
			case 35:
				printf("magenta");
				break;
			case 36:
				printf("cyan");
				break;
			case 37:
				printf("white");
				break;
			default:
				printf("terminal settings");
				break;
		}
		putchar('\n');
		if (selection == 3)
			putchar('>');
		printf(" Theme :      ");
		if (selection != 3)
			putchar(' ');
		switch (newcfg.hud.theme)
		{
			case 1:
				printf("simple line");
				break;
			case 2:
				printf("double line");
				break;
			default:
				printf("empty");
				break;
		}
		putchar('\n');
		if (selection == 4)
			putchar('>');
		printf(" Text speed : ");
		if (selection != 4)
			putchar(' ');
		printf("%f\n\n", (float)newcfg.text.speed.tv_sec + (float)newcfg.text.speed.tv_nsec / 1000000000);
		if (modified)
		{
			if (selection == 5)
				putchar('>');
			printf(" Apply\n");
			if (selection == 6)
				putchar('>');
			printf(" Cancel\n");
		}
		if (selection == 7)
				putchar('>');
			printf(" Reset\n");
		if (selection == 8)
			putchar('>');
		printf(" Back\n" COLOR_RESET);
		while (! kbhit())
			fsleep(0.07);
		if (getarrow(&c))
		{
			if (c == KEY_UP)
			{
				selection--;
				if (! modified && (selection == 5 || selection == 6))
					selection = 4;
				else if (selection < 1)
					selection = 8;
			}
			else if (c == KEY_DOWN)
			{
				selection++;
				if (! modified && (selection == 5 || selection == 6))
					selection = 7;
				else if (selection > 8)
					selection = 1;
			}
			else if (c == KEY_RIGHT)
			{
				if (selection == 1 && newcfg.display.resolution.width < 160)
				{
					newcfg.display.resolution.width += 20;
					newcfg.display.resolution.height += 6;
					modified = 1;
				}
				else if (selection == 2)
				{
					if (newcfg.hud.color == 0)
					{
						newcfg.hud.color = 30;
						modified = 1;
					}
					else if (newcfg.hud.color < 37)
					{
						newcfg.hud.color++;
						modified = 1;
					}
				}
				else if (selection == 3 && newcfg.hud.theme < 2)
				{
					newcfg.hud.theme++;
					modified = 1;
				}
				else if (selection == 4 && newcfg.text.speed.tv_sec < 1)
				{
					if (newcfg.text.speed.tv_nsec > 990000000)
					{
						newcfg.text.speed.tv_sec = 1;
						newcfg.text.speed.tv_nsec = 0;
						modified = 1;
					}
					else
					{
						newcfg.text.speed.tv_nsec += 10000000;
						modified = 1;
					}
				}
			}
			else if (c == KEY_LEFT)
			{
				if (selection == 1 && newcfg.display.resolution.width > 80)
				{
					newcfg.display.resolution.width -= 20;
					newcfg.display.resolution.height -= 6;
					modified = 1;
				}
				else if (selection == 2)
				{
					if (newcfg.hud.color == 30)
					{
						newcfg.hud.color = 0;
						modified = 1;
					}
					else if (newcfg.hud.color > 30)
					{
						newcfg.hud.color--;
						modified = 1;
					}
				}
				else if (selection == 3 && newcfg.hud.theme > 0)
				{
					newcfg.hud.theme--;
					modified = 1;
				}
				else if (selection == 4)
				{
					if (newcfg.text.speed.tv_sec >= 1)
					{
						newcfg.text.speed.tv_sec--;
						if (! newcfg.text.speed.tv_sec)
							newcfg.text.speed.tv_nsec = 990000000;
						modified = 1;
					}
					else if (newcfg.text.speed.tv_nsec > 50000000)
					{
						newcfg.text.speed.tv_nsec -= 10000000;
						modified = 1;
					}
				}
			}
		}
		else if (c == '\n')
		{
			if (modified && selection == 5)
			{
				cfg = newcfg;
				hud = loadhudtheme();
				if (! hud)
					werror(ERROR_RENDER, "options submenu", __func__);
				reload = 1;
			}
			else if (modified && selection == 6)
			{
				cfg = oldcfg;
				newcfg = cfg;
				modified = 0;
				hud = loadhudtheme();
				if (! hud)
					werror(ERROR_RENDER, "options submenu", __func__);
				reload = 1;
			}
			else if (selection == 7)
			{
				setdefcfg();
				newcfg = cfg;
				modified = 0;
				hud = loadhudtheme();
				if (! hud)
					werror(ERROR_RENDER, "options submenu", __func__);
				reload = 1;
			}
			if (selection != 8)
				c = 0;
		}
	}
	free(hud);
	return reload;
}

int printmainmenu()
{
	checkeng(__func__);
	int c = 0, selection = 0;
	const char **hud = loadhudtheme();
	
	if (! hud)
		werror(ERROR_RENDER, "main menu", __func__);
	while (! selection || c != '\n')
	{
		clear();
		printtitle(hud, "MAIN MENU");
		printf(COLOR_BOLDWHITE);
		if (selection == 1)
			putchar('>');
		printf(" New Game\n");
		if (selection == 2)
			putchar('>');
		printf(" Load Game\n");
		if (selection == 3)
			putchar('>');
		printf(" Options\n");
		if (selection == 4)
			putchar('>');
		printf(" Quit\n" COLOR_RESET);
		while (! kbhit())
			fsleep(0.07);
		if (getarrow(&c))
		{
			if (c == KEY_UP)
			{
				selection--;
				if (selection < 1)
					selection = 4;
			}
			else if (c == KEY_DOWN)
			{
				selection++;
				if (selection > 4)
					selection = 1;
			}
		}
		else if (selection == 3 && c == '\n')
		{
			if (printoptionsmenu())
			{
				wcfg();
				hud = loadhudtheme();
				if (! hud)
					werror(ERROR_RENDER, "main menu", __func__);
			}
			c = 0;
		}
	}
	free(hud);
	return selection;
}

void printhud(char **image)
{
	checkeng(__func__);
	if (cfg.display.resolution.width < 2 || cfg.display.resolution.height < 2)
		return;
	int i, j, x = cfg.display.resolution.width, y = cfg.display.resolution.height / 2;
	const char **hud = loadhudtheme();
	
	if (! hud)
	{
		werror(ERROR_RENDER, "hud", __func__);
		wdebug();
		fexit();
	}
	printf(COLOR_CFG "%s", cfg.hud.color, hud[TLC]);
	for (i = 0; i < x - 2; i++)
		printf("%s", hud[HL]);
	printf("%s\n" COLOR_RESET, hud[TRC]);
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
				printf(COLOR_CFG "%s" COLOR_RESET, cfg.hud.color, hud[VL]);
			else
				putchar('\n');
		}
	}
	printf(COLOR_CFG "%s", cfg.hud.color, hud[LLC]);
	for (i = 0; i < x - 2; i++)
		printf("%s", hud[HL]);
	printf("%s\n" COLOR_RESET, hud[LRC]);
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
