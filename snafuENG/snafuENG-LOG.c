#include <snafuENG.h>

/* écrit la date et l'heure courante dans le fichier log */
void wlogtime(FILE *f)
{
	if (! f)
		return;
	time_t t = time(0);
	struct tm tm = *localtime(&t);

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
	fprintf(f, "%d : ", tm.tm_sec);
}

void werror(const int error, const char *data, const char *function)
{
	FILE *f = 0;

	f = fopen(PATH_LOG, "a");
	if (! f)
		return;
	wlogtime(f);
	fprintf(f, "ERROR - ");
	if (error == ERROR_NODESC)
		fprintf(f, "No description.");
	else if (error == ERROR_ENGINE)
		fprintf(f, "Engine not started!");
	else if (error == ERROR_OPEN || error == ERROR_CLOSE)
	{
		fprintf(f, "Could not ");
		fprintf(f, (error == ERROR_OPEN) ? "open" : "close");	
		fprintf(f, " file %s!", data);
	}
	else if (error == ERROR_MEMORY)
		fprintf(f, "Could not allocate memory!");
	else if (error == ERROR_SYSTEM)
		fprintf(f, "System command returned an error!");
	else
		fprintf(f, "%s", data);
	fprintf(f, " (%s)\n", function);
	fclose(f);
}

void wwarning(const int warning, const char *data, const char *function)
{
	FILE *f = 0;

	f = fopen(PATH_LOG, "a");
	if (! f)
		return;
	wlogtime(f);
	fprintf(f, "WARNING - ");
	if (warning == WARNING_NODESC)
		fprintf(f, "No description.");
	else if (warning == WARNING_CFG)
		fprintf(f, "Forbidden value set to %s!", data);
	else if (warning == WARNING_EXIT)
		fprintf(f, "Exit.");
	else
		fprintf(f, "%s", data);
	fprintf(f, " (%s)\n", function);
	fclose(f);
}

void wdebug()
{
	FILE *f = 0;

	f = fopen(PATH_DEBUG, "w");
	if (! f)
		return;
	fprintf(f, "%d,%d\n", cfg.display.resolution.width, cfg.display.resolution.height);
	fprintf(f, "%d,%d\n", cfg.hud.color, cfg.hud.theme);
	fprintf(f, "%d\n", cfg.menu.buttons);
	fprintf(f, "%d,%d\n", cfg.system.cursor, cfg.system.engine);
	fprintf(f, "%ld,%ld\n", cfg.text.speed.tv_sec, cfg.text.speed.tv_nsec);
	fclose(f);
}

int rdebug()
{
	FILE *f = 0;
	CFG_SNAFU dcfg;
	int	n = 0;

	f = fopen(PATH_DEBUG, "r");
	if (! f)
		return ERROR_OPEN;
	if (fscanf(f, "%d,%d", &dcfg.display.resolution.width, &dcfg.display.resolution.height))
	{
		fgetc(f);
		n += 2;
	}
	if (fscanf(f, "%d,%d", &dcfg.hud.color, &dcfg.hud.theme))
	{
		fgetc(f);
		n += 2;
	}
	if (fscanf(f, "%d", &dcfg.menu.buttons))
	{
		fgetc(f);
		n++;
	}
	if (fscanf(f, "%d,%d", &dcfg.system.cursor, &dcfg.system.engine))
	{
		fgetc(f);
		n += 2;
	}
	if (fscanf(f, "%ld,%ld", &dcfg.text.speed.tv_sec, &dcfg.text.speed.tv_nsec))
		n += 2;
	if (n == CFG_TOTAL_ELEMENTS)
	{
		puts("game.debug\n\ncfg:");
		printf("cursor=%d\nengine=%d\n", dcfg.system.cursor, dcfg.system.engine);
		printf("\ncfg.hud:\ncolor=%d\ntheme=%d\n", dcfg.hud.color, dcfg.hud.theme);
		printf("\ncfg.menu:\nbuttons=%d\n", dcfg.menu.buttons);
		printf("\ncfg.text:\nspeed.tv_sec=%ld\nspeed.tv_nsec=%ld\n", dcfg.text.speed.tv_sec, dcfg.text.speed.tv_nsec);
		if (fclose(f))
		return ERROR_CLOSE;
	}
	return CFG_TOTAL_ELEMENTS - n;
}
