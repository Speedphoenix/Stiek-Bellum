#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

/***************************************************
                    INCLUDES
****************************************************/

#include <iostream>
#include <list>
#include <allegro.h>

//these have to be removed
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


/***************************************************
                TESTING PURPOSES
****************************************************/

/* pour enlever tous les trucs qui empechent de tester... */
#define TEST 0
#define LAG 1       //fog of war avancé par exemple
#define HOW_LONG 0  //affiche le temps écoulé à chaque tour de boucle

#define DEBUG 1

#if DEBUG

/* pour tester rapidement à droite et à gauche*/
#define E(x) {std::cerr << std::endl << #x " : " << x << std::endl;}

#else

#define E(x)

#endif // DEBUG

/* renvoie une erreur de chargement */
#define ERR_CHARG(x) {if(!x){allegro_message("probleme de chargement " #x);exit(1);}}


/***************************************************
                UNIT and BUILD
****************************************************/

/*les valeurs de side/type de l'unité*/
#define ENEMY 0
#define ALLY 1
#define NEUTR 2
#define SOLDIER 1
#define PEASANT 2
#define MAIRIE 0    //pour les selections, ou le choix à construire, y'a peasant, mairie ou caserne
#define CASERNE 1



/***************************************************
                        UNIT
****************************************************/

/*les valeurs d'état de l'unité*/ ///A METTRE EN ENUM
#define MOVING 0
#define ATTACK 1
#define CHOP 1
#define MINE 2
#define DEAD 10

/*les valeurs de priorité en cours de l'action d'une unité*/
#define STAND 0     //si l'unité ne bouge pas
#define AUTO 2      //si l'unité fait une action automatique
#define GUARD 4     //si l'unite tient la garde (ira attaquer automatiquement, mais ne se déplace pas toute seule)
#define TAUNT 6     //si l'unité attaque parcequ'elle est attaquée
#define ORDRE 8     //si l'unité a reçu un ordre du joueur
#define CONSTR 10   //si l'unité est en train de construire qcc

/*valeurs d'éxemple de direction dans laquelle l'unité est dirigée...*/
#define RIGHT 0
#define LEFT 4 //on n'utilse que right et left...
#define UP 2
#define DOWN 6



/***************************************************
                        BUILD
****************************************************/

#define NUM_MAIRIE 0
#define NUM_CASERNE 4

/* les etats possibles pour un batiment */
#define NORMAL 0
#define PROGRESS 1
#define DESTRUCT 10

#define QUEUE_MAX 10

/* temps de formation des unites */
#define TEMPS_PAYS 5.0
#define TEMPS_SOLD 10.0
#define TEMPS_ENNE_B 30.0

/* la distance d'un batiment à l'encroit où il peut spawn une unité */
#define FORMRANGE 50    //en pixels
#define SPAWNRANGE 300  //en pixels
#define DEFRANGE 3      //en tuiles
#define GUARD_DIST 7    //en tuiles

/* Des valeurs d'attaque des bâtiments... pas encore utilisé */
#define DEFAULT_RANGE 150
#define DEFAULT_DMG 40



/***************************************************
                GRAPHICAL VALUES
****************************************************/
/* Tout en pixels */

/* Taille de l'écran (entier) */
#define XSCREEN (ECRANX * COTE)
#define YSCREEN (ECRANY * COTE + UI_HEIGHT + MOVELIMIT)

/* des valeurs */
#define MAPSIZEX 300
#define MAPSIZEY 225
#define COTE 75
#define ECRANX 20 //les tailles de l'écran par défaud/Ideale
#define ECRANY 10
#define MAXECRANX 40
#define MAXECRANY 20
#define MINECRANX 10
#define MINECRANY 5
#define UI_HEIGHT 225
#define MAPWIDTH 300 //la taille de la minimap (la même que la taille de toute la map)

/* conversion pixels => tuiles */
#define DIV(x) ((int)((x)/COTE))

/* les macros utilisées par l'UI */
#define SEPARE 1050 //là où sont séparés l'affichage selection du menu individuel
#define PAUSE_S 30 //la taille (cote) de l'icone de pause

/* Macros pour le menu utilisateur */
#define ICON_S 45
#define BOX_H 75
#define BOX_W 225
#define RES_W 75
#define MARGE 5

/* boite d'info en haut à droite. ces valeurs s'éxprimment dans le referrentiel de la boite */
#define RES_S 20
#define ISIZEX 525  //taille de la boite d'info
#define ISIZEY 30   //taille de la boite d'info
#define XCONT 20
#define YTEXT 11
#define YICON 5
#define XBOISI (XCONT + 10)   //icone
#define XBOIST (XCONT + 33)   //texte
#define XPIERREI (XCONT + 108)
#define XPIERRET (XCONT + 131)
#define XVIANDEI (XCONT + 207)
#define XVIANDET (XCONT + 230)
#define XREMAINI (XCONT + 307)
#define XREMAINT (XCONT + 330)
#define XTEMPS (ISIZEX - (XCONT + 65))

/* l'écran se déplace quand la souris est à certains pixels du bord */
#define MOVELIMIT 25

/* loading/health bar */
#define BAR_W 75
#define BAR_H 6

/* for the color of the loading/health bar */
#define BAT 0   //building health
#define UNIT 1  //unit health
#define TIME 2  //time loading

/* valeurs graphiques pour les fonctions menu et pause */
#define PAUSE_Y 75
#define PAUSE_W 700
#define PAUSE_H 800
#define ECART_H 40
#define ECART_P 150
#define ECART_M 180

/* tailles des boutons dans le menu */
#define BOUTON_W 320
#define BOUTON_H 100
#define HOVER_W 90



/***************************************************
                    COLORS
****************************************************/

/* des couleurs */
#define BLANC makecol(255,255,255)
#define NOIR makecol(0,0,0)
#define ROUGE makecol(255,0,0)
#define BLEU makecol(0,0,255)
#define VERT makecol(0,255,0)
#define MAG makecol (255,0,255)         //magenta, invisible
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



/***************************************************
                      TILES
****************************************************/

/* la manière dont sont stockées toutes les cases dans le fichier de la mappe */
#define GRASS ','
#define TREE '?'
#define ROCK '@'
#define WATER '_'
#define BUILDING '#'
#define CAMP '/'        //camp ennemi
#define MOUNTAIN '%'    //terrain impassable

/* les numéros du carré d'herbe ordinaire, sable ordinaire et pont ordinaire dans la position d'une tuile d'herbe */
#define P_HERBE 0
#define P_SABLE 1
#define P_PONT 2

/* les états de passage d'une tuile (si une unité peut passer par là) */
#define VIDE 0
#define NAGE 1
#define SOLID 2



/***************************************************
                GLOBALY NEEDED VALUES
****************************************************/

/* le temps à attendre entre chaque tour (en milisec) */
#define LAPSE 50    //en milisec
#define SCALE 1     //facteur a multiplier chaque autre valeur de temps

/* le temps d'attente entre chaque image d'une animation */
#define DELAY 0.1

/* les differentes valeurs pour les differentes langues */
#define ENGLISH 0
#define FRANCAIS 1
#define ITALIANO 2



/***************************************************
                 GAME BALANCE
****************************************************/

/* les differentes valeurs pour les differents niveaux de difficulté */
#define EASY 1
#define HARD 2
#define HARDCORE 3
#define BALANCE(x) ((double)(x/joueur.level)) ///A CHANGER!!! PRENDRE LEVEL EN PARAM

/* Les points de vie de diverses unités */
#define HP_PEASANT 250
#define HP_SOLDIER 500
#define HP_ENEMY 500
#define HP_BUILD_S 1000
#define HP_BUILD_M 2000
#define HP_BUILD_L 5000

/* Les valeurs de base de differents trucs (resources) */
#define PETIT 500
#define GRAND 1000
#define T_GRAND 2000

/* la quantité max d'un type de ressource pour le joueur */
#define MAXRES 999990

/* les ressource requises pour differents trucs */
#define WOOD_SOLD 40
#define ROCK_SOLD 80
#define WOOD_PEAS 80
#define ROCK_PEAS 20
#define WOOD_ENEMY 100
#define ROCK_ENEMY 200
#define WOOD_MAINB 400
#define ROCK_MAINB 300
#define WOOD_BARACKS 600
#define ROCK_BARACKS 400

/* les valeur pour le spawn aléatoire des ennemis */
#define MAX_ENEMY 1000      //le maximum d'ennemis automatiques sur la map en même temps
#define SAFEZONE 12         //aucune unité ennemie ne peut spawn automatiquement dans cette zone autours d'un batiment allié (en tuiles)
#define TEMPS_ENNE_R 48.0
#define START_SPAWN 60.0    //le temps à partir du début du jeu où commencent à arriver les ennemis



/***************************************************
                PLAYER VALUES
****************************************************/

/* les differentes valeurs pour les actions du joueur */ ///A METTRE DANS UNE ENUM
#define RIEN 0
#define SELECTING 1
#define SELECTED 2
#define SELECT_BUILD 3
#define PLACE_BUILD 4



/***************************************************
                    OTHERS
****************************************************/

/* les types pour la fonction if_elapsed */
#define ANIMATION 0
#define WORK 1
#define TRANSLATION 2   //not used yet

/* les valeurs de retour possible pour un menu */
#define NEWGAME 1
#define LOAD 2
#define EXIT 3
#define RESUME 1
#define SAVE 2

/* les types de menu */ ///A METTRE EN ENUM
#define MAIN_MENU 0
#define PAUSE_MENU 1
#define MAP_MENU 2
#define DIFF_MENU 3
#define SAVE_MENU 4
#define LOAD_MENU 5

/* nombre d'images par animation */
#define NUMFRAMES 6

/* Pour la transparance du brouillard */
#define CLRBUF 0.6
#define CLRF 0.4


/***************************************************
                    FILENAMES
****************************************************/

//pour faire qqc natif à Windows...
#ifdef _WINDOWS

#define FOLDERPATH "\\"

#else //sous linux etc...

#define FOLDERPATH "/"

#endif // _WINDOWS

/* les fichiers de sauvegarde et chargement */
#define SAUV "Saves" FOLDERPATH
#define VALS ".trc"
#define MAP ".map"
#define FOG ".fow"
#define NAME "%ssauv%d%s"
#define SCORE "Scores" FOLDERPATH

/* le dossier qui contient toutes les bitmaps (sans les temporaires) */
#define RES "Res" FOLDERPATH

/* Bitmaps des menus */
#define MENU_FOND RES "Menu.bmp"
#define MENU_BUTTON RES "Menu_buttons.bmp"
#define PAUSE_FOND RES "Pause_menu.bmp"
#define PAUSE_BUTTON RES "Pause_buttons.bmp"
#define MAP_BUTTON RES "Map_buttons.bmp"
#define DIFF_BUTTON RES "Diff_buttons.bmp"
#define SAVE_BUTTON RES "Save_buttons.bmp"
#define FLAGS RES "Flags.bmp"
#define VICTORY_BUTTON RES "Victory_buttons.bmp"

/* l'image de la souris */
#define MOUSE RES "Mouse.bmp"

/* les noms des fichiers source (images) */
#define HERBE "%sGrass%d.bmp"

#define EAU "%sWater%d.bmp"

#define MONTAGNE "%sMountain%d.bmp"

#define FLEUR "%sflower%d.bmp"

#define ARBRE0 RES "Tree_down.bmp"
#define ARBRE1 RES "Tree_up.bmp"
#define ARBRE2 RES "Pine_down.bmp"
#define ARBRE3 RES "Pine_up.bmp"
#define ARBRE4 RES "Tree_small.bmp"

#define PIERRE0 RES "Rocks.bmp"

#define MAIRIE0 "%sMairie%d.bmp"
#define CASERNE0 "%sBaracks%d.bmp"
#define CAMP0 "%sCamp%d.bmp"

#define FOG_TILE RES "fog.bmp"

#define EPEE RES "swordsman.bmp"
#define PAYSANT RES "peasant.bmp"
#define MONSTRE RES "monster.bmp"

#define PAUSE_ICON RES "Pause_icon.bmp"
#define PAUSE_ICON_HOVER RES "Pause_icon_hover.bmp"
#define MAIRIE_ICON RES "mairie_icon.bmp"
#define CASERNE_ICON RES "caserne_icon.bmp"
#define PAYSANT_ICON RES "peasant_icon.bmp"
#define EPEE_ICON RES "sword_icon.bmp"
#define CAMP_ICON RES "camp_icon_small.bmp"
#define BOIS RES "Wood_icon.bmp"
#define PIERRE RES "Rock_icon.bmp"
#define VIANDE RES "burning_enemy_icon.bmp"
#define MANQUE RES "enemy_icon_small.bmp"

#define BROUILLARD RES "FogSw.bmp"

/* les trucs dans l'UI */
#define C_BAR RES "Central_bar.bmp"
#define UNIT_MENU RES "Unit_menu_template.bmp"
#define INFO_BAR RES "Ressource_bar.bmp"
#define IGN_L RES "Left_border.bmp"
#define IGN_D RES "Down_border.bmp"



#endif // DEFINES_H_INCLUDED

