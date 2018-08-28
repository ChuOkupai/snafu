#include <snafu.h>

void prints(char *s, useconds_t dt)
{
	while (*s)
	{
		write(1, s, 1);
		usleep(dt);
		s++;
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
