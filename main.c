#include <snafu.h>

int main(void)
{
	struct termios work, save;

	tcgetattr(0, &save);
	work = save;
	work.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &work);

	start();
	prints("Hello, World!", DEFAULT_DT);
	prints("\nCette phrase est un test pour voir comment se comportent les fonctions...", DEFAULT_DT);
	prints("\nC'est génial, non ?", DEFAULT_DT);
	system("clear");

	tcsetattr(0, TCSANOW, &save);
	return 0;
}
