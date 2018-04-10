#ifndef EVENNEMENT_H_INCLUDED
#define EVENNEMENT_H_INCLUDED

/*
    Ce fichier contient les prototypes des fonctions montrant un evenement spécifique
    Une action du joueur par exemple
*/

#include "chaine_b.h"
#include "defines.h"


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
