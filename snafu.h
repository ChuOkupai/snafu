#ifndef	_SNAFU_H
#define	_SNAFU_H	1

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define DEFAULT_DT	0.05

/* détecte si une touche du clavier a été frappée */
/** renvoie une valeur non nulle si vrai **/
int kbhit(void);

/* attend la confirmation du joueur en appuyant sur entrée */
void wconfirm();

/* affiche une chaîne de caractères avec un délai t entre chaque caractère */
/** le joueur peut passer l'animation en appuyant sur la touche entrée ou espace **/
void prints(char *s, float dt);

/* renvoie un int aléatoire entre min et max */
int randi(int min, int max);

/* renvoie un float aléatoire entre 0 et 1 */
float randf();

/* démarre snafu */
int start();

#endif /* snafu.h  */
