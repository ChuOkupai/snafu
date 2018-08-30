#include <snafuENG.h>

int main(void)
{
	struct termios work, save;

	tcgetattr(0, &save);
	work = save;
	work.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &work);

	start();
	rdial("data/image/test", 0, CFG_TEXT_SPEED);
	getchar();
	rdial("data/image/test", "data/dialog/test", CFG_TEXT_SPEED);
	getchar();
	rdial(0, "data/dialog/marguerite", CFG_TEXT_SPEED);
	getchar();
	rdial(0, "data/dialog/marguerite_actions", CFG_TEXT_SPEED);
	getchar();

	tcsetattr(0, TCSANOW, &save);
	setcur(1);
	return 0;
}
