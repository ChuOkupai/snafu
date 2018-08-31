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

typedef struct ASCIISURFACE
{
    char **character;
}    ASCIISURFACE;

/* Ecrit une message d'erreur dans le fichier log */
void writerr(char *error);

/* nettoie l'écran */
void clear();

/* active ou désactive le curseur */
void setcur(int on);

/* charge le fichier de configuration */
void loadcfg();

/* initialise snafu */
void setengine(int on);

/* détecte si une touche du clavier a été frappée */
/** renvoie une valeur non nulle si vrai **/
int kbhit(void);

/* affiche une chaîne de caractères avec un délai entre chaque caractère */
/** le joueur peut passer l'animation en appuyant sur la touche entrée **/
void prints(char *s);

#endif /* snafu.h  */
