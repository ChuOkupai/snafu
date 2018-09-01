#include <snafuENG.h>

struct CFG_HUD
{
	int theme;
	int x;
	int y;
};

struct CFG_MENU
{
	int maxchoice;
};

struct CFG_TEXT
{
	int				maxlength;
	struct timespec	speed;
};

struct CFG_SNAFU
{
	int				cursor;
	struct CFG_HUD	hud;
	struct CFG_MENU	menu;
	struct CFG_TEXT	text;
};
struct CFG_SNAFU cfg;

/* écrit la date et l'heure courante dans le fichier log */
void wlogtime(FILE *f)
{
	if (! f)
		return;
	time_t t = time(0);
	struct tm tm = *localtime(&t);

	fprintf(f, "%d-", tm.tm_year + 1900);
	if (tm.tm_mon + 1 < 10)
		fputc('0', f);
	fprintf(f, "%d-", tm.tm_mon + 1);
	if (tm.tm_mday < 10)
		fputc('0', f);
	fprintf(f, "%d ", tm.tm_mday);
	if (tm.tm_hour < 10)
		fputc('0', f);
	fprintf(f, "%d:", tm.tm_hour);
	if (tm.tm_min < 10)
		fputc('0', f);
	fprintf(f, "%d:", tm.tm_min);
	if (tm.tm_sec < 10)
		fputc('0', f);
	fprintf(f, "%d : ", tm.tm_sec);
}

void werror(int error, char *data, char *function)
{
	FILE *f = 0;

	f = fopen("data/game.log", "a");
	if (! f)
		return;
	wlogtime(f);
	fprintf(f, "ERROR - ");
	if (error == ERROR_OPEN || error == ERROR_CLOSE)
	{
		fprintf(f, "Could not ");
		fprintf(f, (error == ERROR_OPEN) ? "open" : "close");	
		fprintf(f, " file %s!", data);
	}
	else if (error == ERROR_MEMORY)
		fprintf(f, "Could not allocate memory!");
	else
		fprintf(f, "No description.");
	fprintf(f, " (%s)\n", function);
	fclose(f);
}

void wwarning(int warning, char *data, char *function)
{
	FILE *f = 0;

	f = fopen("data/game.log", "a");
	if (! f)
		return;
	wlogtime(f);
	fprintf(f, "WARNING - ");
	if (warning == WARNING_CFG)
		fprintf(f, "Forbidden value set to %s!", data);
	else
		fprintf(f, "No description.");
	fprintf(f, " (%s)\n", function);
	fclose(f);
}

void clear()
{
	system("clear");
}

void fsleep(float s)
{
	struct timespec	t;

	t.tv_sec = (int)s;
	t.tv_nsec = 1000000000 * (s - t.tv_sec);
	nanosleep(&t, 0);
}

int randi(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

int kbhit()
{
	struct termios oldt, newt;
	int c, oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	c = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (c != EOF)
	{
		ungetc(c, stdin);
		return 1;
	}
	return 0;
}

void setcur(int on)
{
	if (! cfg.cursor && on)
	{
		system("setterm -cursor on");
		cfg.cursor = 1;
	}
	else if (! on && cfg.cursor)
	{
		system("setterm -cursor off");
		cfg.cursor = 0;
	}
}

void loadcfg()
{
	FILE *f = 0;
	char buf[16];
	float s;
	int theme;

	cfg.cursor = 1;
	cfg.hud.theme = 1;
	cfg.hud.x = 80;
	cfg.hud.y = 12;
	cfg.menu.maxchoice = 3;
	cfg.text.maxlength = 78;
	cfg.text.speed.tv_sec = 0;
	cfg.text.speed.tv_nsec = 50000000;
	f = fopen("data/game.cfg", "r");
	if (! f)
	{
		werror(ERROR_OPEN, "data/game.cfg", "loadcfg");
		return;
	}
	while (fgets(buf, 16, f))
	{
		if (strstr(buf, "[HUD]"))
		{
			while (fgets(buf, 16, f))
			{
				if (strstr(buf, "theme="))
				{
					sscanf(buf, "theme=%d", &theme);
					if (theme >= 0 && theme <= 2)
						cfg.hud.theme = theme;
					else
						wwarning(WARNING_CFG, "HUD theme", "loadcfg");
					break;
				}
			}
		}
		else if (strstr(buf, "[TEXT]"))
		{
			while (fgets(buf, 16, f))
			{
				if (strstr(buf, "speed="))
				{
					sscanf(buf, "speed=%f", &s);
					if ((int)s >= 0 && (int)s <= 2)
					{
						cfg.text.speed.tv_sec = (int)s;
						cfg.text.speed.tv_nsec = 1000000000 * (s - (int)s);
					}
					else
						wwarning(WARNING_CFG, "text speed", "loadcfg");
					break;
				}
			}
		}
	}
	if (fclose(f))
		werror(ERROR_CLOSE, "data/game.cfg", "loadcfg");
}

void setengine(int on)
{
	static struct termios current, save;

	if (on)
	{
		tcgetattr(0, &save);
		current = save;
		current.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &current);
		clear();
		loadcfg();
		setcur(OFF);
		srand(time(0));
	}
	else
	{
		tcsetattr(0, TCSANOW, &save);
		setcur(ON);
		clear();
	}
}

char** loadascii(char *path)
{
	FILE *f = 0;
	int c, i, j, x = cfg.hud.x - 2, y = cfg.hud.y - 2;
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
	int resetcur;

	if (! cfg.cursor)
	{
		setcur(ON);
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
		setcur(OFF);
}

int mainmenu()
{
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
						selection = cfg.menu.maxchoice;
				}
				else if (c == 66)
				{
					selection++;
					if (selection > cfg.menu.maxchoice)
						selection = 1;
				}
			}
		}
	}
	return selection;
}

void rhud(char **image)
{
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
