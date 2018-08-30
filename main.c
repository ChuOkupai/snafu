#include <snafuENG.h>

int main(void)
{
	setengine(ON);
	
	rdial("data/image/test", 0, CFG_TEXT_SPEED);
	getchar();
	rdial("data/image/test", "data/dialog/test", CFG_TEXT_SPEED);
	getchar();
	rdial(0, "data/dialog/marguerite", CFG_TEXT_SPEED);
	getchar();
	rdial(0, "data/dialog/marguerite_actions", CFG_TEXT_SPEED);
	getchar();
	
	setengine(OFF);
	return 0;
}
