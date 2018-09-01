#ifndef	_SNAFU_H
#define	_SNAFU_H	1

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define ON	1
#define OFF	0

#define RESET		"\033[0m"
#define BLACK		"\033[30m"
#define RED			"\033[31m"
#define GREEN		"\033[32m"
#define YELLOW		"\033[33m"
#define BLUE		"\033[34m"
#define MAGENTA		"\033[35m"
#define CYAN		"\033[36m"
#define WHITE		"\033[37m"
#define BOLDBLACK	"\033[1m\033[30m"
#define BOLDRED		"\033[1m\033[31m"
#define BOLDGREEN	"\033[1m\033[32m"
#define BOLDYELLOW	"\033[1m\033[33m"
#define BOLDBLUE	"\033[1m\033[34m"
#define BOLDMAGENTA	"\033[1m\033[35m"
#define BOLDCYAN	"\033[1m\033[36m"
#define BOLDWHITE	"\033[1m\033[37m"

/* écrit une message d'erreur dans le fichier log */
#define ERROR_OPEN		-101
#define ERROR_CLOSE		-102
#define ERROR_MEMORY	-103
void werror(int error, char *data, char *function);

/* écrit une message d'avertissement dans le fichier log */
#define WARNING_CFG	-201
void wwarning(int warning, char *data, char *function);

/* nettoie l'écran */
void clear();

/* attend un nombre de secondes s */
void fsleep(float s);

/* renvoie un int aléatoire entre min et max */
int randi(int min, int max);

/* détecte si une touche du clavier a été frappée */
/** renvoie une valeur non nulle si vrai **/
int kbhit();

/* active ou désactive le curseur */
void setcur(int on);

/* charge le fichier de configuration */
void loadcfg();

/* initialise snafu */
void setengine(int on);

/* charge une image ascii personnalisée */
char** loadascii(char *path);

/* affiche une chaîne de caractères avec un délai entre chaque caractère */
/** le joueur peut passer l'animation en appuyant sur la touche entrée **/
void prints(char *s);

/* affiche le menu principal et attend une sélection du joueur */
int mainmenu();

/* affiche l'interface avec une image ascii personnalisée */
void rhud(char **image);

/* affiche SNAFU animé avec un déplacement aléatoire */
/** pour arrêter l'animation, appuyez sur échap, entrée ou espace **/
void snafufx();

#endif /* snafu.h  */
