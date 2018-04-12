#ifndef CHAINE_H_INCLUDED
#define CHAINE_H_INCLUDED


#include "defines.h"
#include "timefuncs.h" //functions for measuring time N' stuff


class Build;

///le pas à faire (pour le pathfinding
typedef struct Step{
    int x, y;
    ///les points depuis le départ
    float g;
    ///les points à l'arrivée
    float h;
    ///la somme des deux
    float total;
    ///si c'est un pas "final" ie si le pas est à la limite de la portée ou bien à la destination
    int ending;

    ///le maillon suivant
    struct Step *next;
    ///le maillon précédent
    struct Step *prev;
    ///le pas precedent
    struct Step *from;
}Step;


///une unité
class Unit{

    public:

        int x, y;
        int xdest, ydest;

        ///le prochain pas à faire (PAS VRAIMENT ENCORE IMPLÉMENTÉ)
        Step *step;

        int xpath, ypath; //la direction court-terme à prendre, en tuiles
        ///booleenne s'il faut recalculer le chemin ou pas
        int changepath;

        ///les points de vie
        int hp;
        ///la valeur de quand l'unité est à max pv
        int hp_max;
        ///degats par actualisation
        int damage;
        ///ally enemy
        int side;
        ///le type d'unité (paysan, soldat, ennemi)
        int type;
        ///la taille occupée par cette unité (en poid/population)
        int unitsize;

        ///le numéro de l'image à afficher
        int frame;
        ///moving, attacking, dead
        int state;
        ///la priorité de l'action (si le joueur veut deplacer mais que l'unité se fait attaquer...)
        int priority;
        ///variable de précision, pour les paysants: si il extrait de la pierre ou du bois (ou qu'il marche etc...) necessaire pour l'animation, servira pour l'animation de mort
        int prec;
        ///dans quelle direction le perso regarde on multiplie par PI/4 pour avoir la direction
        int direction;
        ///ce qui est en train d'ataquer (pour incrémenter la viande quand l'unité decede)
        int predator;


        ///la taille de l'unité (physique)
        int cote;
        ///booleenne si l'unité peut ou pas extraire des ressources/construire
        int prod;
        ///pixels par tours de boucle de jeu
        int speed;
        ///la portée d'attaque/d'éxtraction de ressources
        int range;
        ///latence entre chaque attaque/extraction EN SECONDES
        float delay;
        ///la portée (en tuiles) dans laquelle l'unité peut découvrir la map
        int vision;

        ///le batiment associé à l'unité (s'il y en a un) sert surtout pour les ennemis stationés
        Build *bat;

        ///temps lors de la dernière image de l'animation
        TIMESTRUCT since_a;
        ///temps lors du la dernière attaqu/extraction (/déplacement?)
        TIMESTRUCT since_w;

        Unit();
        Unit(int type, int priority, int x, int y, Build *bat);
        virtual ~Unit();

///une unité
};


#endif // CHAINE_H_INCLUDED