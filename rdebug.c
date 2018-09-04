#include <snafuENG.h>

int main()
{
	const int code = rdebug();
	
	if (code)
	{
		printf("*** ERROR - Could not ");
		if (code == ERROR_OPEN || code == ERROR_CLOSE)
		{
			printf((code == ERROR_OPEN) ? "open" : "close");	
			puts(" game.debug!");
		}
		else
			puts("retrieve all variables!");
	}
	return code;
}
