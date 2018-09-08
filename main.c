#include <snafuENG.h>

int main(void)
{
	setengine(true);
	rcfg();
	clear();
	printhud(rasciirle("data/image/short"));
	getchar();
	printmainmenu();
	setengine(false);
	clear();
	return 0;
}
