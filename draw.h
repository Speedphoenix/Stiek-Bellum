#ifndef DRAW_H_INCLUDED
#define DRAW_H_INCLUDED

/*
    ce fichier contient les prototypes des fonctions de dessin
*/

#include "auto.h"

#define BAT 0
#define UNIT 1
#define TIME 2

#define BAR_W 75
#define BAR_H 6//pour le health bar


///dessine une "barre de chargement" pour l'hp etc
BITMAP *draw_status(double HPmax, double HPcurrent, int type);

///dessine l'UI avec les differents trucs dessus
void draw_ui(BITMAP *dest, Ancre ancre, Tile carte[MAPSIZEX][MAPSIZEY], Sprites *sprites, Joueur joueur);

///dessine la minimap
BITMAP *minimap(Ancre ancre, Tile carte[MAPSIZEX][MAPSIZEY], Joueur joueur, int vis);

///la boite en haut à droite de l'écran avec les ressources...
BITMAP *game_info(Joueur joueur, Sprites *sprites);

///dessine tout sur la bitmap dest
void draw_screen(BITMAP *dest, Ancre ancre, Ancre_b ancre_b, Tile carte[MAPSIZEX][MAPSIZEY], Sprites *sprites, Joueur *joueur);

void apercu();

#endif // DRAW_H_INCLUDED
