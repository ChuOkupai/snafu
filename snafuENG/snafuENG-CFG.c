#include <snafuENG.h>

typedef struct CFG_HUD
{
	int theme;
	int x;
	int y;
}	CFG_HUD;

typedef struct CFG_MENU
{
	int buttons;
}	CFG_MENU;

typedef struct CFG_TEXT
{
	struct timespec	speed;
}	CFG_TEXT;

typedef struct CFG_SNAFU
{
	bool		cursor;
	bool		engine;
	CFG_HUD		hud;
	CFG_MENU	menu;
	CFG_TEXT	text;
}	CFG_SNAFU;
CFG_SNAFU cfg;

void loadcfg()
{
	FILE *f = 0;
	char buf[16];
	float s;
	int theme;

	cfg.cursor = 1;
	cfg.hud.theme = 1;
	cfg.hud.x = 80;
	cfg.hud.y = 12;
	cfg.menu.buttons = 3;
	cfg.text.speed.tv_sec = 0;
	cfg.text.speed.tv_nsec = 50000000;
	f = fopen("data/game.cfg", "r");
	if (! f)
	{
		werror(ERROR_OPEN, "data/game.cfg", "loadcfg");
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
					else
						wwarning(WARNING_CFG, "HUD theme", "loadcfg");
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
					else
						wwarning(WARNING_CFG, "text speed", "loadcfg");
					break;
				}
			}
		}
	}
	if (fclose(f))
		werror(ERROR_CLOSE, "data/game.cfg", "loadcfg");
}

void savecfg()
{
	FILE *f = 0;
	
	f = fopen("data/game.cfg", "w");
	if (! f)
	{
		werror(ERROR_OPEN, "data/game.cfg", "editcfg");
		return;
	}
	fprintf(f, "[HUD]\n");
	fprintf(f, "theme=%d\n\n", cfg.hud.theme);
	fprintf(f, "[TEXT]\n");
	fprintf(f, "speed=%f\n", (float)cfg.text.speed.tv_sec + (float)cfg.text.speed.tv_nsec / 100000000);
	if (fclose(f))
		werror(ERROR_CLOSE, "data/game.cfg", "editcfg");
}
