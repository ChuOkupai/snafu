#include <snafuENG.h>

int main(void)
{
	setengine(true);
	rhud(loadascii("data/image/short"));
	getchar();
	snafufx();
	setengine(false);
	return 0;
}
