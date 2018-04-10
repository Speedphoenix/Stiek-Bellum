#ifndef LOAD_H_INCLUDED
#define LOAD_H_INCLUDED

/*
    Ce fichier contient les prototypes des sous programmes appellées uniquement au début du jeu
    merci à https://material.io/guidelines/style/color.html pour certaines couleurs
*/

#include "evennement.h"
#include "defines.h"


///la structure contenant les bitmaps
typedef struct Sprites{
    BITMAP *herbe[19];
    BITMAP *eau[13];
    BITMAP *montagne[13];
    BITMAP *arbre[5];
    BITMAP *pierre[1];
    BITMAP *batiment[2][8];
    BITMAP *camp[2][4];


    BITMAP *sword[2][NUMFRAMES];
    BITMAP *peasant[3][NUMFRAMES];
    BITMAP *ennemi[2][NUMFRAMES];
    BITMAP *souris;
    BITMAP *bois_i; //les icones
    BITMAP *pierre_i;
    BITMAP *viande_i;
    BITMAP *camp_i;
    BITMAP *enemy_i;
    BITMAP *pause_i[2];
    BITMAP *paysant_i;
    BITMAP *epee_i;
    BITMAP *mairie_i;
    BITMAP *caserne_i;

    BITMAP *flower[10];

    ///une zone non explorée
    BITMAP *brouillard;
    ///Le brouillard de guerre
    BITMAP *fog;


    BITMAP *fond[2];
    BITMAP *hover_unit[2][3];
    BITMAP *buttons[5][3][4]; //tous les boutons utilisés dans les "grands" menus
    BITMAP *flags;
    BITMAP *v_buttons[3][2];

    ///les petits menus en bs à gauche dans l'ui
    BITMAP *small_menu[3];
    ///la zone centrale de l'ui quand aucune unité n'est selectionnée
    BITMAP *c_bar;
    ///la zone où y'a le menu unité quand rien n'est séléctionné
    BITMAP *r_bar;
    ///le rectangle ignoré à gauche de la minimap
    BITMAP *ign_l;
    ///le rectangle ignoré en dessous de l'UI
    BITMAP *ign_d;
    ///le fond de la barre avec les ressources
    BITMAP *info_bar;

    ///l'cran précédent
    BITMAP * prev;
///la structure contenant les bitmaps
}Sprites;


///ce sous programme sert à définir les différents parametres d'une tuile
void set_tile(Tile& tile, char a);

///pour la création des petits menus dans l'UI
BITMAP *create_menu(Sprites& sprites, int type);

///ce sousprogramme sert à charger les bitmaps et sprites
void load_sprites(Sprites& sprites);

///chargement de toute la map dans le tableau vide et des bitmaps dans sprites
void load_game(Tile vide[MAPSIZEX][MAPSIZEY], std::list<Unit *>& ancre, std::list<Build *>& ancre_b, Joueur& joueur, int type);

///sauvegarde toute la partie dans un fichier de sauvegarde
void save_game(Tile carte[MAPSIZEX][MAPSIZEY], std::list<Unit *>& ancre, std::list<Build *>& ancre_b, Joueur& joueur, int num);

///remet tout à zero
void reset(std::list<Unit *>& ancre, std::list<Build *>& ancre_b, Joueur& joueur, Tile carte[MAPSIZEX][MAPSIZEY]);


#endif // LOAD_H_INCLUDED
