#ifndef CHAINE_B_H_INCLUDED
#define CHAINE_B_H_INCLUDED


#include "unit.h"
#include "defines.h"


///un batiment
class Build{

    public: //member variables

        Coord m_pos;
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

    public:

        Build();
        Build(int _x, int _y, int _w, int _h, int _side, int _state);
        virtual ~Build();

        void sendStream(std::ostream& theStream, int version);
        void receiveStream(std::istream& theStream, Tile carte[MAPSIZEX][MAPSIZEY], int version);


///un batiment
};


#endif
