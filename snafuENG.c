#include <snafuENG.h>

struct CFG_HUD
{
	int theme;
	int x;
	int y;
};

struct CFG_TEXT
{
	int				maxlength;
	struct timespec	speed;
};

struct CFG_SNAFU
{
	int				cursor;
	struct CFG_HUD	hud;
	struct CFG_TEXT	text;
};
struct CFG_SNAFU cfg;

void writerr(char *error)
{
	FILE *f = NULL;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	f = fopen("data/game.log", "a");
	if (! f)
		return;
	fprintf(f, "%d-", tm.tm_year + 1900);
	if (tm.tm_mon + 1 < 10)
		fputc('0', f);
	fprintf(f, "%d-", tm.tm_mon + 1);
	if (tm.tm_mday < 10)
		fputc('0', f);
	fprintf(f, "%d ", tm.tm_mday);
	if (tm.tm_hour < 10)
		fputc('0', f);
	fprintf(f, "%d:", tm.tm_hour);
	if (tm.tm_min < 10)
		fputc('0', f);
	fprintf(f, "%d:", tm.tm_min);
	if (tm.tm_sec < 10)
		fputc('0', f);
	fprintf(f, "%d : ERROR - %s\n", tm.tm_sec, error);
	fclose(f);
}

void clear()
{
	system("clear");
}

void setcur(int on)
{
	if (! cfg.cursor && on)
	{
		system("setterm -cursor on");
		cfg.cursor = 1;
	}
	else if (! on && cfg.cursor)
	{
		system("setterm -cursor off");
		cfg.cursor = 0;
	}
}

void loadcfg()
{
	FILE *f = NULL;
	char buf[16];
	float s;
	int theme;
	
	f = fopen("data/game.cfg", "r");
	if (! f)
	{
		writerr("Could not open game.cfg!");
		return;
	}
	while (fgets(buf, 16, f))
	{
		if (strstr(buf, "[HUD]"))
		{
			while (fgets(buf, 16, f))
			{
				if (strstr(buf, "theme="))
				{
					sscanf(buf, "theme=%d", &theme);
					if (theme >= 0 && theme <= 2)
						cfg.hud.theme = theme;
					break;
				}
			}
		}
		else if (strstr(buf, "[TEXT]"))
		{
			while (fgets(buf, 16, f))
			{
				if (strstr(buf, "speed="))
				{
					sscanf(buf, "speed=%f", &s);
					if ((int)s >= 0 && (int)s <= 2)
					{
						cfg.text.speed.tv_sec = (int)s;
						cfg.text.speed.tv_nsec = 1000000000 * (s - (int)s);
					}
					break;
				}
			}
		}
	}
	if (fclose(f))
		writerr("Could not close game.cfg!");
}

void setengine(int on)
{
	static struct termios work, save;
	
	if (on)
	{
		tcgetattr(0, &save);
		work = save;
		work.c_lflag &= ~ECHO;
		tcsetattr(0, TCSANOW, &work);
		clear();
		cfg.cursor = 1;
		cfg.hud.theme = 0;
		cfg.hud.x = 80;
		cfg.hud.y = 12;
		cfg.text.maxlength = 78;
		cfg.text.speed.tv_sec = 0;
		cfg.text.speed.tv_nsec = 50000000;
		loadcfg();
		setcur(OFF);
	}
	else
	{
		tcsetattr(0, TCSANOW, &save);
		setcur(ON);
		clear();
	}
}
 
int kbhit(void)
{
	struct termios oldt, newt;
	int c, oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	c = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (c != EOF)
	{
		ungetc(c, stdin);
		return 1;
	}
	return 0;
}

void prints(char *s)
{
	int resetcur;

	if (! cfg.cursor)
	{
		setcur(ON);
		resetcur = 1;
	}
	else resetcur = 0;
	for (int c = 0; *s; s++)
	{
		putchar(*s);
		if (c != '\n')
		{
			nanosleep(&cfg.text.speed, NULL);
			if (kbhit())
				c = getchar();
		}
	}
	if (resetcur)
		setcur(OFF);
}

/*Mathis WAS HERE*/

char getch()
{
  struct termios old_tio, new_tio;
  unsigned char c;

  tcgetattr(STDIN_FILENO,&old_tio);
  new_tio=old_tio;
  new_tio.c_lflag &=(~ICANON & ~ECHO);

  tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
  c = getchar();
  tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
  
  return c;
}

int main_menu()
{
	int prechoice = -1;
	int choice = 0;
	int min_choice, max_choice;
	
	min_choice = 1; max_choice = 3;
	
	char c;
	
	while(!choice)
	{
		printf("====Main Menu====\n");
		
		if(prechoice == 1) printf("> ");
		printf("New Game\n");
		if(prechoice == 2) printf("> ");
		printf("Load Game\n");
		if(prechoice == 3) printf("> ");
		printf("Quit\n");
		
		c = getch();
		if (c == 27){getch(), c = getch();}
		
		if (c == 10) choice = prechoice;
		
		
		if(c == 66) prechoice++;
		if(c == 65) prechoice--;
		
		if(prechoice < min_choice && prechoice != -1) prechoice = max_choice ;
		if(prechoice > max_choice && prechoice != -1) prechoice = min_choice ;
		clear();
		
	}
	printf("Choice : %d", choice);
	return choice;
}

char **loadascii(char *path)
{
	FILE* f;
	char c;
	int i,j;
	int x = cfg.hud.y - 2; int y = cfg.hud.x - 2;
	/*Init*/
	char **ascii = (char**)malloc(sizeof(char*) * y);
	if(!ascii){printf("Error malloc ascii");exit(1);}
	for(i = 0; i < y; i++)
	{
		ascii[i] = (char*)malloc(sizeof(char) * x);
		if(!ascii[i]){printf("Error malloc ascii");exit(1);}
	}
	
	for(j=0;j<y;j++)
	{
		for(i=0; i<x; i++)
		{
			ascii[j][i] = ' ';
		}
	}

	
	f = fopen(path, "r");
	if (!f)
	{
		printf("Couldn't open %s", path);
		return ascii;
	}
	
	i = j = 0;
	
	c = fgetc(f);
	
	while(c != EOF || (j < y))
	{
		ascii[j][i] = c; 
		
		if(c == '\n'){j++; i = 0;}
		else{ i++;}
		
		if(i >= x)
		{
			j++; i = 0;
		}
		c = fgetc(f);
			
	}
	return ascii;
}

void rhud()
{
	int i, j;
	char *tlc; char *trc; char *llc; char *lrc; char *hl; char *vl;
	
	if(cfg.hud.theme == 1) {tlc="┌"; trc="┐"; llc="└"; lrc="┘"; hl="─"; vl="│";}
	else if(cfg.hud.theme == 2) {tlc="╔"; trc="╗"; llc="╚"; lrc="╝"; hl="═"; vl="║";}
	else{tlc=" "; trc=" "; llc=" "; lrc=" "; hl=" "; vl=" ";}

	
	printf("%s", tlc);
	for (i = 0; i < cfg.hud.x - 2; i++)
		printf("%s", hl);
	puts(trc);
	for (i = 0; i < cfg.hud.y - 2; i++)
	{
		for (j = 0; j < cfg.hud.x + 1; j++)
		{
			if (j > 0 && j < cfg.hud.x - 1)
				//printf("%c", image[i][j]);
				printf(" ");
			else if (j != cfg.hud.x)
				printf("%s", vl);
			else
				putchar('\n');
		}
	}
	printf("%s", llc);
	for (i = 0; i < cfg.hud.x - 2; i++)
		printf("%s", hl);
	puts(lrc);
}
