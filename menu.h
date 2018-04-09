#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

/*
    ce fichier contient les prototypes des fonctions de menu
*/


#include "draw.h"

/* macros pour les fonctions menu et pause */
#define PAUSE_Y 75
#define PAUSE_W 700
#define PAUSE_H 800
#define ECART_H 40
#define ECART_P 150
#define ECART_M 180

#define NEWGAME 1
#define LOAD 2
#define EXIT 3
#define RESUME 1
#define SAVE 2


///les menus
int menu(Sprites& sprites, Joueur& joueur, int type);

///met le jeu en ause, reprend quand le joueur l'a décidé
void pause_game(Joueur& joueur, Sprites& sprites, Tile carte[MAPSIZEX][MAPSIZEY], Ancre& ancre, std::list<Build *>& ancre_b);

///à appeller à la fin d'une partie pour les meilleurs scores
void end_game(Sprites& sprites, Joueur& joueur);

///initialisation d'allegro
void init_alleg(int sizex, int sizey);


#endif // MENU_H_INCLUDED
