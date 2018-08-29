#include <snafu.h>

int main(void)
{
	struct termios work, save;

	tcgetattr(0, &save);
	work = save;
	work.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &work);

	start();
	prints("Cette phrase est un test pour voir comment se comportent les fonctions...", CFG_TEXT_SPEED);
	getchar();
	clear();
	hud(CFG_HUD_0);
	getchar();
	clear();

	tcsetattr(0, TCSANOW, &save);
	setcur(1);
	return 0;
}
