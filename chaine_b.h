#ifndef CHAINE_B_H_INCLUDED
#define CHAINE_B_H_INCLUDED

/*
    UNE GRANDE PARTIE DES SOUS-PROGRAMMES DE CE FICHIER ONT ÉTÉ RECICLÉS DU MINI PROJET TP COLLECTION SUR DREAM THEATER (par Leonardo Jeanteur)
    ce fichier contient les prototypes des sous programmes necessaires à la manipulation des maillons pour les batiments
*/

#include "chaine.h"

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
#define GUARD_DIST 7


///un batiment
typedef struct Batiment{
    int x, y;
    int w, h;   //width height

    ///en construction, normal ou detruit
    int state;
    ///ally ennemy
    int side;
    ///les points de vie
    int hp;
    ///le maximum de points de vie que cette unité peut avoir
    int hp_max;
    ///la portée d'attaque (et de vision) du batiment
    int range;
    ///les degats (s'il y en a) du batiment
    int damage;

    ///la capacité d'unités stationnées là (que pour les batiments ennemis)
    int cap;
    ///le nombre d'unités actuellement stationées là
    int statione;
    ///la queue de formation d'unité (que pour des batients alliés)
    int unit_queue[QUEUE_MAX];
    ///combien d'unités attendent pour être formées (que pour des batiments alliés)
    int curr_queue;
    ///l'instant au début de la formation de l'unité en cours de formation
    TIMESTRUCT start;

///un batiment
}Build;


#endif
