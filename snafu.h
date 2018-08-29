#ifndef	_SNAFU_H
#define	_SNAFU_H	1

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#define CFG_HUD			"┌", "┐", "└", "┘", "─", "│"
#define CFG_IMG_X		78
#define CFG_IMG_Y		10
#define CFG_TEXT_LENGTH	80
#define CFG_TEXT_SPEED	0.05

#define MODE_MENU	0
#define MODE_DIAL	1
#define MODE_INV	2
#define MODE_MAP	3

typedef struct window
{
	int	cursor;
	int hud;
	int mode;
}	Window;

Window w;

typedef struct entity
{
	char	*name;
	int		attack;
	int		defense;
	int		maxhp;
	int		hp;
	float	level;
	float	xpmultiplier;
}	Entity;

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

/* affiche un dialogue et attend une action du joueur */
/** renvoie 0...n pour l'option choisie ou -1
 *  on suppose que l'image est parfaite, lol !
 **/
int rdial(char *image, char *dialog, float dt);

#endif /* snafu.h  */
