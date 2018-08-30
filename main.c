#include <snafuENG.h>

int main(void)
{
	setengine(ON);
	
	rdial("data/image/test", 0);
	getchar();
	rdial("data/image/test", "data/dialog/test");
	getchar();
	rdial(0, "data/dialog/marguerite");
	getchar();
	rdial(0, "data/dialog/marguerite_actions");
	getchar();
	
	setengine(OFF);
	return 0;
}
