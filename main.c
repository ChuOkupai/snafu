#include <snafuENG.h>

int main(void)
{
	setengine(ON);
	rhud(loadascii("data/image/short"));
	getchar();
	snafufx();
	setengine(OFF);
	return 0;
}
