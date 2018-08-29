#ifndef	_SNAFU_H
#define	_SNAFU_H	1

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define CFG_HUD_0		"┌", "┐", "└", "┘", "─", "│"
#define CFG_HUD_1		"╔", "╗", "╚", "╝", "═", "║"
#define CFG_TEXT_SPEED	0.06

/* nettoie l'écran */
void clear();

/* active ou désactive le curseur */
void setcur(int on);

/* détecte si une touche du clavier a été frappée */
/** renvoie une valeur non nulle si vrai **/
int kbhit(void);

/* affiche une chaîne de caractères avec un délai dt entre chaque caractère */
/** le joueur peut passer l'animation en appuyant sur la touche entrée **/
void prints(char *s, float dt);

/* renvoie un int aléatoire entre min et max */
int randi(int min, int max);

/* renvoie un float aléatoire entre 0 et 1 */
float randf();

/* initialise snafu */
int start();

/* rendu de l'interface */
/** tlc : top left corner
 *  trc : top right corner
 *  llc : lower left corner
 *  lrc : lower right corner
 *  hl  : horizontal line
 *  vl  : vertical line
 **/
void hud(char *tlc, char *trc, char *llc, char *lrc, char *hl, char *vl);

#endif /* snafu.h  */
