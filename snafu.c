#include <snafu.h>

void clear()
{
	system("clear");
}

void setcur(int on)
{
	if (on)
		system("setterm -cursor on");
	else
		system("setterm -cursor off");
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

	t.tv_sec = (int)dt;
	t.tv_nsec = 1000000000 * (dt - t.tv_sec);
	setcur(1);
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
	getchar();
	setcur(0);
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
