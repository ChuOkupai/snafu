#include <snafuENG.h>

void setengine(bool on)
{
	static struct termios current, save;
	static bool init = 0;

	if (! init && on)
	{
		srand(time(0));
		tcgetattr(0, &save);
		current = save;
		current.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &current);
		loadcfg();
		setcur(0);
		cfg.engine = 1;
		init = 1;
	}
	else if (! on && init)
	{
		tcsetattr(0, TCSANOW, &save);
		setcur(1);
		cfg.engine = 0;
		init = 0;
	}
	clear();
}

void fexit()
{
	wwarning(WARNING_EXIT, 0, "fexit");
	printf("Snafu was forced to quit.\n");
	printf("Please send your game.debug and game.log files to the developpers :\n");
	printf("https://github.com/ChuOkupai/snafu\n");
	exit(WARNING_EXIT);
}

void checkeng(char *function)
{
	if (! cfg.engine)
	{
		werror(ERROR_ENGINE, 0, function);
		wdebug();
		fexit();
	}
}

void clear()
{
	if (system("clear") == -1)
	{
		werror(ERROR_SYSTEM, 0, "clear");
		if (cfg.engine)
			setengine(0);
		fexit();
	}
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

void setcur(bool on)
{
	bool error = 0;
	
	if (! cfg.cursor && on)
	{
		if (system("setterm -cursor on") == -1)
			error++;
		cfg.cursor = 1;
	}
	else if (! on && cfg.cursor)
	{
		if (system("setterm -cursor off") == -1)
			error++;
		cfg.cursor = 0;
	}
	if (error)
	{
		werror(ERROR_SYSTEM, 0, "setcur");
		if (cfg.engine)
			setengine(0);
		wdebug();
		fexit();
	}
}
