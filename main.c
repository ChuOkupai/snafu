#include <snafu.h>
#define DT	0.06

void testhud(char *tlc, char *trc, char *llc, char *lrc, char *hl, char *vl)
{
	system("clear");
	hud(tlc, trc, llc, lrc, hl, vl);
	prints("HUD test", DT);
}

int main(void)
{
	struct termios work, save;

	tcgetattr(0, &save);
	work = save;
	work.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &work);

	start();
	prints("Hello, World!", DT);
	prints("\nCette phrase est un test pour voir comment se comportent les fonctions...", DT);
	prints("\nC'est génial, non ?", DT);
	testhud(HUD_CFG_0);
	testhud(HUD_CFG_1);
	testhud(HUD_CFG_2);
	system("clear");

	tcsetattr(0, TCSANOW, &save);
	return 0;
}
