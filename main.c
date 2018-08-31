#include <snafuENG.h>

int main(void)
{
	setengine(ON);
	printf("mainmenu() return value = %d\n", mainmenu());
	setengine(OFF);
	return 0;
}
