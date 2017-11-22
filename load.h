#ifndef LOAD_H_INCLUDED
#define LOAD_H_INCLUDED

/*
    Ce fichier contient les prototypes des sous programmes appellées uniquement au début du jeu
    merci à https://material.io/guidelines/style/color.html pour certaines couleurs
*/
#include "evennement.h"

/* les types de menu */
#define MAIN_MENU 0
#define PAUSE_MENU 1
#define MAP_MENU 2
#define DIFF_MENU 3
#define SAVE_MENU 4
#define LOAD_MENU 5


/* les fichiers de sauvegarde et chargement */
#define SAUV "Saves/"
#define VALS ".trc"
#define MAP ".map"
#define FOG ".fow"
#define NAME "%ssauv%d%s"
#define SCORE "Scores/"


/* des couleurs */
#define BLANC makecol(255,255,255)
#define NOIR makecol(0,0,0)
#define ROUGE makecol(255,0,0)
#define BLEU makecol(0,0,255)
#define VERT makecol(0,255,0)
#define MAG makecol (255,0,255)         //magenta
#define COL_UI_FOND makecol(1,87,155)
#define COL_UI_ACC makecol(79,195,247)
#define COL_SAND makecol(194,132,80)
#define COL_BRIDGE makecol(122,61,0)
#define COL_GRASS makecol(110,130,51)
#define COL_WATER makecol(48,140,187)
#define COL_MOUNTAIN makecol(83,83,83)
#define COL_ROCK makecol(128,128,128)
#define COL_TREEL makecol(78,145,101)   //arbre feuillu
#define COL_TREEP makecol(58,99,58)     //conifere
#define COL_CONSTR COL_UI_ACC           //couleur d'un batiment
#define COL_ENEMY ROUGE
#define COL_PEASANT makecol(34,93,255)
#define COL_SOLDIER BLEU

/* Pour la transparance du brouillard */
#define CLRBUF 0.6
#define CLRF 0.4

/* tailles des boutons dans le menu */
#define BOUTON_W 320
#define BOUTON_H 100
#define HOVER_W 90


/* Bitmaps des menus */
#define MENU_FOND "Res/Menu.bmp"
#define MENU_BUTTON "Res/Menu_buttons.bmp"
#define PAUSE_FOND "Res/Pause_menu.bmp"
#define PAUSE_BUTTON "Res/Pause_buttons.bmp"
#define MAP_BUTTON "Res/Map_buttons.bmp"
#define DIFF_BUTTON "Res/Diff_buttons.bmp"
#define SAVE_BUTTON "Res/Save_buttons.bmp"
#define FLAGS "Res/Flags.bmp"
#define VICTORY_BUTTON "Res/Victory_buttons.bmp"

#define MOUSE "Res/Mouse.bmp"

/* nombre d'images par animation */
#define NUMFRAMES 6

#define RES "Res/"

/* les noms des fichiers source (images) */
#define HERBE "%sGrass%d.bmp"

#define EAU "%sWater%d.bmp"

#define MONTAGNE "%sMountain%d.bmp"

#define FLEUR "%sflower%d.bmp"

#define ARBRE0 "Res/Tree_down.bmp"
#define ARBRE1 "Res/Tree_up.bmp"
#define ARBRE2 "Res/Pine_down.bmp"
#define ARBRE3 "Res/Pine_up.bmp"
#define ARBRE4 "Res/Tree_small.bmp"

#define PIERRE0 "Res/Rocks.bmp"

#define MAIRIE0 "%sMairie%d.bmp"
#define CASERNE0 "%sBaracks%d.bmp"
#define CAMP0 "%sCamp%d.bmp"

#define FOG_TILE "Res/fog.bmp"

#define EPEE "Res/swordsman.bmp"
#define PAYSANT "Res/peasant.bmp"
#define MONSTRE "Res/monster.bmp"

#define PAUSE_ICON "Res/Pause_icon.bmp"
#define PAUSE_ICON_HOVER "Res/Pause_icon_hover.bmp"
#define MAIRIE_ICON "Res/mairie_icon.bmp"
#define CASERNE_ICON "Res/caserne_icon.bmp"
#define PAYSANT_ICON "Res/peasant_icon.bmp"
#define EPEE_ICON "Res/sword_icon.bmp"
#define CAMP_ICON "Res/camp_icon_small.bmp"
#define BOIS "Res/Wood_icon.bmp"
#define PIERRE "Res/Rock_icon.bmp"
#define VIANDE "Res/burning_enemy_icon.bmp"
#define MANQUE "Res/enemy_icon_small.bmp"

#define BROUILLARD "Res/FogSw.bmp"

#define C_BAR "Res/Central_bar.bmp"
#define UNIT_MENU "Res/Unit_menu_template.bmp"
#define INFO_BAR "Res/Ressource_bar.bmp"
#define IGN_L "Res/Left_border.bmp"
#define IGN_D "Res/Down_border.bmp"

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
void set_tile(Tile *tile, char a, int x, int y);

///pour la création des petits menus dans l'UI
BITMAP *create_menu(Sprites sprites, int type);

///ce sousprogramme sert à charger les bitmaps et sprites
void load_sprites(Sprites *sprites);

///chargement de toute la map dans le tableau vide et des bitmaps dans sprites
void load_game(Tile vide[MAPSIZEX][MAPSIZEY], Ancre *ancre, Ancre_b *ancre_b, Joueur *joueur, int type);

///sauvegarde toute la partie dans un fichier de sauvegarde
void save_game(Tile carte[MAPSIZEX][MAPSIZEY], Ancre ancre, Ancre_b ancre_b, Joueur joueur, int num);

///remet tout à zero
void reset(Ancre *ancre, Ancre_b *ancre_b, Joueur *joueur, Tile carte[MAPSIZEX][MAPSIZEY], Sprites *sprites);


#endif // LOAD_H_INCLUDED
