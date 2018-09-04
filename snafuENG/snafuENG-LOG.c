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

void werror(int error, char *data, char *function)
{
	FILE *f = 0;

	f = fopen("data/game.log", "a");
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

void wwarning(int warning, char *data, char *function)
{
	FILE *f = 0;

	f = fopen("data/game.log", "a");
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

	f = fopen("data/game.debug", "w");
	if (! f)
		return;
	fprintf(f, "%d,%d\n", cfg.cursor, cfg.engine);
	fprintf(f, "%d,%d,%d\n", cfg.hud.theme, cfg.hud.x, cfg.hud.y);
	fprintf(f, "%d\n", cfg.menu.buttons);
	fprintf(f, "%ld,%ld\n", cfg.text.speed.tv_sec, cfg.text.speed.tv_nsec);
	fclose(f);
}
