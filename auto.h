#ifndef AUTO_H_INCLUDED
#define AUTO_H_INCLUDED

/*
    ce fichier contient les prototypes des sous programmes appellés à chaque tour de boucle de jeu pour l'actualisation
*/

#include "load.h"

/* les types pour la fonction if_elapsed */
#define ANIMATION 0
#define WORK 1
#define TRANSLATION 2

/* le temps d'attente entre chaque image de l'animation */
#define DELAY 0.1


///met à jour toutes les unités
void update(Tile carte[MAPSIZEX][MAPSIZEY], std::list<Unit *>& ancre, std::list<Build *>& ancre_b, Joueur& joueur);

///l'actualisation d'une seule unité
void act_unit(Unit& unite, std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Joueur& joueur);

///déplace l'unité en fonction de la destination
void move_call(Tile carte[MAPSIZEX][MAPSIZEY], std::list<Unit *>& ancre, Unit& unite);

///décide du chemin que l'unité doit prendre
void path(Tile carte[MAPSIZEX][MAPSIZEY], std::list<Unit *>& ancre, Unit& unite);

///fait attaquer l'unité
void attack(std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Unit& unite);

///fait extraire des ressources par l'unité
void mine(Tile carte[MAPSIZEX][MAPSIZEY], Unit& unite, Joueur& joueur);

///Supprime une tuile en faisant attention à enlever les feuilles d'un arbre....
void enlev(Tile carte[MAPSIZEX][MAPSIZEY], int x, int y);

///renvoie une booléenne: si il faut passer ou non à l'image suivante ou faire attaquer l'unité
int if_elapsed(Unit& unite, int type);

///forme regarde le batiment et forme une unité s'il y en a une à former
void formation(std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], Build& build);

///repartit l'unité autours du batiment là où y'a de la place
int reparti(Build& bat, std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY], int *x, int *y);

///spawn des ennemis autours d'un batiment ennemi (et blindage)
void spawn_camp(Build& build, std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY]);

///spawn des ennemis aléatoirement sur la map
void spawn_map(Joueur& joueur, std::list<Unit *>& ancre, Tile carte[MAPSIZEX][MAPSIZEY]);

///dirige le paysant vers la prochaine ressource. renvoie si ça a marché ou pas
int next_res(Unit& unite, Tile carte[MAPSIZEX][MAPSIZEY]);

///pour une unité ennemie, regarde quoi faire
void automat(std::list<Unit *>& ancre, std::list<Build *>& ancre_b, Tile carte[MAPSIZEX][MAPSIZEY], Joueur& joueur, Unit& unite);

#endif // AUTO_H_INCLUDED
