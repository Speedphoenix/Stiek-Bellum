#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

/*
    ce fichier contient les prototypes des fonctions de menu
*/


#include "draw.h"
#include "defines.h"

///les menus
int menu(Sprites& sprites, Joueur& joueur, int type);

///met le jeu en ause, reprend quand le joueur l'a décidé
void pause_game(Joueur& joueur, Sprites& sprites, Tile carte[MAPSIZEX][MAPSIZEY], std::list<Unit *>& ancre, std::list<Build *>& ancre_b);

///à appeller à la fin d'une partie pour les meilleurs scores
void end_game(Sprites& sprites, Joueur& joueur);

///initialisation d'allegro
void init_alleg(int sizex, int sizey);


#endif // MENU_H_INCLUDED
