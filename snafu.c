#include <snafu.h>
 
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
	if(c != EOF)
	{
		ungetc(c, stdin);
		return 1;
	}
	return 0;
}

void wconfirm()
{
	while (1)
		if (getchar() == '\n')
			return;
}

void prints(char *s, float dt)
{
	struct timespec t;

	t.tv_sec = (int)dt;
	t.tv_nsec = 1000000000 * (dt - t.tv_sec);
	for (int c = 0; *s; s++)
	{
		write(1, s, 1);
		if (c != '\n' && c != ' ')
		{
			nanosleep(&t, NULL);
			if (kbhit())
				c = getchar();
		}
	}
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
	return 0;
}
