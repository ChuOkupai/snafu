#include <snafuENG.h>

void setengine(const bool on)
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
		setdefcfg();
		cfg.system.cursor = 1;
		cfg.system.engine = 1;
		setcur(0);
		init = 1;
	}
	else if (! on && init)
	{
		tcsetattr(0, TCSANOW, &save);
		setcur(1);
		cfg.system.engine = 0;
		init = 0;
	}
}

void fexit()
{
	if (cfg.system.engine)
		setengine(0);
	wwarning(WARNING_EXIT, 0, __func__);
	puts("Snafu was forced to quit.");
	printf("Please send your %s and %s files to the developpers :\n", PATH_DEBUG, PATH_LOG);
	puts("https://github.com/ChuOkupai/snafu");
	exit(WARNING_EXIT);
}

void checkeng(const char *function)
{
	if (! cfg.system.engine)
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
		werror(ERROR_SYSTEM, 0, __func__);
		if (cfg.system.engine)
			setengine(0);
		fexit();
	}
}

void fsleep(const float s)
{
	struct timespec	t;

	t.tv_sec = (int)s;
	t.tv_nsec = 1000000000 * (s - t.tv_sec);
	nanosleep(&t, 0);
}

int randi(const int min, const int max)
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

bool getarrow(int *c)
{
	*c = getchar();
	if (*c == 27)
	{
		*c = getchar();
		if (*c == 91)
		{
			*c = getchar();
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

void setcur(const bool on)
{
	bool error = 0;
	
	if (! cfg.system.cursor && on)
	{
		if (system("setterm -cursor on") == -1)
			error++;
		cfg.system.cursor = 1;
	}
	else if (! on && cfg.system.cursor)
	{
		if (system("setterm -cursor off") == -1)
			error++;
		cfg.system.cursor = 0;
	}
	if (error)
	{
		werror(ERROR_SYSTEM, 0, __func__);
		if (cfg.system.engine)
			setengine(0);
		wdebug();
		fexit();
	}
}
