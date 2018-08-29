#include <snafu.h>

int main(void)
{
	start();
	prints("Hello, World!\n", DEFAULT_DT);
	wconfirm();
	prints("Cette phrase est un test pour voir comment se comportent les fonctions...\n", DEFAULT_DT);
	wconfirm();
	prints("C'est génial, non ?\n", DEFAULT_DT);
	wconfirm();
	return 0;
}
