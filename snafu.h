#ifndef	_SNAFU_H
#define	_SNAFU_H	1

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_DT	0.05

/* affiche une chaîne de caractères avec un délai t entre chaque caractère */
void prints(char *s, float dt);

/* renvoie un int aléatoire entre min et max */
int randi(int min, int max);

/* renvoie un float aléatoire entre 0 et 1 */
float randf();

/* démarre snafu */
int start();

#endif /* snafu.h  */
