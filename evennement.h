#ifndef EVENNEMENT_H_INCLUDED
#define EVENNEMENT_H_INCLUDED

/*
    Ce fichier contient les prototypes des fonctions montrant un evenement spécifique
    Une action du joueur par exemple
*/

#include "chaine_b.h"

/* la manière dont sont stockées toutes les cases dans le fichier de la mappe */
#define GRASS ','
#define TREE '?'
#define ROCK '@'
#define WATER '_'
#define BUILDING '#'
#define CAMP '/' //camp ennemi
#define MOUNTAIN '%' //terrain impassable


/* les numéros du carré d'herbe ordinaire, sable ordinaire et pont ordinaire dans la position d'une tuile d'herbe */
#define P_HERBE 0
#define P_SABLE 1
#define P_PONT 2


/* le temps à attendre entre chaque tour (en milisec) */
#define LAPSE 50
#define SCALE 1

/** POUR ENLEVER TOUS LES TRUCS QUI EMPECHENT DE TESTER... */
#define TEST 0
#define LAG 1
#define HOW_LONG 0

/* Les valeurs de base de differents trucs (resources) */
#define PETIT 500
#define GRAND 1000
#define T_GRAND 2000

/* Les points de vie de diverses unités */
#define HP_PEASANT 250
#define HP_SOLDIER 500
#define HP_ENEMY 500
#define HP_BUILD_S 1000
#define HP_BUILD_M 2000
#define HP_BUILD_L 5000

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
#define MAX_ENEMY 1000 //le maximum d'ennemis automatiques sur la map en même temps
#define SAFEZONE 12 //aucune unité ennemie ne peut spawn automatiquement dans cette zone autours d'un batiment (en tuiles)
#define TEMPS_ENNE_R 48.0
#define START_SPAWN 60.0


/* les états de passage d'une tuile (si une unité peut passer par là) */
#define VIDE 0
#define NAGE 1
#define SOLID 2

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
#define MAXRES 999990 //la quantité max d'un type de ressource

/* pour ne pas à avoir à réécrire à chaque fois */
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

/* les differentes valeurs pour les actions du joueur */
#define RIEN 0
#define SELECTING 1
#define SELECTED 2
#define SELECT_BUILD 3
#define PLACE_BUILD 4

/* les differentes valeurs pour les differents niveaux de difficulté */
#define EASY 1
#define HARD 2
#define HARDCORE 3
#define BALANCE(x) ((double)(x/joueur.level))

/* les differentes valeurs pour les differentes langues */
#define ENGLISH 0
#define FRANCAIS 1
#define ITALIANO 2

#define DEBUG 0

#if DEBUG

    int debugnum;
    FILE *debug;
    #define DEB(x) OPE fprintf(debug, "%s\n", x); CLO
    #define OPE debug = fopen((debugnum)?"debug0.txt":"debug1.txt", "w");
    #define CLO fclose(debug); debugnum = !debugnum;

#else

    #define DEB(x)
    #define OPE
    #define CLO

#endif // DEBUG



///structure contenant les valeurs du joueur humain (coordonnées de la caméra, quantité de ressources...)
typedef struct Human{
    int xcamera, ycamera; //les coordonnées du coin en haut à gauche de la caméra/écran en pixels par rapport à la map
    int xecran, yecran; //la taille de l'écran DOIT ÊTRE UN NOMBRE ENTIER DE TUILES
    ///s'il faut reactualiser l'écran
    int change;
    ///s'il faut aussi changer la taill de l'écran (zoom/dézoom)
    int chang_taill;
    ///l'angle precedent de la roulette de souris
    int prevwheel;
    ///un type de ressource
    int bois;
    ///un type de ressource
    int marbre;
    ///un type de ressource (cadavres ennemis)
    int viande;
    ///combien de batiments il reste à détruire pour gagner
    int nend_b;
    ///combien d'ennemis il reste à tuer pour gagner
    int nend_e;
    ///Booleene reste à zero tant que le joueur n'a pas demandé d'interompre
    int quit;
    ///Booleene reste à zero tant que le joueur n'a pas demandé pause
    int pause;
    ///la difficulté du jeu
    int level;
    ///la langue...
    int langue;
    ///le numéro de la map (pour plus tard enregistrer le score en fonction de la map)
    int map_num;

    ///les coordonnées de quand le joueur a commencé à cliquer ou les coordonnées du batiment séléctionné
    int xprev;
    ///les coordonnées de quand le joueur a commencé à cliquer ou les coordonnées du batiment séléctionné
    int yprev;
    ///l'action qui est en train d'être effectuée
    int act;
    ///le type de batiment à construire...
    int type;

    ///la liste chainée contnant toutes unitées selectionnées s'il y en a
    std::list<Unit *> selection;

    ///le mouse_b precedent (pour certaines action on ne regarde que le front montant/descendant
    int clic_prec;
    ///le temps lors du dernier clic (pour le double clic)
    TIMESTRUCT last_clic;

    ///le nombre d'ennemis (non stationaires) présents sur la map
    int nb_enemy;
    ///le temps lors du dernier spawn d'ennemi
    TIMESTRUCT last_spawn;

    ///Le temps initial au début de la partie
    TIMESTRUCT debut;

///structure contenant les valeurs du joueur humain (coordonnées de la caméra, quantité de ressources...)
}Joueur;


///Une tuile
typedef struct Tile{
    ///le type de tuile
    char type;
    ///Le "type" de ce type de tuile (partie en haut à gauche de l'arbre?)
    int position;
    ///La fleur présente dessus (s'il y en a une)
    int fleur;

    ///booleene: si une unite peut passer par là ou pas
    int block;
    ///la quantité de ressources restante sur cette tuile
    int res;
    ///booleene si la case à déjà été découverte ou pas
    int visible;
    ///le batiment présent à cette case (s'il y en a un)
    Build *erige;

///une tuile
}Tile;


///verifie si on peut ajouter le batiment à ces coordonnées et le place et l'ajoute à la liste chainée (renvoie 0 sinon)
bool add_bat(std::list<Build *>& ancre_b, std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], int x, int y, int state, int type);

///detruit le batiment (l'enleve de l'ancre, et de la map)
void destroy_build(std::list<Build *>& ancre, std::list<Build *>::iterator& del, Tile carte[MAPSIZEX][MAPSIZEY]);


///prend les actions du joueur (action avec la souris) ancre est l'ancre de tous les persos
void action(std::list<Unit *>& ancre, std::list<Build *>& ancre_b, Joueur& joueur, Tile carte[MAPSIZEX][MAPSIZEY]);

///prend les actions du joueur sur le terrain de jeu (la map)
void action_ecran(std::list<Unit *>& ancre, std::list<Build *>& ancre_b, Joueur& joueur, Tile carte[MAPSIZEX][MAPSIZEY], int x, int y);

///prend les actions du joueur sur l'UI
void action_ui(std::list<Unit *>& ancre, Joueur& joueur, Tile carte[MAPSIZEX][MAPSIZEY], int x, int y);

///renvoie quel bouton a été cliqué sur le menu unité (0 si rien)
int unit_menu_click(int x, int y);


///renvoie un pointeur sur l'unité présente à ces coordonnées (NULL s'il n'y en a pas) side_excl permet d'ignorer la population ennemie (mettre 2 ou 3 pour ne pas l'utiliser)
Unit *trouve(std::list<Unit *>& ancre, int x, int y, Unit *exclu, int side_excl);

///découvre les tuiles autours des coordonnées dans la portée
void eclaire(Tile carte[MAPSIZEX][MAPSIZEY], int x, int y, int portee);

///renvoie une booleenne: si la distance est inferieure ou non
int if_dist(int x1, int y1, int x2, int y2, int dist);

///rempli l'ancre dest avec les unités comprises dans la selection
void selec(std::list<Unit *>& dest, std::list<Unit *>& ancre, int x1, int y1, int x2, int y2);

///pour voir l'optimisation
void temps_passe(TIMESTRUCT& prev);

///libere une liste chainée de steps
void free_path(Step **ancre);

///passe l'unité au pas suivant
void next_step(Step **ancre);



#endif // EVENNEMENT_H_INCLUDED
