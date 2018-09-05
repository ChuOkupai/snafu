#ifndef	_SNAFU_H
#define	_SNAFU_H	1

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

typedef int bool;
#define false	0
#define true	1

#define COLOR_CFG			"\033[%dm"
#define COLOR_RESET			"\033[0m"
#define COLOR_BLACK			"\033[30m"
#define COLOR_RED			"\033[31m"
#define COLOR_GREEN			"\033[32m"
#define COLOR_YELLOW		"\033[33m"
#define COLOR_BLUE			"\033[34m"
#define COLOR_MAGENTA		"\033[35m"
#define COLOR_CYAN			"\033[36m"
#define COLOR_WHITE			"\033[37m"
#define COLOR_BOLDBLACK		"\033[1m\033[30m"
#define COLOR_BOLDRED		"\033[1m\033[31m"
#define COLOR_BOLDGREEN		"\033[1m\033[32m"
#define COLOR_BOLDYELLOW	"\033[1m\033[33m"
#define COLOR_BOLDBLUE		"\033[1m\033[34m"
#define COLOR_BOLDMAGENTA	"\033[1m\033[35m"
#define COLOR_BOLDCYAN		"\033[1m\033[36m"
#define COLOR_BOLDWHITE		"\033[1m\033[37m"

#define PATH_DATA	"data"
#define PATH_CFG	PATH_DATA"/game.cfg"
#define PATH_LOG	PATH_DATA"/game.log"
#define PATH_DEBUG	PATH_DATA"/game.debug"

/* écriture d'un message d'erreur dans le fichier log */
#define ERROR_NODESC	100
#define ERROR_ENGINE	101
#define ERROR_OPEN		102
#define ERROR_CLOSE		103
#define ERROR_MEMORY	104
#define ERROR_SYSTEM	105
#define ERROR_RENDER	106
void werror(const int error, const char *data, const char *function);

/* écriture d'un message d'avertissement dans le fichier log */
#define WARNING_NODESC	200
#define WARNING_CFG		201
#define WARNING_EXIT	202
void wwarning(const int warning, const char *data, const char *function);

/* écriture des valeurs des variables globales en mémoire sur le fichier de débogage */
void wdebug();

/* lecture du fichier de débogage */
int rdebug();

/* forcer la fermeture du programme */
void fexit();

/* attendre un nombre de secondes s */
void fsleep(const float s);

/* renvoyer un int aléatoire entre min et max */
int randi(const int min, const int max);

/* détecter si une touche du clavier a été frappée */
/** renvoie une valeur non nulle si vrai **/
int kbhit();

/* récupérer l'appuie d'une touche fléchée */
#define KEY_UP		65
#define KEY_DOWN	66
#define KEY_RIGHT	67
#define KEY_LEFT	68
/** renvoie une valeur non nulle si vrai **/
bool getarrow(int *c);

/* activer ou désactiver le curseur */
void setcur(const bool on);

/* écriture du fichier de configuration */
void wcfg();

/* lecture du fichier de configuration */
void rcfg();

/* utiliser la configuration par défaut */
void setdefcfg();

/* initialiser snafu */
void setengine(const bool on);

/* lecture et chargement en mémoire d'une image ascii */
char** rascii(const char *path);

/* nettoyage de l'écran */
void clear();

/* afficher une chaîne de caractères avec un délai entre chaque caractère */
/** le joueur peut passer l'animation en appuyant sur la touche entrée **/
void prints(const char *s);

/* afficher le menu principal et attendre une sélection du joueur */
int printmainmenu();

/* afficher l'interface avec une image ascii */
/** si l'image est nulle, affiche uniquement le hud **/
void printhud(char **image);

/* affiche SNAFU animé avec un déplacement aléatoire */
/** pour arrêter l'animation, appuyez sur échap, entrée ou espace **/
void printsnafufx();

#endif /* snafu.h  */
