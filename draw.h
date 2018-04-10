#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

/*
    ce fichier contient les prototypes des fonctions de dessin
*/

#include "auto.h"
#include "defines.h"



///dessine une "barre de chargement" pour l'hp etc
BITMAP *draw_status(double HPmax, double HPcurrent, int type);

///dessine l'UI avec les differents trucs dessus
void draw_ui(BITMAP *dest, std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Sprites& sprites, Joueur& joueur);

///dessine la minimap
BITMAP *minimap(std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Joueur& joueur, int vis);

///la boite en haut à droite de l'écran avec les ressources...
BITMAP *game_info(Joueur& joueur, Sprites& sprites);

///dessine tout sur la bitmap dest
void draw_screen(BITMAP *dest, std::list<Unit *>& ancre, std::list<Build *>& ancre_b, Tile carte[MAPSIZEX][MAPSIZEY], Sprites& sprites, Joueur& joueur);

#endif // DRAW_H_INCLUDED
