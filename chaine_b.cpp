#include "chaine_b.h"

#include "evennement.h"

Build::Build()
{

}

Build::Build(int _x, int _y, int _w, int _h, int _side, int _state)
    :x(_x), y(_y), w(_w), h(_h), state(_state), side(_side), hp_max(HP_BUILD_S),
    range(DEFAULT_RANGE), damage(DEFAULT_DMG), statione(0), curr_queue(0)
{
    //width height
    //l'état du batiment (normal, en construction..)
    //la portée d'attaque du batiment (ne sert pour l'instant à rien)
    //les dégats du batiment (ne sert pour l'instant à rien)

    //il n'y a aucune unité en cours de formation dans ce batiment

    hp = state?10:hp_max; //si le batiment commence à être construit il recevra les pv au fur et à mesure de la construction

    cap = side?0:5;
}

Build::~Build()
{

}
