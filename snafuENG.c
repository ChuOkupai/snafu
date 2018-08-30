#include <snafuENG.h>

struct CFG_HUD
{
	int theme;
	int x;
	int y;
};

struct CFG_TEXT
{
	int		maxlength;
	float	speed;
};

struct CFG_SNAFU
{
	int				cursor;
	struct CFG_HUD	hud;
	struct CFG_TEXT	text;
};
struct CFG_SNAFU cfg;

void clear()
{
	system("clear");
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

void setengine(int on)
{
	static struct termios work, save;
	
	if (on)
	{
		tcgetattr(0, &save);
		work = save;
		work.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &work);
		srand(time(NULL));
		clear();
		cfg.cursor = 1;
		cfg.hud.theme = 1;
		cfg.hud.x = 80;
		cfg.hud.y = 12;
		cfg.text.maxlength = 78;
		cfg.text.speed = 0.05;
		setcur(OFF);
	}
	else
	{
		tcsetattr(0, TCSANOW, &save);
		setcur(ON);
		clear();
	}
}
 
int kbhit(void)
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

void prints(char *s)
{
	struct timespec t;
	int resetcur;

	t.tv_sec = (int)cfg.text.speed;
	t.tv_nsec = 1000000000 * (cfg.text.speed - t.tv_sec);
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
			nanosleep(&t, NULL);
			if (kbhit())
				c = getchar();
		}
	}
	if (resetcur)
		setcur(OFF);
}

int rdial(char *image, char *dialog)
{
	FILE *f;
	char s[cfg.text.maxlength];
	int i, j, c, dial, sep;
	
	clear();
	if (image)
	{
		f = fopen(image, "r");
		if (! f)
		{
			printf("error\n");
			return -1;
		}
	}
	printf("┌");
	for (i = 0; i < cfg.hud.x - 2; i++)
		printf("─");
	puts("┐");
	for (i = 0; i < cfg.hud.y - 2; i++)
	{
		for (j = 0; j < cfg.hud.x + 1; j++)
		{
			if ((j > 0 && j < cfg.hud.x - 1) || j == cfg.hud.x)
			{
				if (image)
				{
					c = fgetc(f);
					if (c == EOF)
					{
						fclose(f);
						image = 0;
					}
					else
						putchar(c);
				}
				else
				{
					if (j == cfg.hud.x)
						putchar('\n');
					else
						putchar(' ');
				}
			}
			else
				printf("│");
		}
	}
	if (image)
		fclose(f);
	printf("└");
	for (i = 0; i < cfg.hud.x - 2; i++)
		printf("─");
	puts("┘");
	if (dialog)
	{
		f = fopen(dialog, "r");
		if (! f)
		{
			printf("error\n");
			return -2;
		}
		c = fgetc(f);
		dial = 0;
		sep = 0;
		while (c != EOF)
		{
			if (c == '$')
			{
				fgets(s, cfg.text.maxlength, f);
				s[strlen(s) - 1] = '\0';
				printf(" %s :\n\n", s);
			}
			else if (c == '>')
			{
				printf("   ");
				fgets(s, cfg.text.maxlength, f);
				prints(s);
				dial++;
			}
			else if (c == '*')
			{
				if (! sep && dial)
				{
					putchar('\n');
					sep = 1;
				}
				fgets(s, cfg.text.maxlength, f);
				printf(" * %s", s);
			}
			c = fgetc(f);
		}
		fclose(f);
	}
	return 0;
}
