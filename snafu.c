#include <snafu.h>

void prints(char *s, float dt)
{
	struct timespec t;

	t.tv_sec = (int)dt;
	t.tv_nsec = 1000000000 * (dt - t.tv_sec);
	printf("dt = %ld.%ld\n", t.tv_sec, t.tv_nsec);
	while (*s)
	{
		write(1, s, 1);
		nanosleep(&t, NULL);
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
