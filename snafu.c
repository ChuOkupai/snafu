#include <snafu.h>

void clear()
{
	system("clear");
}

void setcur(int on)
{
	if (! w.cursor && on)
	{
		system("setterm -cursor on");
		w.cursor = 1;
	}
	else if (! on && w.cursor)
	{
		system("setterm -cursor off");
		w.cursor = 0;
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

void prints(char *s, float dt)
{
	struct timespec t;
	int resetcur;

	t.tv_sec = (int)dt;
	t.tv_nsec = 1000000000 * (dt - t.tv_sec);
	if (! w.cursor)
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
			nanosleep(&t, NULL);
			if (kbhit())
				c = getchar();
		}
	}
	if (resetcur) setcur(0);
}

int randi(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

float randf()
{
	return (float)rand() / (float)RAND_MAX;
}

int start()
{
	srand(time(NULL));
	clear();
	setcur(0);
	return 0;
}

void hud(char *tlc, char *trc, char *llc, char *lrc, char *hl, char *vl)
{
	int i, j;
	
	printf("%s", tlc);
	for (i = 0; i < 78; i++)
		printf("%s", hl);
	puts(trc);
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 81; j++)
		{
			if (j > 0 && j < 79)
				putchar(' ');
			else if (j != 80)
				printf("%s", vl);
			else
				putchar('\n');
		}
	}
	printf("%s", llc);
	for (i = 0; i < 78; i++)
		printf("%s", hl);
	puts(lrc);
}

int rdial(char *image, char *dialog, float dt)
{
	FILE *f;
	char s[CFG_TEXT_LENGTH];
	int i, j, c, dial, sep;
	
	f = fopen(image, "r");
	if (! f)
	{
		printf("error\n");
		return -1;
	}
	clear();
	printf("┌");
	for (i = 0; i < CFG_IMG_X; i++)
		printf("─");
	puts("┐");
	for (i = 0; i < CFG_IMG_Y; i++)
	{
		for (j = 0; j < CFG_IMG_X + 3; j++)
		{
			if ((j > 0 && j < CFG_IMG_X + 1) || j == CFG_IMG_X + 2)
				putchar(fgetc(f));
			else
				printf("│");
		}
	}
	fclose(f);
	printf("└");
	for (i = 0; i < CFG_IMG_X; i++)
		printf("─");
	puts("┘");
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
			fgets(s, CFG_TEXT_LENGTH, f);
			s[strlen(s) - 1] = '\0';
			printf(" %s :\n\n", s);
		}
		else if (c == '>')
		{
			printf("   ");
			fgets(s, CFG_TEXT_LENGTH, f);
			prints(s, dt);
			dial++;
		}
		else if (c == '*')
		{
			if (! sep && dial)
			{
				putchar('\n');
				sep = 1;
			}
			fgets(s, CFG_TEXT_LENGTH, f);
			printf(" * %s", s);
		}
		c = fgetc(f);
	}
	fclose(f);
	return 0;
}
