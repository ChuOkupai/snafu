#include <snafuENG.h>
#include <snafu.h>

int main()
{
	snf_seteng(true);
	snf_rcfg();
	snf_clear();
	snf_prints("Hello, my name is Jean-Pierre!\nI am currently testing the prints function...\n");
	snf_waitret();
	snf_clear();
	snf_printhud(snf_rasciirle(SNF_PATH_IMG "points"));
	snf_waitret();
	snf_clear();
	snf_printhud(snf_rasciirle(SNF_PATH_IMG "short"));
	snf_waitret();
	snf_printmainmenu();
	snf_clear();
	snf_seteng(false);
	return 0;
}